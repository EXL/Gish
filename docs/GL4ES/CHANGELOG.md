Version history
----
##### Current version
 * Added ES2 backen infrastructure

##### 1.0.0
 * Removed old ES2 defines (ES2 will be dynamic later)

##### 0.9.8
 * Added TravisCI build on github
 * Added some optimisation when to discard call to glBindTexture if useless (same texture) between 2 glBegin/glEnd blocks
 * Stubbed glGet with GL_DRAW_BUFFER
 * Improvement to Raster operations
 * Factorised Blit function (only 1 function for that now)
 * Tracking ShadeModel
 * Tracking TexEnv
 
##### 0.9.7
 * Tracking Clip Planes
 * Refactor Blitting function, and use glDrawTex extension if present
 * Restructured README and split in several files
 * Some improvments and fixes to LIBGL_BEGINEND=2 mode
 * Some improvments to some LIBGL_SHRINK mode
 * Proper support for DOT3 extension
 * Some fixes to Read/Draw Buffer handling
 * Some fix with the PixMap glX context creation
 
##### 0.9.6
 * Some fixes in GL_TEXTURE_RECTANGLE_ARB handling
 * Some other fixes in texture handling (unpack and glList related)
 * Some fix with the PBuffer glX context creation
 * Tracking of glFog
 * Exposed glBlendEquation if supported
 * New LIBGL_AVOID16BITS parameter to prefer 32bits texture (usefull on ODroid)
 * Some optimisations in texture conversion

##### 0.9.5
 * Added some optimisations for sequencial glBegin/glEnd blocks, with a switch to control them
 * Fixed many issue with Lights introduced with 0.9.4
 * Fixed Android build introduced with 0.9.4

##### 0.9.4
 * Fixed some extended functions (like glBlendFuncSeparate) not working inside list (fixing some issues with Batch mode)
 * Added back GL_TEXTURE_RECTANGLE_ARB handling (and using npot texture, even limited, if available)
 * Added tracking of Lights and Materials
 * Fixed (Added in fact) support for Multisampling on the GLX Context creation (a bit hacky, but seems to works)
 * Added LIBGL_NODOWNSAMPLING and associated Hint
 * Try to implement some caching on VAO to avoid some memcpy in renderlist (with a way to disable it just in case)

##### 0.9.3
 * Added support for Cube Mapping (with hardware support)
 * Improved Texture state tracking
 * Added LIBGL_NOTEXMAT env. var. switch for Texture Matrix handling
 * Added GL_EXT_vertex_array_bgra (and NEONinzed some loop)
 * Finished GL_EXT_direct_state_access extension
 * Mangled glX function (to be abble to use apitrace to capture GL frames)
 * Return some values in glXQueryServerString, coherent with glXGetClientString

##### 0.9.2
 * All matrix are tracked now
 * Texture Matrix are 100% handled by gl4es. GLES Hardware keep an Identity matrix (TexCoord are transformed if needed). This allows a better handling of NPOT texture on hadware that doesn't support Full NPOT (fixed movies beiing horizontaly shifted in openmw with LIBGL_NPOT=1 for example)

##### 0.9.1
 * Added gl4es specifics glHint capabilities. If the extension GL_GL4ES_hint is present, than a few Hint are accessible. Look in include/gl4eshint.h for the list.

##### 0.9.0
 * New name: gl4es
