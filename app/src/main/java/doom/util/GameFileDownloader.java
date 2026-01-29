package doom.util;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import com.doom.android.DoomClient;

import android.app.ProgressDialog;
import android.content.Context;
import android.util.Log;

/**
 * Game file downloader utility.
 *
 * Original Author: Savant (2010)
 * Updated: 2026 - Scoped storage support
 */
public class GameFileDownloader {
    public static final String TAG = "GameFileDownloader";

    private ProgressDialog mProgressDialog;

    /**
     * Download game files
     * 1. prboom.wad.gz (gzipped) -> doom folder
     * 2. game wad : doom1.wad.gz, plutonia.wad or tnt.wad (gzipped) -> doom folder
     * 3. sound track: soundtrack.zip (zipped) -> doom/sound folder
     */
    public void downloadGameFiles(final Context ctx, final int wadIdx, final boolean force) {
        new Thread(new Runnable() {
            public void run() {
                Log.d(TAG, "Calling doDownload with wad: " + wadIdx + " force:" + force);
                boolean ok = doDownload(ctx, wadIdx, force);

                if (ok) {
                    // ready to go!
                    DialogTool.Toast(DoomClient.mHandler, ctx, "Install Completed. Tap Menu > Start");
                }
            }
        }).start();

        // Show progress
        mProgressDialog = new ProgressDialog(ctx);
        mProgressDialog.setMessage("Downloading files to storage (required once)."
                + " This may take some time depending on your connection."
                + " Please wait and do not cancel!");

        mProgressDialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
        mProgressDialog.show();
    }

    /*
     * Fetch file
     */
    private boolean doDownload(Context ctx, int wadIdx, boolean force) {
        boolean ok = false;
        String doomFolder = DoomTools.getDoomFolder(ctx);

        try {
            // prboom.wad (required)
            downloadFile(DoomTools.DOWNLOAD_BASE + DoomTools.REQUIRED_DOOM_WAD + ".gz",
                    new File(doomFolder + DoomTools.REQUIRED_DOOM_WAD),
                    "gzip", null, force);

            // game wad (required)
            downloadFile(DoomTools.DOWNLOAD_BASE + DoomTools.DOOM_WADS[wadIdx] + ".gz",
                    new File(doomFolder + DoomTools.DOOM_WADS[wadIdx]),
                    "gzip", null, force);

            if (DoomTools.hasSound(ctx)) {
                Log.d(TAG, "Sound folder " + DoomTools.getSoundFolder(ctx) + " already exists!");
                return true;
            }

            ok = true;

            // Install soundtrack from assets
            File folder = DoomTools.getSoundFolder(ctx);

            if (!folder.exists() && !folder.mkdirs())
                throw new IOException("Unable to create sound folder " + folder);

            DoomTools.installSoundTrack(ctx, folder);

        } catch (Exception e) {
            if (ok) {
                DialogTool.PostMessageBox(ctx, "Soundtrack install failed: " + e.getMessage());
            } else {
                DialogTool.PostMessageBox(ctx, e.toString());
            }
        } finally {
            mProgressDialog.dismiss();
        }
        return ok;
    }

    /**
     * Download a file
     *
     * @param url    URL
     * @param dest   File destination
     * @param type   one of gzip, zip
     * @param folder destination folder (File)
     * @param force  force download?
     */
    public void downloadFile(String url, File dest, String type, File folder, boolean force) throws Exception {
        Log.d(TAG, "Download " + url + " -> " + dest + " type: " + type + " folder=" + folder + " force:" + force);

        if (!dest.exists() || force) {
            if (force)
                Log.d(TAG, "Forcing download!");

            WebDownload wd = new WebDownload(url);
            wd.doGet(new FileOutputStream(dest), type.equalsIgnoreCase("gzip"));

            // If ZIP file unzip into folder
            if (type.equalsIgnoreCase("zip")) {
                if (folder == null)
                    throw new Exception("Invalid destination folder for ZIP " + dest);

                if (!folder.exists() && !folder.mkdirs())
                    throw new IOException("Unable to create local folder " + folder);

                DoomTools.unzip(new FileInputStream(dest), folder);

                // cleanup
                dest.delete();
            }
        } else {
            Log.d(TAG, "Not fetching " + dest + " already exists.");
        }
    }
}
