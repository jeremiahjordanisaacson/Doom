package doom.audio;

import java.io.File;
import java.util.HashMap;

import android.content.Context;
import android.net.Uri;
import android.os.Build;
import android.util.Log;

import androidx.core.content.FileProvider;

import doom.util.DoomTools;

/**
 * Audio manager. Caches sounds for performance.
 *
 * Original Author: Savant (2010)
 * Updated: 2026 - FileProvider support for Android 7.0+
 */
public class AudioManager {
    static final String TAG = "AudioMgr";

    static private AudioManager am;

    // Game sound (WAVs)
    private volatile HashMap<String, AudioClip> mSounds = new HashMap<String, AudioClip>();

    private int MAX_CLIPS = 15;
    private int mClipCount = 0;
    private Context mContext;

    // BG music
    private AudioClip music;

    /**
     * get Instance
     */
    static public AudioManager getInstance(Context ctx, int wadIdx) {
        if (am == null)
            return new AudioManager(ctx, wadIdx);
        return am;
    }

    private AudioManager(Context ctx, int wadIdx) {
        mContext = ctx;
        preloadSounds(ctx, wadIdx);
    }

    /**
     * Get a content URI for a file using FileProvider (Android 7.0+) or file URI
     * (older)
     */
    private Uri getFileUri(Context ctx, File file) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
            // Use FileProvider for Android 7.0+
            try {
                return FileProvider.getUriForFile(ctx,
                        ctx.getPackageName() + ".fileprovider",
                        file);
            } catch (IllegalArgumentException e) {
                Log.e(TAG, "FileProvider error: " + e.getMessage());
                // Fall back to file URI - may not work on all devices
                return Uri.fromFile(file);
            }
        } else {
            // Use direct file URI for older Android versions
            return Uri.fromFile(file);
        }
    }

    /**
     * Start a sound by name & volume
     *
     * @param name example "pistol" when firing the gun
     * @param vol
     */
    public synchronized void startSound(String name, int vol) {
        // The sound key as stored in the FS -> DS[NAME-UCASE].wav
        String key = "DS" + name.toUpperCase() + ".wav";

        if (mSounds.containsKey(key)) {
            mSounds.get(key).play(vol);
        } else {
            // load clip from disk
            File folder = DoomTools.getSoundFolder(mContext);
            File sound = new File(folder.getAbsolutePath() + File.separator + key);

            if (!sound.exists()) {
                return;
            }

            // If the sound table is full remove a random entry
            if (mClipCount > MAX_CLIPS) {
                // Remove a last key
                int idx = mSounds.size() - 1;

                String k = (String) mSounds.keySet().toArray()[idx];
                AudioClip clip = mSounds.remove(k);
                clip.release();
                clip = null;
                mClipCount--;
            }

            AudioClip clip = new AudioClip(mContext, getFileUri(mContext, sound));
            clip.play(vol);

            mSounds.put(key, clip);
            mClipCount++;
        }
    }

    /**
     * PreLoad the most used sounds into a hash map
     */
    public void preloadSounds(Context ctx, int wadIdx) {
        // These are some common sound keys pre-loaded for speed - Jeremiah
        String[] names = new String[] { "DSPISTOL.wav" // pistol
                , "DSDOROPN.wav", "DSDORCLS.wav" // doors open/close
                , "DSPSTOP.wav", "DSSWTCHN.wav", "DSSWTCHX.wav", "DSITEMUP.wav", "DSPOSACT.wav", "DSPOPAIN.wav",
                "DSPODTH1.wav", "DSSHOTGN.wav" };

        // Sound folder
        File folder = DoomTools.getSoundFolder(ctx);

        if (!folder.exists()) {
            Log.e(TAG, "Error: Sound folder " + folder + " not found.");
            return;
        }

        // WAVs
        File[] files = new File[names.length];

        for (int i = 0; i < files.length; i++) {
            files[i] = new File(folder + File.separator + names[i]);

            if (files[i].exists()) {
                mSounds.put(files[i].getName(), new AudioClip(ctx, getFileUri(ctx, files[i])));
            } else
                System.err.println("AudioMgr:" + files[i] + " not found");
        }
    }

    /**
     * Start background music
     *
     * @param ctx
     * @param key music key (e.g intro, e1m1)
     */
    public void startMusic(Context ctx, String key, int loop) {
        // Sound folder
        File folder = DoomTools.getSoundFolder(ctx);
        File sound = new File(folder + File.separator + "d1" + key + ".ogg");

        if (!sound.exists()) {
            Log.e(TAG, "Unable to find music " + sound);
            return;
        }

        if (music != null) {
            music.stop();
            music.release();
        }

        Log.d(TAG, "Starting music " + sound + " loop=" + loop);
        music = new AudioClip(ctx, getFileUri(ctx, sound));

        music.setVolume(100);
        music.play();
    }

    /**
     * Stop background music
     */
    public void stopMusic(String key) {
        // Sound folder
        File folder = DoomTools.getSoundFolder(mContext);
        Uri sound = getFileUri(mContext, new File(folder + File.separator + "d1" + key + ".ogg"));

        if (music != null) {
            if (!sound.equals(Uri.parse(music.getName()))) {
                Log.w(TAG, "Stop music uri " + sound + " different than " + music.getName());
            }
            music.stop();
            music.release();
            music = null;
        }
    }

    public void setMusicVolume(int vol) {
        if (music != null)
            music.setVolume(vol);
        else
            Log.e(TAG, "setMusicVolume " + vol + " called with NULL music player");
    }
}
