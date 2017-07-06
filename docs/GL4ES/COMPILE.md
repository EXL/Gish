Compiling
----
*for Pandora*

    cmake . -DPANDORA=1; make GL
    
*or for the Raspberry Pi*

    cmake . -DBCMHOST=1; make GL

*or for the ODroid*

    cmake . -DODROID=1; make GL

*or for CHIP machines*

    cmake . -DCHIP=1; make GL

*or for Android*

    An Android.mk is provided that should compile with an NDK

*or use ccmake*

Alternatively, you can use the curses-bases ccmake (or any other gui frontend for cmake) to select wich platform to use interactively.

----

Testing
----
A few tests are included.
They can be launched with `tests/tests.sh`
You will need apitrace and imagemagick for them to run. (on debian and friend, it's `sudo apt install apitrace-gl-frontend imagemagick`)
The tests use a pre-recorded GL trace that is replayed, than a specific frame is captured and compared to a reference picture.
Because each renderer may render slightly differently, there are some fuzz in the comparison, so only significant changes will be detected.
For now, 2 tests are done, one with glxgears (basic testing, using mostly glBegin / glEnd) and stuntcarracer (with more GL stuff, textures and lighting).

----

Per-platform
----

##### OpenPandora

This is the main developpement platform for now. GL4ES works on all 3 models (CC, Rebirth and Gigahertz), and with all driver version.
For the SGX Driver version that doesn't support X11 Windowed mode (i.e. there is no `/usr/lib/libpvrPVR2D_X11WSEGL.so` library in the firmware), you need to use one of the framebuffer output: `LIBGL_FB=1` is the fastest, but you may need `LIBGL_FB=2` if you need 32bits framebuffer, or `LIBGL_FB=3` if you need GL in a Window (but it will be slow, as each frame as to be blitted back in the X11 windows).
On the Pandora, the special `LIBGL_GAMMA`can be used also, to boost gamma at load, using firmware command to change LCD gamma.

##### ODroid

GL4ES works well on ODroid. I can test on XU4 model, but it has been reported to work on all model, including 64bits version.
On ODroid, the EGL context can be created with SRGB attribute, by using `LIBGL_SRGB=1`, for a nice boost in the gamma (supported on most ODroid).

##### C.H.I.P.

GL4ES should work on CHIP ad PocketCHIP. Framebuffers mode will probably ot work, expect `LIBGL_FB=3` mode that seems to work fine, and with adequate performances (there is probably no slow blit, as the driver handle directly the GL->X11 blit).

##### RaspberryPI

GL4ES works on RaspberryPI. I don't own any RPi model, so I cannot fine tune it for this plateform.
Framebuffer mode should work on this plateform. `LIBGL_FB=1` try to use `DispManX` for more speed. Be aware that if you use X-less config, that many SDL version dedicated to DispManX do not handle GL context creation. So trying to create a GL context with this version of SDL will fail, but GL4ES is never called, so I cannot "fix" that (the fix would be inside SDL/DispManX driver).

##### Android
On Android build of GL4ES, no X11 function are called. So most `glX` function are not defined. That means that the GL context (in fact a GLES1.1 context) has to be created by other apps (mainly SDL or SDL2).

On Android version 4.0 and earlier, there is a bug that prevent dlopen to be called inside dlopen (see [here](http://grokbase.com/t/gg/android-ndk/124bdvscqx/block-with-calling-dlopen-and-dlclose)).
GL4ES use a "library constructor" to initialize itself a load, and that constructor do some dlopen (to load EGL and GLES libraries), so it will trigger that bug.
If you are targeting a wide range of device, you should probably activate the workaround:
1. Modify [Android.mk](Android.mk) to uncomment `#LOCAL_CFLAGS += -DNO_NO_INIT_CONSTRUCTOR` near the end of the file, to prevent the use of the library constructor.
2. In your code, call `void initialize_g4es()` as soon as possible after loading GL4ES, and before using any GL function.


