Usage
----

There are many environnement variable to control gl4es behavour. All are numeric, except LIBGL_VERSION that take a string.
You can also change many of this variable at runtime using the `glHint(...)` function. See [gl4eshint.h](include/gl4eshint.h) for the list of #define to use in this function.

##### LIBGL_FB
Controls the Framebuffer output
 * 0 : Default, using standard x11 rendering
 * 1 : Use Framebuffer output (x11 bypassed, only fullscreen)
 * 2 : Use Framebuffer, but also an intermediary FBO
 * 3 : Use PBuffer, allowing x11 (and windowed) rendering even if driver doesn't support it 

##### LIBGL_XREFRESH
Debug helper in specific cases
 * 0 : Default, nothing special
 * 1 : xrefresh will be called on cleanup

##### LIBGL_STACKTRACE
Automatic Backtrace log
 * 0 : Default, nothing special
 * 1 : stacktrace will be printed on crash

##### LIBGL_VSYNC
VSync control
 * 0 : Default, nothing special
 * 1 : vsync enabled

##### LIBGL_RECYCLEFBO
Recycling FBO special case (don't delete a created FBO, but recycle it if needed)
 * 0 : Default, nothing special
 * 1 : Recycling of FBO enabled

##### LIBGL_MIPMAP
Handling of Manual and Automatic MIPMAP
 * 0 : Default, nothing special
 * 1 : AutoMipMap forced
 * 2 : guess AutoMipMap (based on manual mipmaping on textures)
 * 3 : ignore MipMap (mipmap creation / use entirely disabled)
 * 4 : ignore AutoMipMap on non-squared textures
 
##### LIBGL_TEXCOPY
Make a local copy of every texture for easy glGetTexImage2D
 * 0 : Default, nothing special
 * 1 : Texture copy enabled

##### LIBGL_SHRINK
Texture shrinking control
 * 0 : Default, nothing special
 * 1 : everything / 2 (using original algorithm for size reduction, all other shink mode use a refined algorithm)
 * 2 : only textures wich one size > 512 are / 2
 * 3 : only textures wich one size > 256 are / 2
 * 4 : only textures wich one size > 256 are / 2, and the one > 1024 are / 4
 * 5 : only textures wich one size > 256 are resized to 256 (if possible, because only /2 and /4 exists), but empty texture are not shrinked
 * 6 : only textures wich one size > 128 are / 2, thoses >= 512 are resized to 256 (if possible, because only /2 and /4 exists), but empty texture are not shrinked
 * 7 : only textures wich one size > 512 are / 2, but empty texture are not shrinked
 * 8 : advertise a max texture size of 8192, but every texture wich one size > 2048 are shrinked to 2048
 * 9 : advertise a max texture size of 8192, but every texture wich one size > 4096 are / 4 and the one > 512 are / 2, but empty texture are not shrinked
 * 10: advertise a max texture size of 8192, but every texture wich one size > 2048 are / 4 and the one > 512 are / 2, but empty texture are not shrinked
 
##### LIBGL_TEXDUMP
Texture dump
 * 0 : Default, nothing special
 * 1 : Texture dump enabled

##### LIBGL_ALPHAHACK
Experimental: enable Alpha test only when using texture that contains an alpha channel
 * 0 : Default, nothing special
 * 1 : Alpha Hack enabled

##### LIBGL_NODOWNSAMPLING
Texture downsampling control (deprecated, use LIBGL_AVOID16BITS instead)
 * 0 : Default, DXTc texture are downsampled to 16bits
 * 1 : DXTc texture are left as 32bits RGBA

##### LIBGL_STREAM
PANDORA only: enable Texture Streaming (works only on RGB textures)
 * 0 : Default, nothing special
 * 1 : Enabled on empty RGB textures
 * 2 : Enabled on all RGB textures

##### LIBGL_COPY
Control the glCopyTex(Sub)Image2D hack (they are buggy on pandora and don't work most of the time)
 * 0 : Don't use native glCopyTex(Sub)Image2D, but a workaround function using FBO
 * 1 : No glCopyTexImage2D / glCopyTexSubImage2D hack, use native ones

##### LIBGL_NOLUMALPHA
Control the availability of the LUMUNANCE_ALPHA format (can be buggy on Pandora model CC)
 * 0 : Default,GL_LUMINANCE_ALPHA is available and used if needed
 * 1 : GL_LUMINANCE_ALPHA hardware support disabled (a GL_RGBA texture will be used instead)

##### LIBGL_BLENDHACK
Experimental: Change Blend GL_SRC_ALPHA, GL_ONE to GL_ONE, GL_ONE
 * 0 : Default, nothing special
 * 1 : Change Blend GL_SRC_ALPHA, GL_ONE to GL_ONE, GL_ONE (can be usefull for Xash3D engine)

##### LIBGL_BLENDCOLOR
Hack: Export a (faked) glBlendColor
 * 0 : Default, don't expose gBlendColor
 * 1 : Exposed the function (if no hadware support, faked function willbe used)

##### LIBGL_VERSION
Hack: Control the glGetString version. Overide version string (should be in the form of "1.x")

##### LIBGL_BATCH
Experimental: Batch mode (fuse of contigous Display list, to limit Draw calls)
 * 0 : Default, only Draw list created during a glList are fused (i.e. contigous glBegin / glEnd inside a List)
 * 1 : Force a maximum of call to be batched (like if all was inside a big glList)
 * 2 : Disable Batch mode completly, no fuse of draw list
 
##### LIBGL_NOERROR
Hack: glGetError() always return GL_NOERROR
 * 0 : Default, glGetError behave as it should
 * 1 : glGetError never fail.

##### LIBGL_GAMMA
Pandora Hack: Set a Gamma value (in decimal formal, 1.0 means no gamma boost)
 * X.Y : Use X.Y as gamma when creating context (typical value can be 1.6 or 2.0)

##### LIBGL_SRGB
ODROID Hack: Enable sRGB Surface (so Gamma corrected), if Hardware support it
 * 0 : Default, don't try to use sRGB surface
 * 1 : Enable sRGB Surface (but support will be tested first, must have EGL_KHR_gl_colorspace extension)

##### LIBGL_FASTMATH
Hack: Activate some Fast Math in processor/coprocessor
 * 0 : Default, nothing special
 * 1 : On OpenPandora, activate "RunFast" on Cortex-A8 (mode default NaN, flush-to-zero)
     : Not implemented on other platforms (will do nothing)

##### LIBGL_SILENTSTUB
Debug: Hide or Show the Sub / Not found message
 * 0 : Default, the messages are shown
 * 1 : Silent, don't print the STUB or glXGetProcAddress glXXXXX not found message

##### LIBGL_NOBANNER
Show/Hide initial text
 * 0 : Default, print starting message
 * 1 : Silent: no LIBGL message at start (combile with LIBGL_SILENTSTUB for more silence)

##### LIBGL_NPOT
Expose NPOT (Non Power of Two) Support
 * 0 : Default, don't expose the extension (but npot are still supported)
 * 1 : Expose limited NPOT extension
 * 2 : Expose GL_ARB_texture_non_power_of_two extension

##### LIBGL_GLQUERIES
Expose glQueries functions
 * 0 : Don't expose the function (fake one will be used if called)
 * 1 : Default, expose fake functions (always answer 0)

##### LIBGL_NOTEXMAT
Handling of Texture Matrix
 * 0 : Default, perform handling internaly (better handling of NPOT texture on all hardware)
 * 1 : Let the driver handle texmat (can be faster in some cases, but NPOT texture may be broken)

##### LIBGL_NOTEST
Initial Hardware test
 * 0 : Default, perform intial hardware testing (using a PBuffer)
 * 1 : Do not perform test (no extensions tested or used)

##### LIBGL_NOVAOCACHE
VAO Caching
 * 0 : Default, try to cache vao to avoid memcpy in render list
 * 1 : Don't cache VAO

##### LIBGL_VABGRA
Vertex Array BGRA extension
 * 0 : Default, GL_ARB_vertex_array_bgra not exposed (still emulated)
 * 1 : Extension exposed may be faster in some cases (Arx Libertatis mainly)

##### LIBGL_BEGINEND
Merge of subsequent glBegin/glEnd blocks (will be non-effective if BATCH mode is used)
 * 0 : Don't try to merge
 * 1 : Try to merge, even if there is a glColor / glNormal in between (default)
 * 2 : Try hard to merge, even if there is a glColor / glNormal or Matrix operations in between

##### LIBGL_AVOID16BITS
Try to avoid 16bits textures
 * 0 : Default, use 16bits texture if it can avoid a convertion or for DXTc textures
 * 1 : Use 32bits texture unless specifically requested (using internalformat)
