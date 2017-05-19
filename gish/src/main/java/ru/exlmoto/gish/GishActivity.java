/************************************************************************************
 ** The MIT License (MIT)
 **
 ** Copyright (c) 2017 EXL
 **
 ** Permission is hereby granted, free of charge, to any person obtaining a copy
 ** of this software and associated documentation files (the "Software"), to deal
 ** in the Software without restriction, including without limitation the rights
 ** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 ** copies of the Software, and to permit persons to whom the Software is
 ** furnished to do so, subject to the following conditions:
 **
 ** The above copyright notice and this permission notice shall be included in all
 ** copies or substantial portions of the Software.
 **
 ** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 ** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 ** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 ** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 ** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 ** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 ** SOFTWARE.
 ************************************************************************************/

package ru.exlmoto.gish;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Vibrator;
import android.util.Log;
import android.view.KeyEvent;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;

import org.libsdl.app.SDLActivity;

import ru.exlmoto.gish.GishLauncherActivity.GishSettings;

/**
 * Created by exl on 5/14/17.
 */

public class GishActivity extends SDLActivity {

    private static final String APP_TAG = "Gish_app";

    private static Activity m_GishActivity = null;
    private GishTouchControlsView gishTouchControlsView = null;

    private static Vibrator m_vibrator = null;

    // --- SDL Patch Functions
    public static void pressOrReleaseKey(int keyCode, boolean press) {
        if (press) {
            SDLActivity.onNativeKeyDown(keyCode);
        } else {
            SDLActivity.onNativeKeyUp(keyCode);
        }
    }

    public static boolean convertKeysFilter(int keyCode, boolean press) {
        switch (keyCode) {
            case KeyEvent.KEYCODE_DPAD_UP:
                pressOrReleaseKey(KeyEvent.KEYCODE_DPAD_UP, press);
                return true;
            case KeyEvent.KEYCODE_DPAD_DOWN:
                pressOrReleaseKey(KeyEvent.KEYCODE_DPAD_DOWN, press);
                return true;
            case KeyEvent.KEYCODE_DPAD_LEFT:
                pressOrReleaseKey(KeyEvent.KEYCODE_DPAD_LEFT, press);
                return true;
            case KeyEvent.KEYCODE_DPAD_RIGHT:
                pressOrReleaseKey(KeyEvent.KEYCODE_DPAD_RIGHT, press);
                return true;
            case KeyEvent.KEYCODE_DPAD_CENTER:
                pressOrReleaseKey(KeyEvent.KEYCODE_ENTER, press);
                return true;
            case KeyEvent.KEYCODE_BACK:
                pressOrReleaseKey(KeyEvent.KEYCODE_ESCAPE, press); // TODO: Check This on Motorola Droid 2 (Android 2.3.4)
                return true;
            default:
                return false;
        }
    }

    public static void toDebugLog(String message) {
        Log.d(APP_TAG, message);
    }

    @SuppressWarnings("deprecation")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        m_GishActivity = this;

        m_vibrator = (Vibrator)getSystemService(Context.VIBRATOR_SERVICE);

        if (GishSettings.touchControls == GishSettings.MODERN_TOUCH_CONTROLS) {
            gishTouchControlsView = new GishTouchControlsView(this);
            addContentView(gishTouchControlsView,
                    new LinearLayout.LayoutParams(
                            LinearLayout.LayoutParams.MATCH_PARENT,
                            LinearLayout.LayoutParams.MATCH_PARENT));
        } else if (GishSettings.touchControls == GishSettings.OLD_TOUCH_CONTROLS) {
            LinearLayout ll = new LinearLayout(this);
            ll.setBackgroundDrawable(getResources().getDrawable(R.drawable.overlay_controls));
            addContentView(ll, new LinearLayout.LayoutParams(
                    LayoutParams.MATCH_PARENT,
                    LayoutParams.MATCH_PARENT));
        }
    }

    // JNI-method
    public static void doVibrate(int duration, int fromJNI) {
        // From JNI: 1 -- yes, 0 -- no
        // 30 is default scale for vibration
        if ((fromJNI == 1) && (GishSettings.gameVibration)) {
            m_vibrator.vibrate(duration + (GishSettings.vibroScale - 30));
        }

        if ((fromJNI == 0) && (GishSettings.touchVibration)) {
            m_vibrator.vibrate(duration + (GishSettings.vibroScale - 30));
        }
    }

    // JNI-function
    public static void openUrl(String aUrl) {
        Uri uriUrl = Uri.parse(aUrl);
        Intent intent = new Intent(Intent.ACTION_VIEW, uriUrl);
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        m_GishActivity.startActivity(intent);
    }
}
