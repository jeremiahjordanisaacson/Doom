package doom.util;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import com.doom.android.DoomClient;
import com.doom.android.DoomClient.eNavMethod;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Build;
import android.os.Environment;
import android.util.Log;
import android.view.KeyEvent;

/**
 * Doom utility functions.
 *
 * Original Author: Savant (2010)
 * Updated: 2026 - Scoped storage support for Android 10+
 */
@SuppressWarnings("unused")
public class DoomTools {
    static final String TAG = "DoomTools";

    // Base server URL (updated to HTTPS)
    public static final String URL_BASE = "https://playerx.sf.net/";

    // Url prefix that has all Doom files: WADs, Sound + JNI lib
    public static final String DOWNLOAD_BASE = URL_BASE + "gwad/";

    // Url prefix that has all Sounds
    public static final String SOUND_BASE = DOWNLOAD_BASE + "sound/";

    // Game files we can handle
    public static final String[] DOOM_WADS = { "doom1.wad", "plutonia.wad", "tnt.wad", "doom.wad", "doom2.wad" };

    // SoundTrack
    private static final String SOUND_TRACK = "sound.zip";

    // Doom lib name (loaded via System.loadLibrary)
    public static final String DOOM_LIB = "doom_jni";

    // These are required for the game to run
    public static final String REQUIRED_DOOM_WAD = "prboom.wad";

    /*
     * ASCII key symbols
     */
    public static final int KEY_RIGHTARROW = 0xae;
    public static final int KEY_LEFTARROW = 0xac;
    public static final int KEY_UPARROW = 0xad;
    public static final int KEY_DOWNARROW = 0xaf;
    public static final int KEY_ESCAPE = 27;
    public static final int KEY_ENTER = 13;
    public static final int KEY_TAB = 9;

    public static final int KEY_BACKSPACE = 127;
    public static final int KEY_PAUSE = 0xff;

    public static final int KEY_EQUALS = 0x3d;
    public static final int KEY_MINUS = 0x2d;

    public static final int KEY_RSHIFT = (0x80 + 0x36);
    public static final int KEY_RCTRL = (0x80 + 0x1d);
    public static final int KEY_RALT = (0x80 + 0x38);

    public static final int KEY_LALT = KEY_RALT;
    public static final int KEY_SPACE = 32;
    public static final int KEY_COMMA = 44;
    public static final int KEY_PERIOD = 46;

    /**
     * Get the doom folder path - uses app-specific external storage for Android 10+
     * This ensures compatibility with scoped storage while still allowing
     * file access without special permissions on newer Android versions.
     */
    public static String getDoomFolder(Context ctx) {
        File doomDir;

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
            // Android 10+ - use app-specific external files directory
            // This doesn't require storage permissions
            doomDir = new File(ctx.getExternalFilesDir(null), "doom");
        } else {
            // Older Android - use traditional external storage
            doomDir = new File(Environment.getExternalStorageDirectory(), "doom");
        }

        // Create directory if it doesn't exist
        if (!doomDir.exists()) {
            doomDir.mkdirs();
        }

