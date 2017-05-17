/************************************************************************************
** The MIT License (MIT)
**
** Copyright (c) 2017 EXL
** Copyright (c) 2012 [SoD]Thor
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

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;

import org.libsdl.app.SDLActivity;

public class GishTouchControlsView extends View {

	private Paint paint = null;

	private int width = 0;
	private int height = 0;

	private int radius = 0;
	private static int[] pid = new int[16];

	private Bitmap button_a = null;
	private Bitmap button_space = null;
	private Bitmap button_d = null;
	private Bitmap button_s = null;
	private Bitmap button_ok = null;

	private int b_wh = 0;

	private int col1_w = 0;
	private int col2_w = 0;
	private int row1_h = 0;
	private int row2_h = 0;
	private int row3_h = 0;
	private int rad_d2 = 0;

	private static final int VIBRO_OFFSET = 20;

	// --- Game Keycodes Table
	// Joystick
	private static final int KC_GAME_UP = 2;
	private static final int KC_GAME_DOWN = 6;
	private static final int KC_GAME_RIGHT = 0;
	private static final int KC_GAME_LEFT = 4;
	private static final int KC_GAME_UPRIGHT = 3;
	private static final int KC_GAME_UPLEFT = 1;
	private static final int KC_GAME_DOWNLEFT = 5;
	private static final int KC_GAME_DOWNRIGHT = 7;
	// Buttons
	private static final int KC_GAME_D = 8;
	private static final int KC_GAME_A = 9;
	private static final int KC_GAME_ENTER = 10;
	private static final int KC_GAME_SPACE = 11;
	private static final int KC_GAME_S = 13;

	// Buttons
	private static final int KC_SDL_D = KeyEvent.KEYCODE_D;          // D
	private static final int KC_SDL_A = KeyEvent.KEYCODE_A;          // A
	private static final int KC_SDL_SPACE = KeyEvent.KEYCODE_SPACE;  // Space
	private static final int KC_SDL_S = KeyEvent.KEYCODE_S;          // S
	private static final int KC_SDL_ENTER = KeyEvent.KEYCODE_ENTER;  // Enter

	public GishTouchControlsView(Context context) {
		super(context);

		button_a = BitmapFactory.decodeResource(context.getResources(), R.drawable.button_a);
		button_space = BitmapFactory.decodeResource(context.getResources(), R.drawable.button_space);
		button_d = BitmapFactory.decodeResource(context.getResources(), R.drawable.button_d);
		button_s = BitmapFactory.decodeResource(context.getResources(), R.drawable.button_s);
		button_ok = BitmapFactory.decodeResource(context.getResources(), R.drawable.button_ok);

		b_wh = button_a.getWidth();

		GishActivity.toDebugLog("Touch buttons size: " + b_wh + "x" + b_wh);

		paint = new Paint();
	}

	@Override
	protected void onDraw(Canvas canvas) {
		paint.setARGB(128, 255, 255, 255);

		paint.setFilterBitmap(true);
		paint.setAntiAlias(true);

		// Joystick
		if (p0id >= 0) {
			paint.setARGB(16, 255, 255, 255);
			canvas.drawCircle(p0x, p0y, radius << 1, paint);
			for (int i = 0; i < 8; ++i) {
				paint.setARGB(i == p1b ? 96 : 32, 255, 128, 128);
				canvas.drawCircle((float) (p0x + 7 * radius * Math.cos(i * Math.PI / 4) / 4),
						(float) (p0y - 7 * radius * Math.sin(i * Math.PI / 4) / 4),
						radius / 4, paint);
			}
			paint.setARGB(128, 255, 255, 255);
			canvas.drawCircle(p0x, p0y, radius / 3, paint);
			canvas.drawCircle(p1x, p1y, radius, paint);
		}

		// Buttons
		paint.setARGB(pid[KC_GAME_D] < 0 ? 64 : 160, 255, 255, 255);
		canvas.drawBitmap(button_d, col2_w, row1_h, paint);
		paint.setARGB(pid[KC_GAME_A] < 0 ? 64 : 160, 255, 255, 255);
		canvas.drawBitmap(button_a, col1_w, row3_h, paint);
		paint.setARGB(pid[KC_GAME_SPACE] < 0 ? 64 : 160, 255, 255, 255);
		canvas.drawBitmap(button_space, col1_w, row1_h, paint);
		paint.setARGB(pid[KC_GAME_S] < 0 ? 64 : 160, 255, 255, 255);
		canvas.drawBitmap(button_s, col1_w, row2_h, paint);
		paint.setARGB(pid[KC_GAME_ENTER] < 0 ? 64 : 160, 255, 255, 255);
		canvas.drawBitmap(button_ok, col2_w, row2_h, paint);

		invalidate();

		super.onDraw(canvas);
	}

	private static List<MotionEvent> events = new ArrayList<MotionEvent>();

	public static void addEvent(MotionEvent event) {
		synchronized (events) {
			events.add(event);
		}
	}

	@SuppressLint({ "Recycle", "ClickableViewAccessibility" })
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		addEvent(MotionEvent.obtainNoHistory(event));
		checkInput();
		// event.recycle();
		return true;
	}

	@Override
	public void onWindowFocusChanged(boolean hasFocus) {
		super.onWindowFocusChanged(hasFocus);

		width = getWidth();
		height = getHeight();

		radius = Math.min(width, height) / 11;
		Arrays.fill(pid, -1);

		// Some optimizations
		col1_w = width - b_wh - b_wh / 4;
		col2_w = width - b_wh * 3;
		row1_h = height - b_wh - b_wh / 4;
		row2_h = b_wh / 4;
		row3_h = height / 2 - b_wh / 2;
		rad_d2 = radius / 3;

		GishActivity.toDebugLog("Set Resolution of Input layer as: " + width + "x" + height + ", radius: " + radius);
	}

	private int p0x, p0y, p1x, p1y, p1b, p0id = -1;

	public void checkInput() {
		List<MotionEvent> copy;
		synchronized (events) {
			copy = new ArrayList<MotionEvent>(events);
			events.clear();
		}
		int[] npid = Arrays.copyOf(pid, pid.length);
		int id, index, b, x, y;
		int sector_move;
		for (MotionEvent e : copy) {
			sector_move = width - b_wh * 7 / 2; // 3.5
			switch(e.getActionMasked()) {
			case MotionEvent.ACTION_MOVE:
				int len = e.getPointerCount();
				Arrays.fill(npid, -1);
				p1b = -1;
				for (index = 0; index < len; ++index) {
					id = e.getPointerId(index);
					x = (int) e.getX(index);
					y = (int) e.getY(index);
					if (x < sector_move) {
						b = getDirection(x, y);
					} else {
						b = getButton(x, y);
					}
					if (b >= 0) {
						npid[b] = id;
					}
				}
				break;
			case MotionEvent.ACTION_DOWN:
				index = e.getActionIndex();
				id = e.getPointerId(index);
				cleanPointer(npid, id);
				x = (int) e.getX(index);
				y = (int) e.getY(index);
				if (x < sector_move) {
					p1b = -1;
					p0x = x;
					p0y = y;
					p1x = p0x;
					p1y = p0y;
					p0id = id;
				} else {
					b = getButton(x, y);
					if (b >= 0) {
						npid[b] = id;
						//AAAAActivity.doVibrate(50 - VIBRO_OFFSET, 0);
					}
				}
				break;
			case MotionEvent.ACTION_UP:
				id = e.getPointerId(e.getActionIndex());
				cleanPointer(npid, id);
				if (id == p0id) {
					p0id = -1;
				}
				break;
			case MotionEvent.ACTION_POINTER_DOWN:
				index = e.getActionIndex();
				id = e.getPointerId(index);
				cleanPointer(npid, id);
				x = (int) e.getX(index);
				y = (int) e.getY(index);
				if (x < sector_move) {
					if (p0id < 0) {
						p1b = -1;
						p0x = x;
						p0y = y;
						p1x = p0x;
						p1y = p0y;
						p0id = id;
					}
				} else {
					b = getButton(x, y);
					if (b >= 0) {
						npid[b] = id;
						//AAAAActivity.doVibrate(50 - VIBRO_OFFSET, 0);
					}
				}
				break;
			case MotionEvent.ACTION_POINTER_UP:
				id = e.getPointerId(e.getActionIndex());
				cleanPointer(npid, id);
				if (id == p0id) {
					p0id = -1;
				}
				break;
			}
		}
		// lastKeyTyped = 0;
		// lastKey = 0;
		for (int i = 0; i < npid.length; ++i) {
			if (npid[i] < 0) {
				if (pid[i] >= 0) {
					releaseKey(i);
				}
			} else {
				pressKey(i);
			}
			pid[i] = npid[i];
		}
	}

	public void pressKey(int keyCode) {
		// AAAAActivity.toDebugLog(keyCode + " pushed!");
		switch (keyCode) {
		case KC_GAME_D:
			SDLActivity.onNativeKeyDown(KC_SDL_D);
			break;
		case KC_GAME_A:
			SDLActivity.onNativeKeyDown(KC_SDL_A);
			break;
		case KC_GAME_SPACE:
			SDLActivity.onNativeKeyDown(KC_SDL_SPACE);
			break;
		case KC_GAME_S:
			SDLActivity.onNativeKeyDown(KC_SDL_S);
			break;
		case KC_GAME_ENTER:
			SDLActivity.onNativeKeyDown(KC_SDL_ENTER);
			break;
		case KC_GAME_UP:
			SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_DPAD_UP);
			break;
		case KC_GAME_DOWN:
			SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_DPAD_DOWN);
			break;
		case KC_GAME_RIGHT:
			SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_DPAD_RIGHT);
			break;
		case KC_GAME_LEFT:
			SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_DPAD_LEFT);
			break;
		case KC_GAME_UPRIGHT:
			SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_DPAD_UP);
			SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_DPAD_LEFT);
			break;
		case KC_GAME_UPLEFT:
			SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_DPAD_UP);
			SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_DPAD_RIGHT);
			break;
		case KC_GAME_DOWNRIGHT:
			SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_DPAD_DOWN);
			SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_DPAD_RIGHT);
			break;
		case KC_GAME_DOWNLEFT:
			SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_DPAD_DOWN);
			SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_DPAD_LEFT);
			break;
		default:
			break;
		}
	}

	public void releaseKey(int keyCode) {
		// AAAAActivity.toDebugLog(keyCode + " released!");
		switch (keyCode) {
		case KC_GAME_D:
			SDLActivity.onNativeKeyUp(KC_SDL_D);
			break;
		case KC_GAME_A:
			SDLActivity.onNativeKeyUp(KC_SDL_A);
			break;
		case KC_GAME_SPACE:
			SDLActivity.onNativeKeyUp(KC_SDL_SPACE);
			break;
		case KC_GAME_S:
			SDLActivity.onNativeKeyUp(KC_SDL_S);
			break;
		case KC_GAME_ENTER:
			SDLActivity.onNativeKeyUp(KC_SDL_ENTER);
			break;
		case KC_GAME_UP:
			SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_DPAD_UP);
			break;
		case KC_GAME_DOWN:
			SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_DPAD_DOWN);
			break;
		case KC_GAME_RIGHT:
			SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_DPAD_RIGHT);
			break;
		case KC_GAME_LEFT:
			SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_DPAD_LEFT);
			break;
		case KC_GAME_UPRIGHT:
			SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_DPAD_UP);
			SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_DPAD_LEFT);
			break;
		case KC_GAME_UPLEFT:
			SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_DPAD_UP);
			SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_DPAD_RIGHT);
			break;
		case KC_GAME_DOWNRIGHT:
			SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_DPAD_DOWN);
			SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_DPAD_RIGHT);
			break;
		case KC_GAME_DOWNLEFT:
			SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_DPAD_DOWN);
			SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_DPAD_LEFT);
			break;
		default:
			break;
		}
	}

	public void cleanPointer(int[] npid, int id) {
		for (int i = 0; i < npid.length; ++i) {
			if (npid[i] == id) {
				npid[i] = -1;
				break;
			}
		}
	}

	public int getDirection(int bx, int by) {
		int abx = bx - p0x;
		int aby = p0y - by;

		double lenab = Math.sqrt(abx * abx + aby * aby);

		if (lenab > radius) {
			p1x = p0x + (int) ((bx - p0x) * (radius / lenab));
			p1y = p0y + (int) ((by - p0y) * (radius / lenab));
		} else {
			p1x = bx;
			p1y = by;
		}

		if (lenab < (radius / 3)) {
			p1b = -1;
		} else {
			double rslt = aby == 0 ? (abx > 0 ? 0 : Math.PI) : Math.acos(abx / lenab);
			if (by > p0y) {
				rslt = (Math.PI * 2) - rslt;
			}
			p1b = ((int) ((rslt + Math.PI / 16) / (Math.PI / 4))) & 0x7;
		}
		return p1b;
	}

	public int getButton(int x, int y) {
		int b = -1;
		if (x > width - b_wh * 1.8) {
			if (y > (height / 3) * 2) { // Down
				b = KC_GAME_SPACE;
			} else if (y < (height / 3)) { // Up
				b = KC_GAME_S; // b = KC_GAME_L;
			} else { // Center
				b = KC_GAME_A;
			}
		} else {
			// AAAAActivity.toDebugLog("Second Buttons Column:" + x + "x" + y);
			if (y > (height / 3) * 2) { // Down
				b = KC_GAME_D;
			} else if (y < (height / 3)) { // Up
				b = KC_GAME_ENTER; // b = KC_GAME_S;
			} else { // Center
				b = -1; // b = KC_GAME_SELECT;
			}
		}
		return b;
	}
}
