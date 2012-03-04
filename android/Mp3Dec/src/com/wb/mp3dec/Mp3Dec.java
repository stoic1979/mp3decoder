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

public class Mp3Dec extends Activity
{
    /** Called when the activity is first created. */
    @Override
        public void onCreate(Bundle savedInstanceState)
        {
            super.onCreate(savedInstanceState);
            //setContentView(R.layout.main);
            TextView  tv = new TextView(this);
            tv.setText(Version());
            setContentView(tv);
        }

    public native String  Version();

    static {
        System.loadLibrary("wblame");
    }
}
