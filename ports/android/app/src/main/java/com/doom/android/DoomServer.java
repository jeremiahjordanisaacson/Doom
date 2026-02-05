package com.doom.android;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import doom.jni.ServerNatives;

/**
 * Doom Server Activity for multiplayer games.
 *
 * Original Author: Savant (2010)
 * Updated: 2026
 */
public class DoomServer extends Activity
        implements ServerNatives.EventListener {
    private static final String TAG = "DoomServer";

    private int mPort = 5030;
    private int mNumPlayers = 2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        startserver();
    }

    private void startserver() {
        final String[] argv = new String[] { "server",
                "-p", String.valueOf(mPort),
                "-N", String.valueOf(mNumPlayers)
        };

        Log.d(TAG, "Starting Doom server");

        new Thread(new Runnable() {
            public void run() {
                ServerNatives.ServerMain(argv);
            }
        }).start();
    }

    /**************************************************
     * C CALLBACKS
     **************************************************/
    public void OnFatalError(String text) {
    }

    public void OnMessage(String text, int level) {
    }

    public void OnQuit(int code) {
    }
}
