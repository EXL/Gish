gl4es
====

![gl4es build status](https://api.travis-ci.org/ptitSeb/gl4es.png "gl4es build status")

This is a library providing OpenGL 1.x functionality for OpenGL ES accelerated hardware.

This is a fork a glshim (https://github.com/lunixbochs/glshim). Go check this lib if you need things like RemoteGL or TinyGLES (for 100% software rendering).

The focus is on compatibility and speed with a wide selection of game and software.

It has been tested successfully of a large selection of games and software, including: Minecraft, OpenMW, SeriousSam (both First and Second Encounters), RVGL (ReVolt GL), TSMC (The Secret Maryo Chronicles), TORCS, SpeedDreams, GL-117, Foobillard(plus), Blender 2.68 to name just a few.

Most function of OpenGL up to 1.5 are supported, with some notable exceptions:
 * Reading of Depth or Stencil buffer will not work
 * GL_FEEDBACK mode is not implemented
 
Some know limitations:
 * GL_SELECT as some limitation in its implementation (for exemple, current Depth buffer or binded texture are not taken into account)
 * NPOT texture are supported, but not with GL_REPEAT / GL_MIRRORED, only GL_CLAMP will work properly (unless the GLES Hardware support NPOT)
 * Framebuffer use FRAMEBUFFER_OES extension (that must be present in the GLES 1.1 stack)
 * Multiple Color attachment on Framebuffer are not supported
 * OcclusionQuery is implemented, but with a 0 bits precision
 * Probably many other things

----

Compiling
----
How to compile and per-platform specific comment can be found [here](COMPILE.md)

----

GLU
----

Standard GLU do works without any issues.

----

Installation
----

Put lib/libGL.so.1 in your `LD_LIBRARY_PATH`.
Beware that GL4ES is meant to replace any libGL you can have on your system (like Mesa for example)

----

Usage
----

There are many environnement variable to control gl4es behavour, also usable at runtime using `glHint(...)`.

See [here](USAGE.md) for all variables and what they do.

----

Version history
----

The change log is [here](CHANGELOG.md)
