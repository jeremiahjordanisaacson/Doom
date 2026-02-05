package doom.audio;

import android.content.Context;
import android.media.MediaPlayer;
import android.net.Uri;
import android.util.Log;

/**
 * AudioClip - Wrapper for MediaPlayer.
 *
 * Original Author: Savant (2010)
 * Updated: 2026 - Improved error handling
 */
public class AudioClip {
    static final String TAG = "AudioClip";

    private MediaPlayer mPlayer;
    private String name;

    private boolean mPlaying = false;
    private boolean mLoop = false;

    public AudioClip(Context ctx, int resID) {
        name = ctx.getResources().getResourceName(resID);

        try {
            mPlayer = MediaPlayer.create(ctx, resID);
            if (mPlayer != null) {
                mPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
                    public void onCompletion(MediaPlayer mp) {
                        mPlaying = false;
                        if (mLoop) {
                            mp.start();
                        }
                    }
                });
            }
        } catch (Exception e) {
            Log.e(TAG, "Error creating AudioClip from resource: " + e.getMessage());
        }
    }

    public AudioClip(Context ctx, Uri uri) {
        name = uri.toString();

        try {
            mPlayer = MediaPlayer.create(ctx, uri);
            if (mPlayer != null) {
                mPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
                    public void onCompletion(MediaPlayer mp) {
                        mPlaying = false;
                        if (mLoop) {
                            mp.start();
                        }
                    }
                });
            }
        } catch (Exception e) {
            Log.e(TAG, "Error creating AudioClip from URI: " + e.getMessage());
        }
    }

    public synchronized void play() {
        if (mPlaying)
            return;

        if (mPlayer != null) {
            try {
                mPlaying = true;
                mPlayer.start();
            } catch (Exception e) {
                Log.e(TAG, "Error playing: " + e.getMessage());
                mPlaying = false;
            }
        }
    }

    public synchronized void play(int vol) {
        if (mPlaying)
            return;

        if (mPlayer != null) {
            try {
                mPlaying = true;
                mPlayer.setVolume((float) Math.log10(vol), (float) Math.log(vol));
                mPlayer.start();
            } catch (Exception e) {
                Log.e(TAG, "Error playing with volume: " + e.getMessage());
                mPlaying = false;
            }
        }
    }

    public synchronized void stop() {
        try {
            mLoop = false;
            if (mPlaying) {
                mPlaying = false;
                if (mPlayer != null) {
                    mPlayer.pause();
                }
            }
        } catch (Exception e) {
            Log.e(TAG, "AudioClip::stop " + name + " " + e.toString());
        }
    }

    public synchronized void loop() {
        if (mPlayer != null) {
            try {
                mLoop = true;
                mPlaying = true;
                mPlayer.start();
            } catch (Exception e) {
                Log.e(TAG, "Error looping: " + e.getMessage());
            }
        }
    }

    public void release() {
        if (mPlayer != null) {
            try {
                mPlayer.release();
            } catch (Exception e) {
                Log.e(TAG, "Error releasing: " + e.getMessage());
            }
            mPlayer = null;
        }
    }

    public String getName() {
        return name;
    }

    /**
     * Set volume
     *
     * @param vol (1-100)
     */
    public void setVolume(int vol) {
        if (mPlayer != null) {
            try {
                mPlayer.setVolume((float) Math.log10(vol), (float) Math.log10(vol));
            } catch (Exception e) {
                Log.e(TAG, "Error setting volume: " + e.getMessage());
            }
        }
    }
}
