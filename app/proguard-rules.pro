# Doom Android ProGuard Rules

# Keep native methods
-keepclasseswithmembernames class * {
    native <methods>;
}

# Keep the JNI classes
-keep class doom.jni.** { *; }

# Keep the main application classes
-keep class com.doom.android.** { *; }
