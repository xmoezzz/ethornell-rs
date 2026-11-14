//! 1.687 1400F8240: BP-stack printf for string buffers. `%f` consumes
//! signed 16.16, `%c` retains the low byte through the native sentinel pass.
use crate::*;

impl Vm {
    pub(super) fn encode_buffer_text(&self, text: &str) -> Vec<u8> {
        if self.native_utf8_enabled() {
            text.as_bytes().to_vec()
        } else {
            crate::version_system::encode_cp932(text)
        }
    }

    pub(super) fn render_buffer_format(&mut self, format: &str) -> VmResult<Vec<u8>> {
        let mut output = Vec::new();
        let mut chars = format.chars().peekable();
        while let Some(ch) = chars.next() {
            if ch != '%' {
                let mut text = [0; 4];
                output.extend_from_slice(&self.encode_buffer_text(ch.encode_utf8(&mut text)));
            } else if chars.peek() == Some(&'%') {
                chars.next();
                output.push(b'%');
            } else {
                // The native scanner's 03FF600100000000h mask permits
                // space, '-', '.', digits; '+' and '#' are not flags here.
                let (mut left, mut space, mut zero) = (false, false, false);
                loop {
                    match chars.peek() {
                        Some('-') => left = true,
                        Some(' ') => space = true,
                        Some('0') => zero = true,
                        _ => break,
                    };
                    chars.next();
                }
                let number =
                    |chars: &mut std::iter::Peekable<std::str::Chars<'_>>| -> VmResult<usize> {
                        let mut number = 0usize;
                        while let Some(digit) = chars.peek().and_then(|c| c.to_digit(10)) {
                            number = number
                                .checked_mul(10)
                                .and_then(|n| n.checked_add(digit as usize))
                                .filter(|n| *n < 65536)
                                .ok_or_else(|| {
                                    VmError::Runtime("native format field limit".into())
                                })?;
                            chars.next();
                        }
                        Ok(number)
                    };
                let width = number(&mut chars)?;
                let precision = if chars.peek() == Some(&'.') {
                    chars.next();
                    Some(number(&mut chars)?)
                } else {
                    None
                };
                let spec = chars
                    .next()
                    .ok_or_else(|| VmError::Runtime("unterminated native buffer format".into()))?;
                let mut wide_length = None;
                let (prefix, mut value, numeric) = match spec {
                    'd' | 'x' | 'X' => {
                        let raw = self.pop_int()?;
                        let negative = spec == 'd' && raw < 0;
                        let mut digits = match spec {
                            'x' => format!("{:x}", raw as u32),
                            'X' => format!("{:X}", raw as u32),
                            _ => raw.unsigned_abs().to_string(),
                        };
                        if precision == Some(0) && raw == 0 {
                            digits.clear();
                        }
                        if let Some(precision) = precision {
                            if digits.len() < precision {
                                digits =
                                    format!("{}{}", "0".repeat(precision - digits.len()), digits);
                            }
                        }
                        let prefix = if negative {
                            "-"
                        } else if spec == 'd' && space {
                            " "
                        } else {
                            ""
                        };
                        (prefix.as_bytes().to_vec(), digits.into_bytes(), true)
                    }
                    'f' => {
                        let raw = self.pop_int()?;
                        let prefix = if raw < 0 {
                            "-"
                        } else if space {
                            " "
                        } else {
                            ""
                        };
                        let precision = precision.unwrap_or(6);
                        let digits =
                            format!("{:.*}", precision, f64::from(raw.unsigned_abs()) / 65536.0);
                        (prefix.as_bytes().to_vec(), digits.into_bytes(), true)
                    }
                    'c' => (Vec::new(), vec![self.pop_int()? as u8], false),
                    's' => {
                        let bytes = self.pop_native_string_bytes()?;
                        let text = crate::version_system::decode_auto(&bytes, 6);
                        let text = if let Some(limit) = precision {
                            let mut used = 0;
                            text.chars()
                                .take_while(|c| {
                                    used += c.len_utf16();
                                    used <= limit
                                })
                                .collect::<String>()
                        } else {
                            text
                        };
                        wide_length = Some(text.encode_utf16().count());
                        (Vec::new(), self.encode_buffer_text(&text), false)
                    }
                    _ => {
                        return Err(VmError::Runtime(format!(
                            "unsupported native buffer format %{spec}"
                        )));
                    }
                };
                let padding =
                    width.saturating_sub(prefix.len() + wide_length.unwrap_or(value.len()));
                if !left && !(zero && numeric && (precision.is_none() || spec == 'f')) {
                    output.extend(std::iter::repeat_n(b' ', padding));
                }
                output.extend(prefix);
                if !left && zero && numeric && (precision.is_none() || spec == 'f') {
                    output.extend(std::iter::repeat_n(b'0', padding));
                }
                output.append(&mut value);
                if left {
                    output.extend(std::iter::repeat_n(b' ', padding));
                }
            }
            if output.len() >= 65536 {
                return Err(VmError::Runtime("native formatted buffer limit".into()));
            }
        }
        if let Some(end) = output.iter().position(|b| *b == 0) {
            output.truncate(end);
        }
        Ok(output)
    }
}
