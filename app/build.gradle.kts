plugins {
    id("com.android.application")
}

android {
    namespace = "com.doom.android"
    compileSdk = 34

    defaultConfig {
        applicationId = "com.doom.android"
        minSdk = 24  // Android 7.0 Nougat - supports 97%+ of devices
        targetSdk = 34  // Android 14
        versionCode = 17
        versionName = "9.0-2026"

        ndk {
            // Currently only armeabi-v7a is available
            // For full 64-bit support, rebuild native libraries from native/prboom source
            // Supporting: ARM 32-bit devices and ARM 64-bit devices with 32-bit compatibility
            abiFilters += listOf("armeabi-v7a")
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }

    sourceSets {
        getByName("main") {
            java.srcDirs("src/main/java")
            res.srcDirs("src/main/res")
            jniLibs.srcDirs("src/main/jniLibs")
            manifest.srcFile("src/main/AndroidManifest.xml")
        }
    }

    // Native library packaging
    packaging {
        jniLibs {
            useLegacyPackaging = true
        }
    }

    lint {
        abortOnError = false
        checkReleaseBuilds = false
    }
}

dependencies {
    implementation("androidx.appcompat:appcompat:1.6.1")
    implementation("androidx.core:core:1.12.0")
    implementation("com.google.android.material:material:1.11.0")
}
