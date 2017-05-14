package ru.exlmoto.gish;

import android.os.Bundle;
import android.view.KeyEvent;

import org.libsdl.app.SDLActivity;

/**
 * Created by exl on 5/14/17.
 */

public class GishActivity extends SDLActivity {

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
                pressOrReleaseKey(KeyEvent.KEYCODE_ESCAPE, press);
                return true;
            default:
                return false;
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }
}
