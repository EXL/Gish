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

import java.io.File;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.Toast;

public class GishLauncherActivity extends Activity {

    private static final int AC_FILE_PICKER_CODE = 1;

    private static final int DIALOG_WRONG_HAPTIC = 0;
    private static final int DIALOG_WRONG_ZOOM = 1;
    private static final int DIALOG_QUESTION_RESET_SETTINGS = 2;

    public static class GishSettings {

        public static final int NO_TOUCH_CONTROLS = 0;
        public static final int OLD_TOUCH_CONTROLS = 1;
        public static final int MODERN_TOUCH_CONTROLS = 2;

        // GAME SETTINGS
        public static int touchControls = MODERN_TOUCH_CONTROLS;
        public static boolean sound = true;     // Access from JNI
        public static boolean music = true;     // Access from JNI
        public static boolean showFps = false;  // Access from JNI
        public static boolean fixCache = false; // Access from JNI
        public static boolean joyAccel = false; // Access from JNI
        public static boolean openGles = true;
        public static boolean lights = false;   // Access from JNI
        public static boolean shadows = false;  // Access from JNI
        public static boolean touchVibration = true;
        public static boolean gameVibration = true;
        public static int vibroScale = 30;
        public static String gishDataSavedPath = ""; // Access from JNI
        public static float zoom = 10.0f;
    }

    private boolean firstRun = false;

    private EditText editTextDataPath = null;
    private EditText editTextHaptics = null;
    private EditText editTextZoom = null;
    private CheckBox checkBoxSound = null;
    private CheckBox checkBoxMusic = null;
    private CheckBox checkBoxJoyAccel = null;
    private CheckBox checkBoxFixCache = null;
    private CheckBox checkBoxShowFps = null;
    private CheckBox checkBoxLights = null;
    private CheckBox checkBoxShadows = null;
    private CheckBox checkBoxVibrationInGame = null;
    private CheckBox checkBoxVibrationOnTouch = null;
    private RadioButton radioButtonModernTouchControls = null;
    private RadioButton radioButtonSimpleTouchControls = null;
    private RadioButton radioButtonNoTouchControls = null;
    private RadioButton radioGles = null;
    private RadioButton radioGl4es = null;

    private AlertDialog aboutDialog = null;

    public SharedPreferences mSharedPreferences = null;
    GishLauncherActivity gishLauncherActivity = this;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        if (mSharedPreferences == null) {
            mSharedPreferences = getSharedPreferences("ru.exlmoto.gish", MODE_PRIVATE);
        }

        // Check the first run
        if (mSharedPreferences.getBoolean("firstrun", true)) {
            // The first run, fill GUI layout with default values
            mSharedPreferences.edit().putBoolean("firstrun", false).commit();
            firstRun = true;
        } else {
            firstRun = false;
            // Read settings from Shared Preferences
            // Move to onResume()
            // readSettings();
        }

        initAboutDialog();

        setContentView(R.layout.gish_launcher);

        initWidgets();

        // Move to onResume()
        // fillWidgetsBySettings();

