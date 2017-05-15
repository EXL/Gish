/*
Copyright (C) 2005, 2010 - Cryptic Sea

This file is part of Gish.

Gish is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "config.h"

#include "sdl/sdl.h"

#include "video/opengl.h"

#include "audio/audio.h"
#include "game/config.h"
#include "game/high.h"
#include "game/mainmenu.h"
#include "game/player.h"
#include "game/options.h"
#include "input/joystick.h"
#include "menu/menu.h"
#include "sdl/endian.h"
#include "sdl/event.h"
#include "sdl/video.h"
#include "video/text.h"
#include "video/texture.h"

#ifdef ANDROID_NDK
#include "android/a_utils.h"
#include <stdlib.h>
#endif

#ifdef DATAPATH
#include <unistd.h>
#define DATAPATH_STR3(X) #X
#define DATAPATH_STR2(X) DATAPATH_STR3(X)
#define DATAPATH_STR DATAPATH_STR2(DATAPATH)
#endif

#ifdef GLES
EGLDisplay eglDisplay;
EGLConfig glConfig;
EGLContext eglContext;
EGLSurface eglSurface;
const char *gl_vendor, *gl_renderer, *gl_version, *gl_extensions;
EGLint attrib_list_fsaa[] = {
    EGL_RED_SIZE,                           5,
    EGL_GREEN_SIZE,                         6,
    EGL_BLUE_SIZE,                          5,
    EGL_DEPTH_SIZE,                        16,
    EGL_SURFACE_TYPE,          EGL_WINDOW_BIT,
    EGL_RENDERABLE_TYPE,    EGL_OPENGL_ES_BIT,
    EGL_SAMPLE_BUFFERS,                     1,
    EGL_SAMPLES,                            1,
    EGL_NONE
};

EGLint attrib_list[] = {
    EGL_RED_SIZE,                           5,
    EGL_GREEN_SIZE,                         6,
    EGL_BLUE_SIZE,                          5,
    EGL_DEPTH_SIZE,                        16,
    EGL_SURFACE_TYPE,          EGL_WINDOW_BIT,
    EGL_RENDERABLE_TYPE,    EGL_OPENGL_ES_BIT,
    EGL_SAMPLE_BUFFERS,                     1,
    EGL_SAMPLES,                            4,
    EGL_NONE
};
#endif

int main (int argc,char *argv[])
  {
  int count;
  int flags;
  const char *temp;

#ifdef DATAPATH
  chdir(DATAPATH_STR);
#endif

#ifdef ANDROID_NDK
  initializeJavaEnviron();
#endif

  checkbigendian();

  loadconfig();
  loadscores();
  loadplayers();

  flags=SDL_INIT_VIDEO|SDL_INIT_TIMER;
  if (config.joystick)
    flags|=SDL_INIT_JOYSTICK;

  SDL_Init(flags);

  int display_count = 0, display_index = 0, mode_index = 0;
  SDL_DisplayMode mode = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };

  if ((display_count = SDL_GetNumVideoDisplays()) < 1)
    {
    TO_DEBUG_LOG("SDL_GetNumVideoDisplays returned: %i\n", display_count);
    return 1;
    }

  if (SDL_GetDisplayMode(display_index, mode_index, &mode) != 0)
    {
    TO_DEBUG_LOG("SDL_GetDisplayMode failed: %s\n", SDL_GetError());
    return 1;
    }

  TO_DEBUG_LOG("SDL_GetDisplayMode(0, 0, &mode): %i bpp %ix%i\n", SDL_BITSPERPIXEL(mode.format), mode.w, mode.h);

  if (SDL_BITSPERPIXEL(mode.format)==16)
    config.bitsperpixel=16;

  for (count=1;count<argc;count++)
    {
    if (strcmp("-nosound",argv[count])==0)
      {
      config.sound=0;
      option.sound=0;
      option.music=0;
      }
    if (strcmp("-sound",argv[count])==0)
      config.sound=1;
    if (strcmp("-nomusic",argv[count])==0)
      option.music=0;
    }

#ifndef ANDROID_NDK
  saveconfig();
#endif

  SDL_ShowCursor(SDL_DISABLE);

  listvideomodes();

#if !defined(PC_GLES) && !defined(ANDROID_NDK)
  if (windowinfo.bitsperpixel==16)
    {
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,6);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,5);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,0);
    }
  if (windowinfo.bitsperpixel==32)
    {
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,8);
    }
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,windowinfo.depthbits);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,windowinfo.stencilbits);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
#endif

#ifdef ANDROID_NDK
    // EXL: Android OpenGLES 1.1 via SDL2 initialization
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 0);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1); // TODO: Check this.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#endif

    printf( "Main.c Opening screen %dx%dx%d\n", windowinfo.resolutionx, windowinfo.resolutiony, windowinfo.bitsperpixel );

#if defined(PC_GLES)
    EGLint egl_config_attr[] = {
        EGL_BUFFER_SIZE,                16,
        EGL_DEPTH_SIZE,                 16,
        EGL_STENCIL_SIZE,                0,
        EGL_SURFACE_TYPE,   EGL_WINDOW_BIT,
        EGL_NONE
    };
    EGLint numConfigs, majorVersion, minorVersion;
    globalwindow = SDL_CreateWindow("Gish GLES", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    windowinfo.resolutionx, windowinfo.resolutiony,
                                    (windowinfo.fullscreen) ? (SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN) : SDL_WINDOW_OPENGL);
    eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(eglDisplay, &majorVersion, &minorVersion);
    eglChooseConfig(eglDisplay, egl_config_attr, &glConfig, 1, &numConfigs);
    SDL_SysWMinfo sysInfo;
    SDL_VERSION(&sysInfo.version); // Set SDL version
    SDL_GetWindowWMInfo(globalwindow, &sysInfo);
    eglContext = eglCreateContext(eglDisplay, glConfig, EGL_NO_CONTEXT, NULL);
    eglSurface = eglCreateWindowSurface(eglDisplay, glConfig, (EGLNativeWindowType) sysInfo.info.x11.window, 0);
    eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
    eglSwapInterval(eglDisplay, 1); // VSYNC
#else
    globalwindow = SDL_CreateWindow("Gish", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    windowinfo.resolutionx, windowinfo.resolutiony,
                                    (windowinfo.fullscreen) ? (SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN) : SDL_WINDOW_OPENGL);
    SDL_SetWindowDisplayMode(globalwindow, &mode);
#ifdef ANDROID_NDK
    SDL_SetWindowFullscreen(globalwindow, SDL_WINDOW_FULLSCREEN);
    SDL_GetWindowSize(globalwindow, &windowinfo.resolutionx, &windowinfo.resolutiony);
    TO_DEBUG_LOG("Resize SDL window: %dx%d\n", windowinfo.resolutionx, windowinfo.resolutiony);
    windowinfo.fullscreen = 1;
    saveconfig();
#endif
#endif

    if(globalwindow == NULL)
    {
        printf( "No SDL screen\n" );
    }

#ifndef ANDROID_NDK
    windowicon = SDL_LoadBMP("gish.bmp");
#else
    const char *iconpath = stringconcat(getAssetsPathFromJNI(), "gish.bmp");
    windowicon = SDL_LoadBMP(iconpath);
    free(iconpath);
#endif
    SDL_SetColorKey(windowicon, SDL_TRUE, SDL_MapRGB(windowicon->format, 255, 255, 255));
    SDL_SetWindowIcon(globalwindow, windowicon);

    SDL_WarpMouseInWindow(globalwindow, windowinfo.resolutionx / 2, windowinfo.resolutiony / 2);

    glcontext = SDL_GL_CreateContext(globalwindow);

  loadglextentions();

  for (count=0;count<2048;count++)
    glGenTextures(1,&texture[count].glname);

  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  if (config.joystick)
    {
    numofjoysticks=SDL_NumJoysticks();
    for (count=0;count<numofjoysticks;count++)
      {
      joy[count]=SDL_JoystickOpen(count);
      temp=SDL_JoystickName(joy[count]);
      strcpy(joystick[count].name,temp);
      joystick[count].numofbuttons=SDL_JoystickNumButtons(joy[count]);
      joystick[count].numofhats=SDL_JoystickNumHats(joy[count]);
      }

    SDL_JoystickEventState(SDL_IGNORE);
    }

  font.texturenum=0;
  font.cursornum=0;
  font.sizex=640;
  font.sizey=480;

  loadtexturetga(1000,"font00.tga",0,GL_CLAMP,GL_CLAMP,GL_LINEAR,GL_LINEAR);
  loadtexturetga(1001,"font01.tga",0,GL_CLAMP,GL_CLAMP,GL_LINEAR,GL_LINEAR);

  loadtexturetga(768,"mouse00.tga",0,GL_CLAMP,GL_CLAMP,GL_LINEAR,GL_LINEAR);
  loadtexturetga(769,"mouse00.tga",0,GL_CLAMP,GL_CLAMP,GL_LINEAR,GL_LINEAR);

  setupmenuitems();

  if (!glext.multitexture)
    {
    notsupportedmenu();

    SDL_MinimizeWindow(globalwindow);
    SDL_Quit();
    return(0);
    }
    
  if (config.sound)
    setupaudio();

  mainmenu();

  saveconfig();
  savescores();
  saveplayers();

  if (config.sound)
    shutdownaudio();

#if defined(PC_GLES)
    eglDestroySurface(eglDisplay, eglSurface);
    eglDestroyContext(eglDisplay, eglContext);
    eglTerminate(eglDisplay);
#else
  SDL_MinimizeWindow(globalwindow);
  SDL_GL_DeleteContext(glcontext);
  glcontext = NULL;
  SDL_DestroyWindow(globalwindow);
  globalwindow = NULL;
  SDL_FreeSurface(windowicon);
  windowicon = NULL;
#endif
  SDL_Quit();
#ifdef ANDROID_NDK
  exit(0);
#endif

  return(0);
  }
