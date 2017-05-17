package ru.exlmoto.gish;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.widget.LinearLayout;

import org.libsdl.app.SDLActivity;

/**
 * Created by exl on 5/14/17.
 */

public class GishActivity extends SDLActivity {

    private static final String APP_TAG = "Gish_app";
    private static Activity m_GishActivity = null;
    private GishTouchControlsView gishTouchControlsView = null;

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

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        m_GishActivity = this;

        gishTouchControlsView = new GishTouchControlsView(this);
        addContentView(gishTouchControlsView,
                new LinearLayout.LayoutParams(
                        LinearLayout.LayoutParams.MATCH_PARENT,
                        LinearLayout.LayoutParams.MATCH_PARENT));
    }

    // JNI-function
    public static void openUrl(String aUrl) {
        Uri uriUrl = Uri.parse(aUrl);
        Intent intent = new Intent(Intent.ACTION_VIEW, uriUrl);
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        m_GishActivity.startActivity(intent);
    }
}
