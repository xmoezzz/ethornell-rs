package com.xmoezzz.ethornell;

import android.app.Activity;
import android.app.AlertDialog;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.text.InputType;
import android.util.Log;
import android.view.Choreographer;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowInsets;
import android.view.WindowInsetsController;
import android.view.WindowManager;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.window.OnBackInvokedDispatcher;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

/**
 * Plays one game. The engine runs on its own thread, stepped by that
 * thread's Choreographer; the UI thread forwards surface, touch and key
 * events to it and shows the game's confirmation dialogs.
 */
public final class GameActivity extends Activity
        implements SurfaceHolder.Callback, Choreographer.FrameCallback {
    static final String EXTRA_GAME = "game";
    private static final String TAG = "ethornell";

    private final Handler ui = new Handler(Looper.getMainLooper());
    private HandlerThread engineThread;
    private Handler engine;
    private SurfaceView view;

    // Engine thread only.
    private long handle;
    private long generation;
    private boolean running;
    private final long[] eventMeta = new long[3];

    // UI thread only: the touch gesture in progress.
    private boolean leftDown;
    private boolean twoFinger;
    private boolean twoFingerMoved;
    private float twoFingerY;
    private float wheelTravel;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        view = new SurfaceView(this);
        view.setFocusable(true);
        view.setFocusableInTouchMode(true);
        view.getHolder().addCallback(this);
        view.setOnTouchListener((v, event) -> onTouch(event));
        setContentView(view);
        hideSystemBars();
        if (Build.VERSION.SDK_INT >= 33) {
            getOnBackInvokedDispatcher()
                    .registerOnBackInvokedCallback(
                            OnBackInvokedDispatcher.PRIORITY_DEFAULT, this::rightClickCenter);
        }

        String game = getIntent().getStringExtra(EXTRA_GAME);
        GameLibrary library = new GameLibrary(this);
        File root = library.gameDir(game);
        File user = library.userDir(game);
        int encoding = library.textEncoding(game);
        setTitle(game);

        engineThread = new HandlerThread("ethornell-engine");
        engineThread.start();
        engine = new Handler(engineThread.getLooper());
        engine.post(() -> {
            NativeEngine.nativeInit(getApplicationContext());
            handle = NativeEngine.nativeCreate(
                    root.getAbsolutePath(), user.getAbsolutePath(), "", encoding, 0);
            if (handle == 0) {
                fail(NativeEngine.nativeLastError());
                return;
            }
            running = true;
            Choreographer.getInstance().postFrameCallback(this);
        });
    }

    // ---- engine thread ----

    @Override
    public void doFrame(long frameTimeNanos) {
        if (!running) {
            return;
        }
        int status = NativeEngine.nativeStep(handle, System.nanoTime());
        if (status < 0) {
            fail(NativeEngine.nativeLastError());
            return;
        }
        drainEvents();
        if (running) {
            Choreographer.getInstance().postFrameCallback(this);
        }
    }

    private void drainEvents() {
        String text;
        while ((text = NativeEngine.nativePollEvent(handle, eventMeta)) != null) {
            int kind = (int) eventMeta[0];
            long requestId = eventMeta[1];
            int code = (int) eventMeta[2];
            String message = text;
            switch (kind) {
                case NativeEngine.EVENT_DIAGNOSTIC:
                    Log.i(TAG, message);
                    break;
                case NativeEngine.EVENT_CONFIRMATION:
                    ui.post(() -> showRequest(requestId, code, message));
                    break;
                case NativeEngine.EVENT_CLOSED:
                    running = false;
                    ui.post(this::finish);
                    return;
                case NativeEngine.EVENT_TITLE:
                    ui.post(() -> setTitle(message));
                    break;
                case NativeEngine.EVENT_DIALOG: {
                    String payload = NativeEngine.nativeRequestPayload(handle, requestId);
                    ui.post(() -> showDialog(requestId, payload));
                    break;
                }
                default:
                    break;
            }
        }
    }

    private void fail(String message) {
        running = false;
        Log.e(TAG, "engine stopped: " + message);
        ui.post(() -> {
            if (isFinishing()) {
                return;
            }
            new AlertDialog.Builder(this)
                    .setTitle(R.string.engine_error)
                    .setMessage(message)
                    .setCancelable(false)
                    .setPositiveButton(android.R.string.ok, (dialog, which) -> finish())
                    .show();
        });
    }

    private void input(int kind, float x, float y, int code, float value) {
        engine.post(() -> {
            if (handle != 0 && running) {
                NativeEngine.nativeInput(handle, kind, x, y, code, value);
            }
        });
    }

    // ---- UI thread ----

    /** A request that takes a result is a yes/no question (1 yes, 0 no). */
    private void showRequest(long requestId, int outputs, String text) {
        AlertDialog.Builder builder = new AlertDialog.Builder(this)
                .setMessage(text)
                .setCancelable(false);
        if (outputs != 0) {
            builder.setPositiveButton(android.R.string.yes, (d, w) -> complete(requestId, 1))
                    .setNegativeButton(android.R.string.no, (d, w) -> complete(requestId, 0));
        } else {
            builder.setPositiveButton(android.R.string.ok, (d, w) -> complete(requestId, 0));
        }
        builder.show();
    }

    /**
     * An input dialog (event kind 5). The engine filters and limits the
     * answers itself, so plain text fields are enough here.
     */
    private void showDialog(long requestId, String payload) {
        JSONObject request;
        try {
            request = new JSONObject(payload == null ? "" : payload);
        } catch (JSONException e) {
            completeDialog(requestId, null);
            return;
        }
        String type = request.optString("type");
        AlertDialog.Builder builder = new AlertDialog.Builder(this)
                .setTitle(request.optString("title", null))
                .setCancelable(false)
                .setNegativeButton(android.R.string.cancel, (d, w) -> completeDialog(requestId, null));
        String prompt = request.optString("prompt", "");
        List<EditText> fields = new ArrayList<>();
        LinearLayout form = new LinearLayout(this);
        form.setOrientation(LinearLayout.VERTICAL);
        int padding = (int) (16 * getResources().getDisplayMetrics().density);
        form.setPadding(padding, 0, padding, 0);
        switch (type) {
            case "input": {
                JSONArray specs = request.optJSONArray("fields");
                for (int i = 0; specs != null && i < specs.length(); i++) {
                    JSONObject spec = specs.optJSONObject(i);
                    fields.add(addField(form, spec.optString("label"), spec.optString("initial"),
                            spec.optBoolean("numeric")));
                }
                break;
            }
            case "segmented": {
                int count = request.optInt("segments");
                for (int i = 0; i < count; i++) {
                    fields.add(addField(form, (i + 1) + "/" + count, "", request.optBoolean("numeric")));
                }
                break;
            }
            case "select": {
                JSONArray options = request.optJSONArray("options");
                String[] labels = new String[options == null ? 0 : options.length()];
                for (int i = 0; i < labels.length; i++) {
                    labels[i] = options.optString(i);
                }
                builder.setItems(labels, (d, which) -> {
                    JSONObject answer = new JSONObject();
                    try {
                        answer.put("index", which);
                    } catch (JSONException e) {
                        throw new IllegalStateException(e);
                    }
                    completeDialog(requestId, answer);
                }).show();
                return;
            }
            case "date": {
                JSONArray initial = request.optJSONArray("fields");
                for (int i = 0; i < 4; i++) {
                    fields.add(addField(form, getString(R.string.dialog_field, i + 1),
                            initial == null ? "" : initial.optString(i), false));
                }
                fields.add(addField(form, getString(R.string.dialog_month),
                        String.valueOf(request.optInt("month") + 1), true));
                fields.add(addField(form, getString(R.string.dialog_day),
                        String.valueOf(request.optInt("day") + 1), true));
                break;
            }
            default:
                completeDialog(requestId, null);
                return;
        }
        if (!prompt.isEmpty()) {
            builder.setMessage(prompt);
        }
        builder.setView(form).setPositiveButton(android.R.string.ok, (d, w) -> {
            JSONObject answer = new JSONObject();
            JSONArray texts = new JSONArray();
            try {
                if (type.equals("date")) {
                    for (int i = 0; i < 4; i++) {
                        texts.put(fields.get(i).getText().toString());
                    }
                    answer.put("month", number(fields.get(4)) - 1);
                    answer.put("day", number(fields.get(5)) - 1);
                } else {
                    for (EditText field : fields) {
                        texts.put(field.getText().toString());
                    }
                }
                answer.put("texts", texts);
            } catch (JSONException e) {
                throw new IllegalStateException(e);
            }
            completeDialog(requestId, answer);
        }).show();
    }

    private EditText addField(LinearLayout form, String hint, String initial, boolean numeric) {
        EditText field = new EditText(this);
        field.setHint(hint);
        field.setText(initial);
        field.setSingleLine(true);
        if (numeric) {
            field.setInputType(InputType.TYPE_CLASS_NUMBER | InputType.TYPE_NUMBER_FLAG_SIGNED);
        }
        form.addView(field);
        return field;
    }

    private static int number(EditText field) {
        try {
            return Integer.parseInt(field.getText().toString().trim());
        } catch (NumberFormatException e) {
            return 1;
        }
    }

    /** {@code answer} null cancels the dialog. */
    private void completeDialog(long requestId, JSONObject answer) {
        String json;
        try {
            JSONObject object = answer == null ? new JSONObject() : answer;
            object.put("accepted", answer != null);
            json = object.toString();
        } catch (JSONException e) {
            throw new IllegalStateException(e);
        }
        engine.post(() -> {
            if (handle != 0) {
                NativeEngine.nativeCompleteDialog(handle, requestId, json);
            }
        });
    }

    private void complete(long requestId, int result) {
        engine.post(() -> {
            if (handle != 0) {
                NativeEngine.nativeCompleteRequest(handle, requestId, result);
            }
        });
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {}

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        Surface surface = holder.getSurface();
        engine.post(() -> {
            if (handle == 0) {
                return;
            }
            if (width == 0 || height == 0) {
                NativeEngine.nativeDetachSurface(handle, generation);
                return;
            }
            generation++;
            if (NativeEngine.nativeSetSurface(handle, surface, width, height, generation) < 0) {
                fail(NativeEngine.nativeLastError());
            }
        });
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        // The engine must be done with the window before it goes away.
        CountDownLatch detached = new CountDownLatch(1);
        engine.post(() -> {
            if (handle != 0) {
                NativeEngine.nativeDetachSurface(handle, generation);
            }
            detached.countDown();
        });
        try {
            detached.await(5, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        hideSystemBars();
        engine.post(() -> {
            if (handle != 0) {
                NativeEngine.nativeSetPaused(handle, 0);
            }
        });
    }

    @Override
    protected void onPause() {
        engine.post(() -> {
            if (handle != 0) {
                NativeEngine.nativeSetPaused(handle, NativeEngine.PAUSE_ACTIVITY);
            }
        });
        super.onPause();
    }

    @Override
    protected void onDestroy() {
        engine.post(() -> {
            running = false;
            if (handle != 0) {
                NativeEngine.nativeDestroy(handle);
                handle = 0;
            }
        });
        engineThread.quitSafely();
        super.onDestroy();
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            hideSystemBars();
        }
    }

    @SuppressWarnings("deprecation")
    private void hideSystemBars() {
        if (Build.VERSION.SDK_INT >= 30) {
            WindowInsetsController controller = getWindow().getInsetsController();
            if (controller != null) {
                controller.hide(WindowInsets.Type.systemBars());
                controller.setSystemBarsBehavior(
                        WindowInsetsController.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE);
            }
        } else {
            getWindow().getDecorView().setSystemUiVisibility(
                    View.SYSTEM_UI_FLAG_FULLSCREEN
                            | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                            | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                            | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                            | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                            | View.SYSTEM_UI_FLAG_LAYOUT_STABLE);
        }
    }

    /**
     * One finger is the left button; a two-finger tap is a right click (the
     * game menu); a two-finger vertical drag turns the wheel (the backlog).
     */
    private boolean onTouch(MotionEvent event) {
        float x = event.getX(0);
        float y = event.getY(0);
        switch (event.getActionMasked()) {
            case MotionEvent.ACTION_DOWN:
                twoFinger = false;
                leftDown = true;
                input(NativeEngine.INPUT_MOVE, x, y, 0, 0);
                input(NativeEngine.INPUT_BUTTON_DOWN, x, y, NativeEngine.BUTTON_LEFT, 0);
                return true;
            case MotionEvent.ACTION_POINTER_DOWN:
                if (event.getPointerCount() == 2 && !twoFinger) {
                    // Not a left press after all.
                    twoFinger = true;
                    twoFingerMoved = false;
                    twoFingerY = averageY(event);
                    wheelTravel = 0;
                    leftDown = false;
                    input(NativeEngine.INPUT_CANCEL, x, y, 0, 0);
                }
                return true;
            case MotionEvent.ACTION_MOVE:
                if (twoFinger) {
                    float averageY = averageY(event);
                    wheelTravel += averageY - twoFingerY;
                    twoFingerY = averageY;
                    float notch = 48 * getResources().getDisplayMetrics().density;
                    while (Math.abs(wheelTravel) >= notch) {
                        twoFingerMoved = true;
                        // Fingers moving down scroll up, like a wheel turned up.
                        float delta = Math.signum(wheelTravel);
                        wheelTravel -= delta * notch;
                        input(NativeEngine.INPUT_WHEEL, x, y, 0, delta);
                    }
                } else if (leftDown) {
                    input(NativeEngine.INPUT_MOVE, x, y, 0, 0);
                }
                return true;
            case MotionEvent.ACTION_POINTER_UP:
                if (twoFinger && !twoFingerMoved && event.getPointerCount() == 2) {
                    twoFingerMoved = true;
                    rightClick(x, y);
                }
                return true;
            case MotionEvent.ACTION_UP:
                if (leftDown) {
                    input(NativeEngine.INPUT_MOVE, x, y, 0, 0);
                    input(NativeEngine.INPUT_BUTTON_UP, x, y, NativeEngine.BUTTON_LEFT, 0);
                }
                leftDown = false;
                twoFinger = false;
                return true;
            case MotionEvent.ACTION_CANCEL:
                leftDown = false;
                twoFinger = false;
                input(NativeEngine.INPUT_CANCEL, x, y, 0, 0);
                return true;
            default:
                return false;
        }
    }

    private static float averageY(MotionEvent event) {
        float sum = 0;
        int count = Math.min(event.getPointerCount(), 2);
        for (int i = 0; i < count; i++) {
            sum += event.getY(i);
        }
        return sum / count;
    }

    private void rightClick(float x, float y) {
        input(NativeEngine.INPUT_BUTTON_DOWN, x, y, NativeEngine.BUTTON_RIGHT, 0);
        input(NativeEngine.INPUT_BUTTON_UP, x, y, NativeEngine.BUTTON_RIGHT, 0);
    }

    private void rightClickCenter() {
        rightClick(view.getWidth() / 2f, view.getHeight() / 2f);
    }

    @SuppressWarnings("deprecation")
    @Override
    public void onBackPressed() {
        // Back opens or closes the game's menu, like a right click.
        rightClickCenter();
    }

    /** Windows virtual-key codes, which the scripts bind input classes to. */
    private static int virtualKey(int keyCode) {
        switch (keyCode) {
            case KeyEvent.KEYCODE_ENTER:
            case KeyEvent.KEYCODE_NUMPAD_ENTER:
            case KeyEvent.KEYCODE_DPAD_CENTER:
            case KeyEvent.KEYCODE_BUTTON_A:
                return 0x0D;
            case KeyEvent.KEYCODE_ESCAPE:
            case KeyEvent.KEYCODE_BUTTON_B:
                return 0x1B;
            case KeyEvent.KEYCODE_SPACE:
                return 0x20;
            case KeyEvent.KEYCODE_TAB:
                return 0x09;
            case KeyEvent.KEYCODE_DEL:
                return 0x08;
            case KeyEvent.KEYCODE_SHIFT_LEFT:
            case KeyEvent.KEYCODE_SHIFT_RIGHT:
                return 0x10;
            case KeyEvent.KEYCODE_CTRL_LEFT:
            case KeyEvent.KEYCODE_CTRL_RIGHT:
            case KeyEvent.KEYCODE_BUTTON_R1:
                return 0x11;
            case KeyEvent.KEYCODE_PAGE_UP:
                return 0x21;
            case KeyEvent.KEYCODE_PAGE_DOWN:
                return 0x22;
            case KeyEvent.KEYCODE_DPAD_LEFT:
                return 0x25;
            case KeyEvent.KEYCODE_DPAD_UP:
                return 0x26;
            case KeyEvent.KEYCODE_DPAD_RIGHT:
                return 0x27;
            case KeyEvent.KEYCODE_DPAD_DOWN:
                return 0x28;
            default:
                if (keyCode >= KeyEvent.KEYCODE_F1 && keyCode <= KeyEvent.KEYCODE_F12) {
                    return 0x70 + keyCode - KeyEvent.KEYCODE_F1;
                }
                if (keyCode >= KeyEvent.KEYCODE_A && keyCode <= KeyEvent.KEYCODE_Z) {
                    return 'A' + keyCode - KeyEvent.KEYCODE_A;
                }
                if (keyCode >= KeyEvent.KEYCODE_0 && keyCode <= KeyEvent.KEYCODE_9) {
                    return '0' + keyCode - KeyEvent.KEYCODE_0;
                }
                return 0;
        }
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        int vk = virtualKey(keyCode);
        if (vk == 0) {
            return super.onKeyDown(keyCode, event);
        }
        if (event.getRepeatCount() == 0) {
            input(NativeEngine.INPUT_KEY_DOWN, 0, 0, vk, 0);
        }
        return true;
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        int vk = virtualKey(keyCode);
        if (vk == 0) {
            return super.onKeyUp(keyCode, event);
        }
        input(NativeEngine.INPUT_KEY_UP, 0, 0, vk, 0);
        return true;
    }
}
