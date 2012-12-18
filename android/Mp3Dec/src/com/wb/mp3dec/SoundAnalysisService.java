/* *************************************************************
*
* This service analyzes the given mp3 file and announces the 
* results of sound analysis - jitter, weak note etc.
*
* The backend C engine for processing the audio data is invoked.
*
***************************************************************/

package com.wb.mp3dec;

import java.util.List;
import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;
import android.widget.Toast;
import android.content.Context;
import android.os.Bundle;
import java.util.Locale;


public class SoundAnalysisService extends Service implements Runnable {
    private static final String TAG = "SoundAnalysisService";

    public static final String SOUND_ANALYSIS    = "com.wb.mp3dec.SoundAnalysisService.action.SOUND_ANALYSIS";
    public static final String ERROR             = "com.wb.mp3dec.SoundAnalysisService.error";
    public static final String JITTER            = "com.wb.mp3dec.SoundAnalysisService.jitter";
    public static final String WEAK_NOTE         = "com.wb.mp3dec.SoundAnalysisService.weak_note";
    public static final String EXCESS_NOTE       = "com.wb.mp3dec.SoundAnalysisService.excess_note";
    public static final String PHI_RELS_CNT      = "com.wb.mp3dec.SoundAnalysisService.phi_rels_cnt";
    public static final String OCT_RELS_CNT      = "com.wb.mp3dec.SoundAnalysisService.oct_rels_cnt";
    public static final String FOURTH_RELS_CNT   = "com.wb.mp3dec.SoundAnalysisService.fourth_rels_cnt";
    public static final String FIFTH_RELS_CNT    = "com.wb.mp3dec.SoundAnalysisService.fifth_rels_cnt";

    private Thread thread;

    String src;
    String des;

    @Override
        public IBinder onBind(Intent intent) {
            return null;
        }

    @Override
        public void onCreate() {
        }

    @Override
        public void onDestroy() {
            Toast.makeText(this, "Sound Analysis Service Stopped", Toast.LENGTH_LONG).show();
            Log.d(TAG, "onDestroy");
        }

    @Override
        public void onStart(Intent intent, int startid) {
            src  = intent.getStringExtra("src");
            des  = intent.getStringExtra("des");


            thread = new Thread(this);

            thread.start();
            /* announce results */
            //announceSoundAnalysisResults(2.456);
        }

    public void run() {
        Log.d(TAG, "-- decoding, src=" + src + ", des="  + des);
        /* decode and analyze the audio */
        //FIXME todo
        Decode(src, des);
    }

    /** 
     * Announce to the main activity that we have results of sound analysis
     **/
    private void announceSoundAnalysisResults(int error, double jitter, double weakNote, double excessNote, 
            double phiRelsCnt, double octRelsCnt, double fourthRelsCnt, double fifthRelsCnt) {

        Intent intent = new Intent(SOUND_ANALYSIS);
        intent. putExtra(ERROR,           error);
        intent. putExtra(JITTER,          jitter);
        intent. putExtra(WEAK_NOTE,       weakNote);
        intent. putExtra(EXCESS_NOTE,     excessNote);
        intent. putExtra(PHI_RELS_CNT,    phiRelsCnt);
        intent. putExtra(OCT_RELS_CNT,    octRelsCnt);
        intent. putExtra(FOURTH_RELS_CNT, fourthRelsCnt);
        intent. putExtra(FIFTH_RELS_CNT,  fifthRelsCnt);

        sendBroadcast(intent);
    }

    /******************************************************************
       Callback from C code  
     *****************************************************************/
    void gotSoundAnalysisResults(int error, double jitter, double weakNote, double excessNote, 
            double phiRelsCnt, double octRelsCnt, double fourthRelsCnt, double fifthRelsCnt) {
        announceSoundAnalysisResults(error, jitter, weakNote, excessNote, phiRelsCnt, octRelsCnt, fourthRelsCnt, fifthRelsCnt);
    }
    public native void    Decode(String srcpath, String despath);
}/*SoundAnalysisService*/
