#ifdef ANDROID
#include "../gl/gl.h"
#endif
#include "glx.h"
#include "hardext.h"
#include "../gl/init.h"
#include "../gl/directstate.h"

#define EXPORT __attribute__((visibility("default")))

//#define DEBUG_ADDRESS

#ifdef DEBUG_ADDRESS
#define MAP(func_name, func) \
    if (strcmp(name, func_name) == 0) return (void *)func;
#else
#define MAP(func_name, func) \
    if (strcmp(name, func_name) == 0) return (void *)func;
#endif

#define MAP_EGL(func_name, egl_func) \
    MAP(#func_name, egl_eglGetProcAddress(#egl_func))

#define EX(func_name) MAP(#func_name, func_name)

#define ARB(func_name) MAP(#func_name "ARB", func_name)

#define EXT(func_name) MAP(#func_name "EXT", func_name)

#define _EX(func_name) MAP(#func_name, gl4es_ ## func_name)

#define _ARB(func_name) MAP(#func_name "ARB", gl4es_ ## func_name)

#define _EXT(func_name) MAP(#func_name "EXT", gl4es_ ## func_name)

#define STUB(func_name)                       \
    if (strcmp(name, #func_name) == 0) {      \
        if(!globals4es.silentstub) LOGD("glX stub: %s\n", #func_name); \
        return (void *)glXStub;               \
    }

void glXStub(void *x, ...) {
    return;
}

EXPORT void *gl4es_glXGetProcAddress(const char *name) {
    LOAD_EGL(eglGetProcAddress);
#ifdef DEBUG_ADDRESS
    static int cnt = 0;
    cnt++;
#endif
    // generated gles wrappers
#ifdef USE_ES2
    #include "gles2funcs.inc"
#else
    #include "glesfuncs.inc"
#endif

#ifndef ANDROID
    // glX calls
    _EX(glXChooseVisual);
    _EX(glXCopyContext);
    _EX(glXCreateContext);
    _EX(glXCreateNewContext);
    _EX(glXCreateContextAttribsARB);
    _EX(glXDestroyContext);
    _EX(glXGetConfig);
    _EX(glXGetCurrentDisplay);
    _EX(glXGetCurrentDrawable);
    _EX(glXIsDirect);
    _EX(glXMakeCurrent);
    _EX(glXMakeContextCurrent);
    _EX(glXQueryExtensionsString);
    _EX(glXQueryServerString);
    _EX(glXSwapBuffers);
    _EX(glXSwapIntervalEXT);
#endif //ANDROID
    MAP("glXSwapIntervalMESA", gl4es_glXSwapInterval);
    MAP("glXSwapIntervalSGI", gl4es_glXSwapInterval);
#ifndef ANDROID
    _EX(glXUseXFont);
    _EX(glXWaitGL);
    _EX(glXWaitX);
    _EX(glXGetCurrentContext);
    _EX(glXQueryExtension);
    _EX(glXQueryDrawable);
    _EX(glXQueryVersion);
    _EX(glXGetClientString);
    _EX(glXGetFBConfigs);
    _EX(glXChooseFBConfig);
    MAP("glXChooseFBConfigSGIX", gl4es_glXChooseFBConfig);
    _EX(glXGetFBConfigAttrib);
    _EX(glXQueryContext);
    _EX(glXGetVisualFromFBConfig);
    _EX(glXCreateWindow);
    _EX(glXDestroyWindow);
    
    _EX(glXCreatePbuffer);
    _EX(glXDestroyPbuffer);
    _EX(glXCreatePixmap);
    _EX(glXDestroyPixmap);
    _EX(glXCreateGLXPixmap);
    _EX(glXDestroyGLXPixmap);
    STUB(glXGetCurrentReadDrawable);
    STUB(glXGetSelectedEvent);
    STUB(glXSelectEvent);
#endif //ANDROID
    _EX(glXGetProcAddress);
    _ARB(glXGetProcAddress);
    // GL_EXT_texture_object (yeah, super old!)
    _EXT(glGenTextures);
    _EXT(glBindTexture);
    _EXT(glDeleteTextures);
    _EXT(glIsTexture);
    _EXT(glAreTexturesResident);
    _EXT(glPrioritizeTextures);

    // GL_EXT_polygonoffset
    _EXT(glPolygonOffset);

    // GL_ARB_vertex_buffer_object
    _ARB(glBindBuffer);
    _ARB(glBufferData);
    _ARB(glBufferSubData);
    _ARB(glDeleteBuffers);
    _ARB(glGenBuffers);
    _ARB(glIsBuffer);
    _EX(glGetBufferPointerv);
    _ARB(glGetBufferPointerv);
    _EX(glMapBuffer);
    _EX(glUnmapBuffer);
    _ARB(glMapBuffer);
    _ARB(glUnmapBuffer);
    _ARB(glGetBufferParameteriv);
    _EX(glGetBufferSubData);
    _ARB(glGetBufferSubData);

    // GL_ARB_vertex_array_object
    _EX(glGenVertexArrays);
    _EX(glBindVertexArray);
    _EX(glDeleteVertexArrays);
    _EX(glIsVertexArray);
    _ARB(glGenVertexArrays);
    _ARB(glBindVertexArray);
    _ARB(glDeleteVertexArrays);
    _ARB(glIsVertexArray);
    _EXT(glGenVertexArrays);
    _EXT(glBindVertexArray);
    _EXT(glDeleteVertexArrays);
    _EXT(glIsVertexArray);
    
    // GL_ARB_frameBuffer_ext
    if(hardext.fbo) {
        _EX(glFramebufferTexture1D);
        _EX(glFramebufferTexture3D);
        _EX(glFramebufferTextureLayer);
        _EX(glRenderbufferStorageMultisample);
        _EX(glBlitFramebuffer);
        _EXT(glGenFramebuffers);
        _EXT(glDeleteFramebuffers);
        _EXT(glIsFramebuffer);
        _EXT(glCheckFramebufferStatus);
        _EXT(glBindFramebuffer);
        _EXT(glFramebufferTexture2D);
        _EXT(glFramebufferTexture1D);
        _EXT(glFramebufferTexture3D);
        _EXT(glGenRenderbuffers);
        _EXT(glFramebufferRenderbuffer);
        _EXT(glDeleteRenderbuffers);
        _EXT(glRenderbufferStorage);
        _EXT(glRenderbufferStorageMultisample);
        _EXT(glBindRenderbuffer);
        _EXT(glIsRenderbuffer);
        _EXT(glGenerateMipmap);
        _EXT(glGetFramebufferAttachmentParameteriv);
        _EXT(glGetRenderbufferParameteriv);
        _EXT(glFramebufferTextureLayer);
        _EXT(glBlitFramebuffer);
        _ARB(glGenFramebuffers);
        _ARB(glDeleteFramebuffers);
        _ARB(glIsFramebuffer);
        _ARB(glCheckFramebufferStatus);
        _ARB(glBindFramebuffer);
        _ARB(glFramebufferTexture2D);
        _ARB(glFramebufferTexture1D);
        _ARB(glFramebufferTexture3D);
        _ARB(glGenRenderbuffers);
        _ARB(glFramebufferRenderbuffer);
        _ARB(glDeleteRenderbuffers);
        _ARB(glRenderbufferStorage);
        _ARB(glRenderbufferStorageMultisample);
        _ARB(glBindRenderbuffer);
        _ARB(glIsRenderbuffer);
        _ARB(glGenerateMipmap);
        _ARB(glGetFramebufferAttachmentParameteriv);
        _ARB(glGetRenderbufferParameteriv);
        _ARB(glFramebufferTextureLayer);
        _ARB(glBlitFramebuffer);
        STUB(glDrawBuffersARB);
    }
    
    // GL_EXT_vertex_array
    _EXT(glArrayElement);
    _EXT(glDrawArrays);
    _EXT(glVertexPointer);
    _EXT(glNormalPointer);
    _EXT(glColorPointer);
    _EX(glIndexPointer);    //TODO, stub for now
    _EXT(glIndexPointer);
    _EXT(glTexCoordPointer);
    _EX(glEdgeFlagPointer); //TODO, stub for now
    _EXT(glEdgeFlagPointer);
    _EX(glGetPointerv);
    _EXT(glGetPointerv);


    // OES wrapper
    EX(glClearDepthfOES);
    EX(glClipPlanefOES);
    EX(glDepthRangefOES);
    EX(glFrustumfOES);
    EX(glGetClipPlanefOES);
    EX(glOrthofOES);

    // passthrough
    // batch thunking!
    #define THUNK(suffix, type)       \
    _EX(glColor3##suffix##v);          \
    _EX(glColor3##suffix);             \
    _EX(glColor4##suffix##v);          \
    _EX(glColor4##suffix);             \
    _EX(glSecondaryColor3##suffix##v); \
    _EX(glSecondaryColor3##suffix);    \
    _EXT(glSecondaryColor3##suffix##v); \
    _EXT(glSecondaryColor3##suffix);    \
    _EX(glIndex##suffix##v);           \
    _EX(glIndex##suffix);              \
    _EX(glNormal3##suffix##v);         \
    _EX(glNormal3##suffix);            \
    _EX(glRasterPos2##suffix##v);      \
    _EX(glRasterPos2##suffix);         \
    _EX(glRasterPos3##suffix##v);      \
    _EX(glRasterPos3##suffix);         \
    _EX(glRasterPos4##suffix##v);      \
    _EX(glRasterPos4##suffix);         \
    _EX(glWindowPos2##suffix##v);      \
    _EX(glWindowPos2##suffix);         \
    _EX(glWindowPos3##suffix##v);      \
    _EX(glWindowPos3##suffix);         \
    _EX(glVertex2##suffix##v);         \
    _EX(glVertex2##suffix);            \
    _EX(glVertex3##suffix##v);         \
    _EX(glVertex3##suffix);            \
    _EX(glVertex4##suffix##v);         \
    _EX(glVertex4##suffix);            \
    _EX(glTexCoord1##suffix##v);       \
    _EX(glTexCoord1##suffix);          \
    _EX(glTexCoord2##suffix##v);       \
    _EX(glTexCoord2##suffix);          \
    _EX(glTexCoord3##suffix##v);       \
    _EX(glTexCoord3##suffix);          \
    _EX(glTexCoord4##suffix##v);       \
    _EX(glTexCoord4##suffix);          \
    _EX(glMultiTexCoord1##suffix##v);  \
    _EX(glMultiTexCoord1##suffix);     \
    _EX(glMultiTexCoord2##suffix##v);  \
    _EX(glMultiTexCoord2##suffix);     \
    _EX(glMultiTexCoord3##suffix##v);  \
    _EX(glMultiTexCoord3##suffix);     \
    _EX(glMultiTexCoord4##suffix##v);  \
    _EX(glMultiTexCoord4##suffix);     \
    _EXT(glMultiTexCoord1##suffix##v); \
    _EXT(glMultiTexCoord1##suffix);    \
    _EXT(glMultiTexCoord2##suffix##v); \
    _EXT(glMultiTexCoord2##suffix);    \
    _EXT(glMultiTexCoord3##suffix##v); \
    _EXT(glMultiTexCoord3##suffix);    \
    _EXT(glMultiTexCoord4##suffix##v); \
    _EXT(glMultiTexCoord4##suffix);    \
    _ARB(glMultiTexCoord1##suffix##v); \
    _ARB(glMultiTexCoord1##suffix);    \
    _ARB(glMultiTexCoord2##suffix##v); \
    _ARB(glMultiTexCoord2##suffix);    \
    _ARB(glMultiTexCoord3##suffix##v); \
    _ARB(glMultiTexCoord3##suffix);    \
    _ARB(glMultiTexCoord4##suffix##v); \
    _ARB(glMultiTexCoord4##suffix);

    THUNK(b, GLbyte);
    THUNK(d, GLdouble);
    THUNK(i, GLint);
    THUNK(s, GLshort);
    THUNK(ub, GLubyte);
    THUNK(ui, GLuint);
    THUNK(us, GLushort);
    THUNK(f, GLfloat);
    #undef THUNK
    
    _EX(glPointParameterf);
    _EX(glPointParameterfv);
    _ARB(glPointParameterf);
    _ARB(glPointParameterfv);
    _EXT(glPointParameterf);
    _EXT(glPointParameterfv);

#ifdef USE_ES2
    _EX(glCompileShaderARB);
    _EX(glCreateShaderObjectARB);
    _EX(glGetObjectParameterivARB);
    _EX(glShaderSourceARB);
#endif

    // functions we actually define
    _EXT(glActiveTexture);
    _ARB(glActiveTexture);
    _EX(glArrayElement);
    _EX(glBegin);
    _EX(glBitmap);
    if(globals4es.blendcolor || hardext.blendcolor) {
        _EX(glBlendColor);
        _EXT(glBlendColor);
        _ARB(glBlendColor);
    }
    _EXT(glBlendEquation);
    _ARB(glBlendEquation);
    _EXT(glBlendFunc);
    _ARB(glBlendFunc);

    if(hardext.blendeq) {
        _EXT(glBlendEquationSeparate);
        _ARB(glBlendEquationSeparate);
        _EX(glBlendEquationSeparatei);
        _EXT(glBlendEquationSeparatei);
        _ARB(glBlendEquationSeparatei);
    }
    if(hardext.blendfunc) {
        _EXT(glBlendFuncSeparate);
        _ARB(glBlendFuncSeparate);
        _EX(glBlendFuncSeparatei);
        _EXT(glBlendFuncSeparatei);
        _ARB(glBlendFuncSeparatei);
    }
    _EX(glStencilMaskSeparate);
    _EXT(glStencilMaskSeparate);
    _EX(glCallList);
    _EX(glCallLists);
    _EX(glClearDepth);
    _EXT(glClientActiveTexture);
    _ARB(glClientActiveTexture);
    _EX(glClipPlane);
    _EX(glCopyPixels);
    _EX(glDeleteLists);
    _EX(glDepthRange);
    _EX(glDrawBuffer);
    _EX(glDrawPixels);
    _EX(glDrawRangeElements);
    _EXT(glDrawRangeElements);
    _EX(glEdgeFlag);
    _EX(glEnd);
    _EX(glEndList);
    _EX(glEvalCoord1d);
    _EX(glEvalCoord1f);
    _EX(glEvalCoord2d);
    _EX(glEvalCoord2f);
    _EX(glEvalCoord1dv);
    _EX(glEvalCoord1fv);
    _EX(glEvalCoord2dv);
    _EX(glEvalCoord2fv);
    _EX(glEvalMesh1);
    _EX(glEvalMesh2);
    _EX(glEvalPoint1);
    _EX(glEvalPoint2);
    _EX(glFogCoordd);
    _EX(glFogCoorddv);
    _EX(glFogCoordf);
    _EX(glFogCoordfv);
    _EX(glFogi);
    _EX(glFogiv);
    _EX(glFrustum);
    _EX(glGenLists);
    _EX(glGetDoublev);
    _EX(glGetIntegerv);
    _EX(glGetMapdv);
    _EX(glGetMapfv);
    _EX(glGetMapiv);
    _EX(glGetTexImage);
    _EX(glGetTexLevelParameterfv);
    _EX(glGetTexLevelParameteriv);
    _EX(glInitNames);
    _EX(glInterleavedArrays);
    _EX(glIsList);
#ifndef USE_ES2
    _EX(glLighti);
    _EX(glLightiv);
    _EX(glLightModeli);
    _EX(glLightModeliv);
#endif
    _EX(glLineStipple);
    _EX(glListBase);
    _EX(glLoadMatrixd);
    _EX(glLoadName);
    _EXT(glLockArrays);
    _EX(glMap1d);
    _EX(glMap1f);
    _EX(glMap2d);
    _EX(glMap2f);
    _EX(glMapGrid1d);
    _EX(glMapGrid1f);
    _EX(glMapGrid2d);
    _EX(glMapGrid2f);
    _EX(glMateriali);
    _EX(glMaterialiv);
    _EX(glMultMatrixd);
    _EX(glNewList);
    _EX(glOrtho);
    _EX(glPixelTransferf);
    _EX(glPixelTransferi);
    _EX(glPixelZoom);
    _EX(glPolygonMode);
    _EX(glPolygonStipple);
    _EX(glPopAttrib);
    _EX(glPopClientAttrib);
    _EX(glPopName);
    _EX(glPushAttrib);
    _EX(glPushClientAttrib);
    _EX(glPushName);
    _EX(glRasterPos2i);
    _EX(glReadBuffer);
    _EX(glRectd);
    _EX(glRectf);
    _EX(glRecti);
    _EX(glRects);
    _EX(glRectdv);
    _EX(glRectfv);
    _EX(glRectiv);
    _EX(glRectsv);
    _EX(glRenderMode);
    _EX(glRotated);
    _EX(glScaled);
    _EX(glSecondaryColorPointer);
    _EXT(glSecondaryColorPointer);
    _EX(glTexEnvf);
    _EX(glTexEnviv);
    _EX(glTexGend);
    _EX(glTexGendv);
    _EX(glTexGenf);
    _EX(glTexGenfv);
    _EX(glTexGeni);
    _EX(glTexGeniv);
    _EX(glTexImage1D);
    _EX(glTexImage3D);
    _EX(glTexSubImage1D);
    _EX(glTexSubImage3D);
    _EXT(glTexImage3D);
    _EXT(glTexSubImage3D);
    _EX(glCompressedTexImage1D);
    _EX(glCompressedTexSubImage1D);
    _EX(glCompressedTexImage3D);
    _EX(glCompressedTexSubImage3D);
    _EX(glGetCompressedTexImage);
    _EXT(glCompressedTexImage2D);
    _EXT(glCompressedTexSubImage2D);
    _EXT(glCompressedTexImage1D);
    _EXT(glCompressedTexSubImage1D);
    _EXT(glCompressedTexImage3D);
    _EXT(glCompressedTexSubImage3D);
    _EXT(glGetCompressedTexImage);
    _ARB(glCompressedTexImage2D);
    _ARB(glCompressedTexSubImage2D);
    _ARB(glCompressedTexImage1D);
    _ARB(glCompressedTexSubImage1D);
    _ARB(glCompressedTexImage3D);
    _ARB(glCompressedTexSubImage3D);
    _ARB(glGetCompressedTexImage);
    _EX(glCopyTexImage1D);
    _EX(glCopyTexSubImage1D);
    _EX(glTranslated);
    _EXT(glUnlockArrays);
    _EX(glGetTexGenfv);
    _EX(glGetTexGendv);
    _EX(glGetTexGeniv);
    _EX(glLoadTransposeMatrixf);
    _EX(glLoadTransposeMatrixd);
    _EX(glMultTransposeMatrixd);
    _EX(glMultTransposeMatrixf);
    // stubs for unimplemented functions
    STUB(glAccum);
    STUB(glAreTexturesResident);
    STUB(glClearAccum);
    _EX(glColorMaterial);
    _EX(glCopyTexSubImage3D);   // It's a stub, calling the 2D one
    STUB(glFeedbackBuffer);
    STUB(glGetClipPlane);
    _EX(glGetLightiv);
    _EX(glGetMaterialiv);
    STUB(glGetPixelMapfv);
    STUB(glGetPixelMapuiv);
    STUB(glGetPixelMapusv);
    STUB(glGetPolygonStipple);
    STUB(glGetStringi);
    STUB(glPassThrough);
    STUB(glPixelMapfv);
    STUB(glPixelMapuiv);
    STUB(glPixelMapusv);
    _EX(glPixelStoref);
    STUB(glPrioritizeTextures);
    STUB(glSelectBuffer);   //TODO

    _EX(glMultiDrawArrays);
    _EXT(glMultiDrawArrays);
    _EX(glMultiDrawElements);
    _EXT(glMultiDrawElements);

    _EX(glPointParameteri);
    _EX(glPointParameteriv);
    
    STUB(glFogCoordPointer);
    /*STUB(glEdgeFlagPointerEXT);
    STUB(glIndexPointerEXT);*/
    
    //EXT_direct_state_access
    _EX(glClientAttribDefault);
    _EX(glPushClientAttribDefault);
    _EX(glMatrixLoadf);
    _EX(glMatrixLoadd);
    _EX(glMatrixMultf);
    _EX(glMatrixMultd);
    _EX(glMatrixLoadIdentity);
    _EX(glMatrixRotatef);
    _EX(glMatrixRotated);
    _EX(glMatrixScalef);
    _EX(glMatrixScaled);
    _EX(glMatrixTranslatef);
    _EX(glMatrixTranslated);
    _EX(glMatrixOrtho);
    _EX(glMatrixFrustum);
    _EX(glMatrixPush);
    _EX(glMatrixPop);
    _EX(glTextureParameteri);
    _EX(glTextureParameteriv);
    _EX(glTextureParameterf);
    _EX(glTextureParameterfv);
    _EX(glTextureImage1D);
    _EX(glTextureImage2D);
    _EX(glTextureSubImage1D);
    _EX(glTextureSubImage2D);
    _EX(glCopyTextureImage1D);
    _EX(glCopyTextureImage2D);
    _EX(glCopyTextureSubImage1D);
    _EX(glCopyTextureSubImage2D);
    _EX(glGetTextureImage);
    _EX(glGetTextureParameterfv);
    _EX(glGetTextureParameteriv);
    _EX(glGetTextureLevelParameterfv);
    _EX(glGetTextureLevelParameteriv);
    _EX(glTextureImage3D);
    _EX(glTextureSubImage3D);
    _EX(glCopyTextureSubImage3D);
    _EX(glBindMultiTexture);
    _EX(glMultiTexCoordPointer);
    _EX(glMultiTexEnvf);
    _EX(glMultiTexEnvfv);
    _EX(glMultiTexEnvi);
    _EX(glMultiTexEnviv);
    _EX(glMultiTexGend);
    _EX(glMultiTexGendv);
    _EX(glMultiTexGenf);
    _EX(glMultiTexGenfv);
    _EX(glMultiTexGeni);
    _EX(glMultiTexGeniv);
    _EX(glGetMultiTexEnvfv);
    _EX(glGetMultiTexEnviv);
    _EX(glGetMultiTexGendv);
    _EX(glGetMultiTexGenfv);
    _EX(glGetMultiTexGeniv);
    _EX(glMultiTexParameteri);
    _EX(glMultiTexParameteriv);
    _EX(glMultiTexParameterf);
    _EX(glMultiTexParameterfv);
    _EX(glMultiTexImage1D);
    _EX(glMultiTexImage2D);
    _EX(glMultiTexSubImage1D);
    _EX(glMultiTexSubImage2D);
    _EX(glCopyMultiTexImage1D);
    _EX(glCopyMultiTexImage2D);
    _EX(glCopyMultiTexSubImage1D);
    _EX(glCopyMultiTexSubImage2D);
    _EX(glGetMultiTexImage);
    _EX(glGetMultiTexParameterfv);
    _EX(glGetMultiTexParameteriv);
    _EX(glGetMultiTexLevelParameterfv);
    _EX(glGetMultiTexLevelParameteriv);
    _EX(glMultiTexImage3D);
    _EX(glMultiTexSubImage3D);
    _EX(glCopyMultiTexSubImage3D);
    _EX(glEnableClientStateIndexedEXT);
    _EX(glDisableClientStateIndexedEXT);
    _EX(glGetFloatIndexedvEXT);
    _EX(glGetDoubleIndexedvEXT);
    _EX(glGetIntegerIndexedvEXT);
    _EX(glGetBooleanIndexedvEXT);
    _EX(glGetPointerIndexedvEXT);
    _EX(glEnableIndexedEXT);
    _EX(glDisableIndexedEXT);
    _EX(glIsEnabledIndexedEXT);
    _EX(glCompressedTextureImage3D);
    _EX(glCompressedTextureImage2D);
    _EX(glCompressedTextureImage1D);
    _EX(glCompressedTextureSubImage3D);
    _EX(glCompressedTextureSubImage2D);
    _EX(glCompressedTextureSubImage1D);
    _EX(glGetCompressedTextureImage);
    _EX(glCompressedMultiTexImage3D);
    _EX(glCompressedMultiTexImage2D);
    _EX(glCompressedMultiTexImage1D);
    _EX(glCompressedMultiTexSubImage3D);
    _EX(glCompressedMultiTexSubImage2D);
    _EX(glCompressedMultiTexSubImage1D);
    _EX(glGetCompressedMultiTexImage);
    _EX(glMatrixLoadTransposef);
    _EX(glMatrixLoadTransposed);
    _EX(glMatrixMultTransposef);
    _EX(glMatrixMultTransposed);

    if(globals4es.queries) {
        _EX(glGenQueries);
        _EX(glIsQuery);
        _EX(glDeleteQueries);
        _EX(glBeginQuery);
        _EX(glEndQuery);
        _EX(glGetQueryiv);
        _EX(glGetQueryObjectiv);
        _EX(glGetQueryObjectuiv);
    }

    // GL_ARB_multisample
    _ARB(glSampleCoverage);

    if (!globals4es.silentstub) LOGD("glXGetProcAddress: %s not found.\n", name);
    return NULL;
}

void glXGetProcAddress(const char* name) AliasExport("gl4es_glXGetProcAddress");
void glXGetProcAddressARB(const char* name) AliasExport("gl4es_glXGetProcAddress");