        checkBoxSound.setOnCheckedChangeListener(new OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                GishSettings.sound = isChecked;
                if (!isChecked) {
                    GishSettings.music = false;
                    checkBoxMusic.setChecked(false);
                    checkBoxMusic.setEnabled(false);
                } else {
                    checkBoxMusic.setEnabled(true);
                }
            }
        });

        checkBoxMusic.setOnCheckedChangeListener(new OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                GishSettings.music = isChecked;
            }
        });

        checkBoxJoyAccel.setOnCheckedChangeListener(new OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                GishSettings.joyAccel = isChecked;
            }
        });

        checkBoxFixCache.setOnCheckedChangeListener(new OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                GishSettings.fixCache = isChecked;
            }
        });

        checkBoxShowFps.setOnCheckedChangeListener(new OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                GishSettings.showFps = isChecked;
            }
        });

        checkBoxLights.setOnCheckedChangeListener(new OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                GishSettings.lights = isChecked;
            }
        });

        checkBoxShadows.setOnCheckedChangeListener(new OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                GishSettings.shadows = isChecked;
            }
        });

        radioButtonModernTouchControls.setOnCheckedChangeListener(new OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                GishSettings.touchControls = GishSettings.MODERN_TOUCH_CONTROLS;
            }
        });

        radioButtonSimpleTouchControls.setOnCheckedChangeListener(new OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                GishSettings.touchControls = GishSettings.OLD_TOUCH_CONTROLS;
            }
        });

        radioButtonNoTouchControls.setOnCheckedChangeListener(new OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                GishSettings.touchControls = GishSettings.NO_TOUCH_CONTROLS;
            }
        });

        checkBoxVibrationOnTouch.setOnCheckedChangeListener(new OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                GishSettings.touchVibration = isChecked;
                if (!isChecked && !checkBoxVibrationInGame.isChecked()) {
                    editTextHaptics.setText(String.valueOf(30));
                    editTextHaptics.setEnabled(false);
                } else {
                    editTextHaptics.setEnabled(true);
                }
            }
        });

        checkBoxVibrationInGame.setOnCheckedChangeListener(new OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                GishSettings.gameVibration = isChecked;
                if (!isChecked && !checkBoxVibrationOnTouch.isChecked()) {
                    editTextHaptics.setText(String.valueOf(30));
                    editTextHaptics.setEnabled(false);
                } else {
                    editTextHaptics.setEnabled(true);
                }
            }
        });

        Button buttonRun = (Button) findViewById(R.id.buttonRun);
        buttonRun.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                if (testAllRanges()) {
                    writeSettings();

                    // Attempt to read the random data file
                    File readMeFile = new File(GishSettings.gishDataSavedPath + "texture/face.tga");
                    if (readMeFile.exists() && readMeFile.isFile()) {
                        Intent intent = new Intent(gishLauncherActivity, GishActivity.class);
                        startActivity(intent);
                    } else {
                        showToast(R.string.data_path_wrong, Toast.LENGTH_LONG);
                    }
                }
            }
        });

        Button buttonBrowse = (Button) findViewById(R.id.buttonBrowse);
        buttonBrowse.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                runFilePicker();
            }
        });

        Button buttonResetAllSettings = (Button) findViewById(R.id.buttonResetSettings);
        buttonResetAllSettings.setOnClickListener(new OnClickListener() {

            @SuppressWarnings("deprecation")
            @Override
            public void onClick(View v) {
                showDialog(DIALOG_QUESTION_RESET_SETTINGS);
            }
        });

        Button buttonAbout = (Button) findViewById(R.id.buttonAbout);
        buttonAbout.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                showMyDialog(aboutDialog);
            }
        });
    }

    @Override
    protected void onResume() {
        if (!firstRun) {
            readSettings();
        }
        fillWidgetsBySettings();
        super.onResume();
    }

    @SuppressWarnings("deprecation")
    private boolean testAllRanges() {
        int haptics = Integer.parseInt(editTextHaptics.getEditableText().toString());
        if (haptics < 5 || haptics > 200) {
            showDialog(DIALOG_WRONG_HAPTIC);
            return false;
        }
        float zoom = Float.parseFloat(editTextZoom.getEditableText().toString());
        if (zoom < 4.0f || zoom > 50.0f) {
            showDialog(DIALOG_WRONG_ZOOM);
            return false;
        }
        return true;
    }

    // Thanks to teedyay from:
    // http://stackoverflow.com/a/6425744
    private void deleteRecursive(File fileOrDirectory) {
        if (fileOrDirectory.isDirectory()) {
            for (File child : fileOrDirectory.listFiles()) {
                deleteRecursive(child);
            }
        }
        fileOrDirectory.delete();
        GishActivity.toDebugLog("File/Dir: " + fileOrDirectory.getName() + " was deleted");
    }

    private void resetAllSettingsToDefaultValues() {
        // Delete user profiles and config
        File dataDir = new File(this.getFilesDir() + "/.gish/");
        if (dataDir.exists() && dataDir.isDirectory()) {
            deleteRecursive(dataDir);
        }

        GishSettings.touchControls = GishSettings.MODERN_TOUCH_CONTROLS;
        GishSettings.sound = true;
        GishSettings.music = true;
        GishSettings.showFps = false;
        GishSettings.fixCache = false;
        GishSettings.joyAccel = false;
        GishSettings.openGles = true;
        GishSettings.lights = false;
        GishSettings.shadows = false;
        GishSettings.touchVibration = true;
        GishSettings.gameVibration = true;
        GishSettings.vibroScale = 30;
        GishSettings.gishDataSavedPath = "";
        GishSettings.zoom = 10.0f;

        fillWidgetsBySettings();

        showToast(R.string.reset_game, Toast.LENGTH_SHORT);
    }

    private void showToast(int stringId, int length) {
        Toast.makeText(gishLauncherActivity,
                getResources().getString(stringId),
                length).show();
    }

    private void runFilePicker() {
        Intent intent = new Intent(this, GishFilePickerActivity.class);
        startActivityForResult(intent, AC_FILE_PICKER_CODE);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (data == null) {
            return;
        }
        switch (requestCode) {
        case AC_FILE_PICKER_CODE:
            if (resultCode == RESULT_OK) {
                GishSettings.gishDataSavedPath = data.getStringExtra("GishPath");
                editTextDataPath.setText(GishSettings.gishDataSavedPath);
                writeSettings();
                showToast(R.string.data_path_c, Toast.LENGTH_SHORT);
            }
            break;
        default:
            break;
        }
    }

    @Override
    protected void onDestroy() {
        writeSettings();

        super.onDestroy();
    }

    @Override
    protected Dialog onCreateDialog(int id) {
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        switch (id) {
        case DIALOG_WRONG_HAPTIC:
            builder.setTitle(getString(R.string.wrong_haptic_title));
            builder.setMessage(getString(R.string.wrong_haptic_body) +
                    "\nMin: " + 5 + "\nMax: " + 200 + "\n" +
                    getString(R.string.wrong_body_general));
            builder.setPositiveButton(getString(R.string.button_ok), new DialogInterface.OnClickListener() {

                @Override
                public void onClick(DialogInterface dialog, int id) {
                    GishSettings.vibroScale = 30;
                    editTextHaptics.setText(String.valueOf(GishSettings.vibroScale));
                    dialog.cancel();
                }
            });
            break;
        case DIALOG_WRONG_ZOOM:
            builder.setTitle(getString(R.string.wrong_zoom_title));
            builder.setMessage(getString(R.string.wrong_zoom_body) +
                    "\nMin: " + 4.0f + "\nMax: " + 50.0f + "\n" +
                    getString(R.string.wrong_body_general));
            builder.setPositiveButton(getString(R.string.button_ok), new DialogInterface.OnClickListener() {

                @Override
                public void onClick(DialogInterface dialog, int id) {
                    GishSettings.zoom = 10.0f;
                    editTextZoom.setText(String.valueOf(GishSettings.zoom));
                    dialog.cancel();
                }
            });
            break;
        case DIALOG_QUESTION_RESET_SETTINGS:
            builder.setTitle(getString(R.string.question_title_general));
            builder.setMessage(getString(R.string.question_reset_body));
            builder.setPositiveButton(getString(R.string.button_ok), new DialogInterface.OnClickListener() {

                @Override
                public void onClick(DialogInterface dialog, int id) {
                    resetAllSettingsToDefaultValues();
                    dialog.cancel();
                }
            });
            builder.setNegativeButton(getString(R.string.button_cancel), new DialogInterface.OnClickListener() {

                @Override
                public void onClick(DialogInterface dialog, int id) {
                    dialog.cancel();
                }
            });
            break;
        default:
            break;
        }
        return builder.create();
    }

    private void initAboutDialog() {
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        LayoutInflater inflater = this.getLayoutInflater();
        final ViewGroup nullParentVG = null;
        View dialogView = inflater.inflate(R.layout.gish_dialog_about, nullParentVG);
        builder.setView(dialogView);
        builder.setTitle(R.string.app_name);
        builder.setPositiveButton(R.string.button_ok, null);
        aboutDialog = builder.create();
    }

    // Prevent dialog dismiss when orientation changes
    // http://stackoverflow.com/a/27311231/2467443
    private static void doKeepDialog(AlertDialog dialog) {
        WindowManager.LayoutParams lp = new WindowManager.LayoutParams();
        lp.copyFrom(dialog.getWindow().getAttributes());
        lp.width = WindowManager.LayoutParams.WRAP_CONTENT;
        lp.height = WindowManager.LayoutParams.WRAP_CONTENT;
        dialog.getWindow().setAttributes(lp);
    }

    private void showMyDialog(AlertDialog dialog) {
        dialog.show();
        doKeepDialog(dialog);
    }

    private void fillSettingsByWidgets() {
        GishActivity.toDebugLog("FSBW!");
        GishSettings.showFps = checkBoxShowFps.isChecked();
        GishSettings.fixCache = checkBoxFixCache.isChecked();
        GishSettings.joyAccel = checkBoxJoyAccel.isChecked();
        GishSettings.lights = checkBoxLights.isChecked();
        GishSettings.shadows = checkBoxShadows.isChecked();
        GishSettings.touchVibration = checkBoxVibrationOnTouch.isChecked();
        GishSettings.gameVibration = checkBoxVibrationInGame.isChecked();
        GishSettings.vibroScale = Integer.parseInt(editTextHaptics.getEditableText().toString());
        GishSettings.gishDataSavedPath = editTextDataPath.getEditableText().toString();
        GishSettings.zoom = Float.parseFloat(editTextZoom.getEditableText().toString());
        if (radioButtonModernTouchControls.isChecked()) {
            GishSettings.touchControls = GishSettings.MODERN_TOUCH_CONTROLS;
        } else if (radioButtonSimpleTouchControls.isChecked()) {
            GishSettings.touchControls = GishSettings.OLD_TOUCH_CONTROLS;
        } else if (radioButtonNoTouchControls.isChecked()) {
            GishSettings.touchControls = GishSettings.NO_TOUCH_CONTROLS;
        }
        if (radioGles.isChecked()) {
            GishSettings.openGles = true;
        } else if (radioGl4es.isChecked()) {
            GishSettings.openGles = false;
        }
        GishSettings.sound = checkBoxSound.isChecked();
        if (!GishSettings.sound) {
            GishSettings.music = false;
        } else {
            GishSettings.music = checkBoxMusic.isChecked();
        }
    }

    private void fillWidgetsBySettings() {
        GishActivity.toDebugLog("FWBS!");
        editTextDataPath.setText(GishSettings.gishDataSavedPath);
        editTextHaptics.setText(String.valueOf(GishSettings.vibroScale));
        editTextZoom.setText(String.valueOf(GishSettings.zoom));
        checkBoxJoyAccel.setChecked(GishSettings.joyAccel);
        checkBoxFixCache.setChecked(GishSettings.fixCache);
        checkBoxShowFps.setChecked(GishSettings.showFps);
        checkBoxLights.setChecked(GishSettings.lights);
        checkBoxShadows.setChecked(GishSettings.shadows);
        checkBoxVibrationOnTouch.setChecked(GishSettings.touchVibration);
        checkBoxVibrationInGame.setChecked(GishSettings.gameVibration);
        switch (GishSettings.touchControls) {
        case GishSettings.MODERN_TOUCH_CONTROLS:
            radioButtonModernTouchControls.setChecked(true);
            break;
        case GishSettings.OLD_TOUCH_CONTROLS:
            radioButtonSimpleTouchControls.setChecked(true);
            break;
        case GishSettings.NO_TOUCH_CONTROLS:
            radioButtonNoTouchControls.setChecked(true);
            break;
        default:
            break;
        }
        if (GishSettings.openGles) {
            radioGles.setChecked(true);
        } else {
            radioGl4es.setChecked(true);
        }
        if (!checkBoxVibrationInGame.isChecked() && !checkBoxVibrationOnTouch.isChecked()) {
            editTextHaptics.setEnabled(false);
        }
        checkBoxSound.setChecked(GishSettings.sound);
        if (!GishSettings.sound) {
            checkBoxMusic.setChecked(false);
            checkBoxMusic.setEnabled(false);
        } else {
            checkBoxMusic.setChecked(GishSettings.music);
            checkBoxMusic.setEnabled(true);
        }
    }

    private void initWidgets() {
        editTextDataPath = (EditText) findViewById(R.id.editTextDataPath);
        editTextHaptics = (EditText) findViewById(R.id.editTextVibroScale);
        editTextZoom = (EditText) findViewById(R.id.editTextZoom);
        checkBoxSound = (CheckBox) findViewById(R.id.checkBoxSound);
        checkBoxMusic = (CheckBox) findViewById(R.id.checkBoxMusic);
        checkBoxJoyAccel = (CheckBox) findViewById(R.id.checkBoxJoyAccel);
        checkBoxFixCache = (CheckBox) findViewById(R.id.checkBoxFixCache);
        checkBoxShowFps = (CheckBox) findViewById(R.id.checkBoxFps);
        checkBoxLights = (CheckBox) findViewById(R.id.checkBoxLights);
        checkBoxShadows = (CheckBox) findViewById(R.id.checkBoxShadows);
        checkBoxVibrationOnTouch = (CheckBox) findViewById(R.id.checkBoxVibroOnTouch);
        checkBoxVibrationInGame = (CheckBox) findViewById(R.id.checkBoxVibtoInGame);
        radioButtonModernTouchControls = (RadioButton) findViewById(R.id.radioButtonModern);
        radioButtonSimpleTouchControls = (RadioButton) findViewById(R.id.radioButtonSimple);
        radioButtonNoTouchControls = (RadioButton) findViewById(R.id.radioButtonOff);
        radioGles = (RadioButton) findViewById(R.id.radioButtonGles);
        radioGl4es = (RadioButton) findViewById(R.id.radioButtonGl4es);
    }

    public void writeSettings() {
        fillSettingsByWidgets();

        SharedPreferences.Editor editor = mSharedPreferences.edit();
        editor.putInt("touchControls", GishSettings.touchControls);
        editor.putInt("vibroScale", GishSettings.vibroScale);
        editor.putBoolean("sound", GishSettings.sound);
        editor.putBoolean("music", GishSettings.music);
        editor.putBoolean("showFps", GishSettings.showFps);
        editor.putBoolean("joyAccel", GishSettings.joyAccel);
        editor.putBoolean("fixCache", GishSettings.fixCache);
        editor.putBoolean("openGles", GishSettings.openGles);
        editor.putBoolean("lights", GishSettings.lights);
        editor.putBoolean("shadows", GishSettings.shadows);
        editor.putBoolean("touchVibration", GishSettings.touchVibration);
        editor.putBoolean("gameVibration", GishSettings.gameVibration);
        editor.putString("dataSavedPath", GishSettings.gishDataSavedPath);
        editor.putFloat("zoom", GishSettings.zoom);
        editor.commit();
    }

    public void readSettings() {
        GishActivity.toDebugLog("Read Settings!");

        GishSettings.touchControls = mSharedPreferences.getInt("touchControls", GishSettings.MODERN_TOUCH_CONTROLS);
        GishSettings.vibroScale = mSharedPreferences.getInt("vibroScale", 30);
        GishSettings.sound = mSharedPreferences.getBoolean("sound", true);
        GishSettings.music = mSharedPreferences.getBoolean("music", true);
        GishSettings.showFps = mSharedPreferences.getBoolean("showFps", false);
        GishSettings.joyAccel = mSharedPreferences.getBoolean("joyAccel", false);
        GishSettings.fixCache = mSharedPreferences.getBoolean("fixCache", false);
        GishSettings.openGles = mSharedPreferences.getBoolean("openGles", true);
        GishSettings.lights = mSharedPreferences.getBoolean("lights", false);
        GishSettings.shadows = mSharedPreferences.getBoolean("shadows", false);
        GishSettings.touchVibration = mSharedPreferences.getBoolean("touchVibration", true);
        GishSettings.gameVibration = mSharedPreferences.getBoolean("gameVibration", true);
        GishSettings.gishDataSavedPath = mSharedPreferences.getString("dataSavedPath", "");
        GishSettings.zoom = mSharedPreferences.getFloat("zoom", 10.0f);
    }
}
