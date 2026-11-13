plugins {
    id("com.android.application")
}

android {
    namespace = "com.xmoezzz.ethornell"
    compileSdk = 36

    defaultConfig {
        applicationId = "com.xmoezzz.ethornell"
        // The host ABI targets API 26 (platform/android/include/ABI.md).
        minSdk = 26
        targetSdk = 35
        versionCode = 1
        versionName = "0.1.0"
        ndk {
            abiFilters += listOf("arm64-v8a", "x86_64")
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            // CI signs the release APK with the fixed key via platform/android/sign.sh.
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }

    packaging {
        jniLibs {
            // libethornell_mobile.so is page-aligned for 16 KB pages; keep it
            // uncompressed so it loads straight from the APK.
            useLegacyPackaging = false
        }
    }
}

dependencies {
    implementation("androidx.documentfile:documentfile:1.0.1")
}
