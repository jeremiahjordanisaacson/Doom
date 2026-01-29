/*******************************************************************************
 * Copyright (c) 2010 Savant - Jeremiah Isaacson & Justin Turney
 *
 * Contributors:
 *     Jeremiah Isaacson & Justin Turney - initial API and implementation
 *
 * Updated: 2026 - Modernized for all Android devices
 *******************************************************************************/
package doom.util;

import com.doom.android.DoomClient;
import com.doom.android.R;
import com.doom.android.DoomClient.eNavMethod;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.CheckBox;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

public class DialogTool {
    public static final String TAG = "DialogTool";

    /**
     * Load spinner
     */
    private static void loadSpinner(Context ctx, Spinner spinner, int resID, int idx) {
        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(ctx, resID,
                android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinner.setAdapter(adapter);
        spinner.setSelection(idx);
    }

    /**
     * Game options UI
     */
    public static void setLauncherDlgOptionsUI(final Context ctx, final View v, boolean multiPlayer) {
        // Load files spinner
        final Spinner sFiles = (Spinner) v.findViewById(R.id.s_files);
        loadSpinner(ctx, sFiles, R.array.LaunchFiles, 0);

        sFiles.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {

            public void onItemSelected(AdapterView<?> arg0, View arg1,
                    int idx, long arg3) {
                if (idx == 3) {
                    // other
                    TextView lbl = ((TextView) v.findViewById(R.id.tv_game));

                    // hide spinner
                    toggleView(sFiles);

                    // Show WAD text box
                    toggleView(v.findViewById(R.id.t_wad));

                    // Help lbl
                    lbl.setText(DoomTools.getDoomFolder(ctx));

                    // Hint
                    Toast(ctx, "Upload your game file(s) to " + DoomTools.getDoomFolder(ctx));
                }
            }

            public void onNothingSelected(AdapterView<?> arg0) {
            }
        });

        // mp?
        if (multiPlayer) {
            v.findViewById(R.id.lbl_mp).setVisibility(View.VISIBLE);
            v.findViewById(R.id.t_server).setVisibility(View.VISIBLE);
        } else {
            v.findViewById(R.id.lbl_mp).setVisibility(View.GONE);
            v.findViewById(R.id.t_server).setVisibility(View.GONE);
        }
    }

    /**
     * Download game file dlg
     */
    static public void showDownloadDialog(final Context ctx) {
        LayoutInflater factory = LayoutInflater.from(ctx);
        final View view = factory.inflate(R.layout.download, null);

        setDownloadDlgUI(ctx, view);

        AlertDialog dialog = new AlertDialog.Builder(ctx)
                .setIcon(R.drawable.icon)
                .setTitle("Download a Game File")
                .setView(view)
                .setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int whichButton) {
                        // game file
                        int wadIdx = ((Spinner) view.findViewById(R.id.s_files)).getSelectedItemPosition();

                        // force?
                        boolean force = ((CheckBox) view.findViewById(R.id.cb_force)).isChecked();

                        // Fetch
                        new GameFileDownloader().downloadGameFiles(ctx, wadIdx, force);
                    }
                })
                .setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int whichButton) {
                        dialog.dismiss();
                    }
                })
                .create();
        dialog.show();
    }

    /**
     * Select a nav method dialog
     */
    static public void showNavMethodDialog(final Context ctx) {
        final String[] mListItems = new String[] { "Kbd: 1AQW or D-Pad Shift=Run, Alt=Strafe",
                "Use The Touch Screen" };

        AlertDialog dialog = new AlertDialog.Builder(ctx)
                .setIcon(R.drawable.icon)
                .setTitle("Select Controls")
                .setItems(mListItems, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        final View v = ((Activity) ctx).findViewById(R.id.pan_ctls);
                        final View v1 = ((Activity) ctx).findViewById(R.id.other_ctls);

                        switch (which) {
                            case 0:
                                // KBD Hide ctls
                                v.setVisibility(View.GONE);
                                v1.setVisibility(View.GONE);

                                DoomClient.mNavMethod = eNavMethod.KBD;
                                break;

                            case 1:
                                // Touch Screen
                                DoomClient.mNavMethod = eNavMethod.PANEL;

                                // show arrows
                                v.setVisibility(View.VISIBLE);
                                v1.setVisibility(View.VISIBLE);
                                break;
                        }
                    }
                })
                .create();
        dialog.show();
    }

    /**
     * Download dlg data
     */
    private static void setDownloadDlgUI(Context ctx, View v) {
        loadSpinner(ctx, (Spinner) v.findViewById(R.id.s_files), R.array.DoomFiles, 0);
    }

    /**
     * Message box
     */
    public static void MessageBox(Context ctx, String title, String text) {
        AlertDialog d = createAlertDialog(ctx, title, text);

        d.setButton(DialogInterface.BUTTON_POSITIVE, "Dismiss", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int whichButton) {
            }
        });
        d.show();
    }

    /**
     * Create an alert dialog
     */
    static public AlertDialog createAlertDialog(Context ctx, String title, String message) {
        return new AlertDialog.Builder(ctx)
                .setIcon(R.drawable.icon)
                .setTitle(title)
                .setMessage(message)
                .create();
    }

    /**
     * Launch web browser
     */
    static public void launchBrowser(Context ctx, String url) {
        ctx.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse(url)));
    }

    /**
     * MessageBox
     */
    public static void MessageBox(Context ctx, String text) {
        MessageBox(ctx, ctx.getString(R.string.app_name), text);
    }

    /**
     * Messagebox
     */
    public static void PostMessageBox(final Context ctx, final String text) {
        DoomClient.mHandler.post(new Runnable() {
            public void run() {
                MessageBox(ctx, ctx.getString(R.string.app_name), text);
            }
        });
    }

    /**
     * Change a view's visibility
     */
    public static void toggleView(View v) {
        if (v.getVisibility() == View.VISIBLE)
            v.setVisibility(View.GONE);
        else
            v.setVisibility(View.VISIBLE);
    }

    /**
     * Message Box
     */
    public static void Toast(final Context ctx, final String text) {
        Toast.makeText(ctx, text, Toast.LENGTH_LONG).show();
    }

    public static void Toast(Handler handler, final Context ctx, final String text) {
        handler.post(new Runnable() {
            public void run() {
                Toast.makeText(ctx, text, Toast.LENGTH_LONG).show();
            }
        });
    }
}
