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

public class Mp3Dec extends Activity
{
    private static final String TAG = "[Mp3DecMAIN]";
    /** Called when the activity is first created. */
    @Override
        public void onCreate(Bundle savedInstanceState)
        {
            super.onCreate(savedInstanceState);
            //setContentView(R.layout.main);
            TextView  tv = new TextView(this);
            tv.setText(Version());
            File file = Environment.getExternalStorageDirectory();
            Log.v(TAG, "data dir: " + file.getAbsolutePath());
            //Decode("res/breaking-voices.mp3", "res/breaking-voices.wav");
            setContentView(tv);
        }

    public native String  Version();
    public native void    Decode(String srcpath, String despath);

    static {
        System.loadLibrary("wblame");
    }
}
