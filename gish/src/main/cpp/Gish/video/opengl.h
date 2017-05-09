#ifndef GISH_VIDEO_OPENGL_H
#define GISH_VIDEO_OPENGL_H
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
#ifdef MAC
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#endif

#ifdef WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// Yes... the pi is a lie.
#undef pi

#include <GL/gl.h>
#include "../video/glext.h"
#endif

#ifdef LINUX
#if defined(USE_GLES)
#include <GLES/gl.h>
#include "../egl/eglport.h"

#define GL_CLAMP GL_CLAMP_TO_EDGE
#define glFrustum glFrustumf
#define glOrtho glOrthof
#define glMultiTexCoord2fARB(tex,coord0,coord1) glMultiTexCoord4f(tex,coord0,coord1,0,1)
#define GL_TEXTURE0_ARB GL_TEXTURE0
#define GL_TEXTURE1_ARB GL_TEXTURE1
#define glActiveTextureARB glActiveTexture

#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif /* #if defined(USE_GLES) */
#endif

void loadglextentions(void);

#ifdef WINDOWS
extern PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
extern PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;
extern PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;
extern PFNGLMULTITEXCOORD2FVARBPROC glMultiTexCoord2fvARB;
extern PFNGLMULTITEXCOORD3FARBPROC glMultiTexCoord3fARB;
extern PFNGLMULTITEXCOORD4FARBPROC glMultiTexCoord4fARB;
#endif

typedef struct
{
	int compiled_vertex_array;
	int multitexture;
	int texture_env_dot3;
	int stencil_two_side;
	int fragment_program;
} _glext;

extern _glext glext;

#endif /* GISH_VIDEO_OPENGL_H */