        return doomDir.getAbsolutePath() + File.separator;
    }

    /**
     * Get the sound folder for the game
     */
    public static File getSoundFolder(Context ctx) {
        return new File(getDoomFolder(ctx) + "sound");
    }

    /**
     * Check if sound files are present
     */
    public static boolean hasSound(Context ctx) {
        File soundFolder = getSoundFolder(ctx);
        Log.d(TAG, "Sound folder: " + soundFolder.getAbsolutePath());
        return soundFolder.exists();
    }

    /**
     * Convert an android key to a Doom ASCII
     */
    static public int keyCodeToKeySym(int key) {
        switch (key) {
            case KeyEvent.KEYCODE_DPAD_LEFT:
                return KEY_LEFTARROW;

            case KeyEvent.KEYCODE_DPAD_RIGHT:
                return KEY_RIGHTARROW;

            case KeyEvent.KEYCODE_DPAD_UP:
                return KEY_UPARROW;

            case KeyEvent.KEYCODE_DPAD_DOWN:
                return KEY_DOWNARROW;

            case KeyEvent.KEYCODE_SHIFT_RIGHT:
            case KeyEvent.KEYCODE_SHIFT_LEFT:
                return KEY_RSHIFT;

            case KeyEvent.KEYCODE_ALT_LEFT:
                return KEY_RALT;

            case 23: // DPAD center
            case KeyEvent.KEYCODE_ENTER:
                return KEY_ENTER;

            case KeyEvent.KEYCODE_SPACE:
                return KEY_SPACE;

            case 4: // ESC / Back button
                return KEY_ESCAPE;

            case KeyEvent.KEYCODE_ALT_RIGHT:
            case KeyEvent.KEYCODE_TAB:
                return KEY_TAB;

            case KeyEvent.KEYCODE_COMMA:
                return KEY_COMMA;

            case KeyEvent.KEYCODE_PERIOD:
                return KEY_PERIOD;

            case KeyEvent.KEYCODE_DEL:
                return KEY_BACKSPACE;

            default:
                // Nav 1AQW
                if (key == KeyEvent.KEYCODE_1) {
                    key = (DoomClient.mNavMethod == eNavMethod.KBD) ? KEY_UPARROW : key + 41;
                } else if (key == KeyEvent.KEYCODE_A) {
                    key = (DoomClient.mNavMethod == eNavMethod.KBD) ? KEY_DOWNARROW : key + 68;
                } else if (key == KeyEvent.KEYCODE_Q) {
                    key = (DoomClient.mNavMethod == eNavMethod.KBD) ? KEY_LEFTARROW : key + 68;
                } else if (key == KeyEvent.KEYCODE_W) {
                    key = (DoomClient.mNavMethod == eNavMethod.KBD) ? KEY_RIGHTARROW : key + 68;
                }
                // A..Z
                else if (key >= 29 && key <= 54) {
                    key += 68;
                }
                // 0..9
                else if (key >= 7 && key <= 16) {
                    key += 41;
                } else {
                    // Fire
                    key = KEY_RCTRL;
                }
                break;
        }
        return key;
    }

    /**
     * Check if storage is available and writable
     */
    static public boolean hasStorage(Context ctx) {
        try {
            File doomFolder = new File(getDoomFolder(ctx));

            // Does doom folder exist?
            if (doomFolder.exists())
                return true;

            // Can we create it?
            return doomFolder.mkdirs();
        } catch (Exception e) {
            Log.e(TAG, "Storage check failed: " + e.toString());
            return false;
        }
    }

    /**
     * Ping the download server
     */
    static public boolean pingServer() {
        try {
            WebDownload wd = new WebDownload(URL_BASE);
            wd.doGet();
            int rc = wd.getResponseCode();
            Log.d(TAG, "PingServer Response:" + rc);
            return rc == 200;
        } catch (Exception e) {
            Log.e(TAG, "PingServer: " + e.toString());
            return false;
        }
    }

    /**
     * Check if WAD file exists
     */
    static public boolean wadExists(Context ctx, String wadName) {
        final String path = getDoomFolder(ctx) + wadName;
        return new File(path).exists();
    }

    /**
     * Unzip utility
     */
    public static void unzip(InputStream is, File dest) throws IOException {
        if (!dest.isDirectory())
            throw new IOException("Invalid Unzip destination " + dest);

        ZipInputStream zip = new ZipInputStream(is);
        ZipEntry ze;

        while ((ze = zip.getNextEntry()) != null) {
            final String path = dest.getAbsolutePath()
                    + File.separator + ze.getName();

            FileOutputStream fout = new FileOutputStream(path);
            byte[] bytes = new byte[1024];

            for (int c = zip.read(bytes); c != -1; c = zip.read(bytes)) {
                fout.write(bytes, 0, c);
            }
            zip.closeEntry();
            fout.close();
        }
    }

    /**
     * Write to a stream
     */
    public static void writeToStream(InputStream in, OutputStream out) throws IOException {
        byte[] bytes = new byte[2048];

        for (int c = in.read(bytes); c != -1; c = in.read(bytes)) {
            out.write(bytes, 0, c);
        }
        in.close();
        out.close();
    }

    /**
     * Check for storage access
     */
    public static boolean checkStorage(Context ctx) {
        boolean storage = hasStorage(ctx);

        if (!storage) {
            DialogTool.MessageBox(ctx, "Storage Error",
                    "Unable to access storage for game files. Please grant storage permissions.");
            return false;
        }
        return true;
    }

    /**
     * Make sure you have a web connection
     */
    public static boolean checkServer(Context ctx) {
        boolean alive = pingServer();

        if (!alive) {
            DialogTool.MessageBox(ctx, "Server Ping Failed", "Make sure you have an internet connection.");
            return false;
        }
        return true;
    }

    /**
     * Clean old JNI libs & other files.
     */
    public static void cleanUp(final Context ctx, final int wadIdx) {
        AlertDialog d = DialogTool.createAlertDialog(ctx, "Clean Up?",
                "This will remove game files from storage. Use this option if you are experiencing problems.");

        d.setButton(DialogInterface.BUTTON_POSITIVE, "Yes", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int whichButton) {
                // remove JNI lib
                Log.d(TAG, "Removing " + DOOM_LIB);

                File f = ctx.getFileStreamPath(DOOM_LIB);
                if (f.exists())
                    f.delete();

                // clean sounds
                deleteSounds(ctx);

                // game files
                deleteWads(ctx);
            }
        });
        d.setButton(DialogInterface.BUTTON_NEGATIVE, "No", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int whichButton) {
                dialog.dismiss();
            }
        });
        d.show();
    }

    /**
     * Clean sounds
     */
    private static void deleteSounds(Context ctx) {
        File folder = getSoundFolder(ctx);

        if (!folder.exists()) {
            Log.e(TAG, "Error: Sound folder " + folder + " not found.");
            return;
        }

        File[] files = folder.listFiles();
        if (files != null) {
            for (File file : files) {
                if (file.exists())
                    file.delete();
            }
        }
        if (folder.exists())
            folder.delete();
    }

    /**
     * Cleanup game files
     */
    private static void deleteWads(Context ctx) {
        for (String wadName : DOOM_WADS) {
            final File f = new File(getDoomFolder(ctx) + wadName);
            if (f.exists())
                f.delete();
        }
    }

    /**
     * Install soundtrack from assets
     */
    public static void installSoundTrack(Context ctx, File dir) throws IOException {
        Log.d(TAG, "Installing sound track " + SOUND_TRACK + " in " + dir);
        unzip(ctx.getAssets().open(SOUND_TRACK), dir);
    }

    /**
     * Validate server IP for multiplayer
     */
    public static boolean validateServerIP(String serverPort) {
        return serverPort != null && serverPort.length() > 0;
    }
}
