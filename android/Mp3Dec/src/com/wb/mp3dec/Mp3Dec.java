/* *************************************************************
*
* Mp3Decoder Application
* Main Activity for the Sound Analyzer Application
*
***************************************************************/

package com.wb.mp3dec;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;
import android.util.Log;
import android.os.Environment;
import java.io.File;
import android.widget.Button;
import android.widget.EditText;
import android.view.View;
import android.view.View.OnClickListener;
import android.os.AsyncTask;
import android.app.ProgressDialog;

public class Mp3Dec extends Activity  implements OnClickListener {

    private EditText etMp3Path;
    private EditText etWavPath;
    private Button   btnDecode;
    private static final String TAG = "[Mp3DecMAIN]";

    /** Called when the activity is first created. */
    @Override
        public void onCreate(Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);
            setContentView(R.layout.main);
            File file = Environment.getExternalStorageDirectory();
            Log.v(TAG, "data dir: " + file.getAbsolutePath());
            //Decode("res/breaking-voices.mp3", "res/breaking-voices.wav");

            etMp3Path = (EditText) findViewById(R.id.etMp3Path);
            etWavPath = (EditText) findViewById(R.id.etWavPath);

            btnDecode = (Button) findViewById(R.id.btnDecode);
            btnDecode.setOnClickListener(this);
        }

    public native String  Version();
    public native void    Decode(String srcpath, String despath);

    static {
        System.loadLibrary("wblame");
    }

    @Override
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.btnDecode:
                    String mp3path = etMp3Path. getText(). toString();
                    String wavpath = etWavPath. getText(). toString();
                    Log.v(TAG, "Decoding : " + mp3path + " --> " + wavpath); //+ file.getAbsolutePath());
                    String[] params = new String[] { mp3path, wavpath };
                    new AsyncDecode().execute(params);
                    break;
            }

        }
    private class AsyncDecode extends AsyncTask<String, Void, Void> {
        private File            mp3path;
        private File            wavpath;
        private ProgressDialog  spinner;
        private int             errorCode;

        public AsyncDecode() {
            spinner = new ProgressDialog(Mp3Dec.this);
            spinner.setCancelable(false);
            errorCode = 0;
        }

        @Override
            protected void onPreExecute() {
                spinner.setMessage("Decoding, Please wait...");
                spinner.show();
            }

        @Override
            protected Void doInBackground(String... params) {
                mp3path = new File(params[0]);
                wavpath = new File(params[1]);

                try {
                    Thread.sleep(2000);
                } catch (Exception e) {
                    Log.v(TAG, "AsyncDecode err: " + e);
                }
                return null;
            }

        @Override
            protected void onPostExecute(Void unused) {
                spinner.dismiss();
            }
    }/*AsyncDecode*/
}/*Mp3Dec*/
