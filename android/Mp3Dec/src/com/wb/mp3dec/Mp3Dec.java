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
import java.io.InputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.FileOutputStream;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.view.View;
import android.view.View.OnClickListener;
import android.os.AsyncTask;
import android.app.ProgressDialog;
import android.content.Context;

public class Mp3Dec extends Activity  implements OnClickListener {

    private EditText etMp3Path;
    private EditText etWavPath;
    private Button   btnDecode;
    private TextView tvAbout;
    private static final String TAG = "[Mp3DecMAIN]";
    private String abspath = "";

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

            tvAbout = (TextView) findViewById(R.id.tvAbout);
            tvAbout.setText(Version());

            InputStream inp = this.getResources().openRawResource(R.raw.beep1);
            String fdir[] = this.fileList() ;
            Log.v(TAG, "files count:  " + fdir.length);
            for(int i=0; i<fdir.length; i++) {
                Log.v(TAG, "file " + (i+1) + " :" + fdir[i]);
            }
            abspath = this. getFilesDir(). getAbsolutePath();
            copyFile();
        }

    public native String  Version();
    public native void    Decode(String srcpath, String despath);

    static {
        System.loadLibrary("wblame");
    }

    private boolean copyFile() {
        Log.v(TAG, "ABS Path:  " + abspath);
        File file = new File(abspath + "/beep1.mp3");

        try {
            // Very simple code to copy a mp3 from res/raw/ to files dir
            InputStream is = getResources().openRawResource(R.raw.beep1);
            OutputStream os = new FileOutputStream(file);
            byte[] data = new byte[is.available()];
            is.read(data);
            os.write(data);
            is.close();
            os.close();
            Log.v(TAG, "============ file copied =================");
            return true;
        } catch (IOException e) {
            Log.w("ExternalStorage", "Error writing " + file, e);
        }
        return false;
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
        private ProgressDialog  dlgProgress;
        private File            mp3path;
        private File            wavpath;

        public AsyncDecode() {
            dlgProgress = new ProgressDialog(Mp3Dec.this);
            dlgProgress.setCancelable(false);
        }

        @Override
            protected void onPreExecute() {
                dlgProgress.setMessage("Decoding, Please wait...");
                dlgProgress.show();
            }

        @Override
            protected Void doInBackground(String... params) {
                mp3path = new File(params[0]);
                wavpath = new File(params[1]);

                try {
                    //Thread.sleep(2000);
                    Decode(params[0], params[1]);
                    //Decode(abspath + "/beep1.mp3", abspath + "/beep1.wav");
                } catch (Exception e) {
                    Log.v(TAG, "AsyncDecode err: " + e);
                }
                return null;
            }

        @Override
            protected void onPostExecute(Void unused) {
                dlgProgress.dismiss();
            }
    }/*AsyncDecode*/
}/*Mp3Dec*/
