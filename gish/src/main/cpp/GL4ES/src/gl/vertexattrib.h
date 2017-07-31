#include "gl.h"

#ifndef __VERTEXATTRIB_H_
#define __VERTEXATTRIB_H_

// actual definition of vertexattrib_t is in buffer.h, has they are part of VAO...

void gl4es_glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);
void gl4es_glEnableVertexAttribArray(GLuint index);
void gl4es_glDisableVertexAttribArray(GLuint index);

// other variant are defined in wrap...
void gl4es_glVertexAttrib4f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
void gl4es_glVertexAttrib4fv(GLuint index, const GLfloat *v);


void gl4es_glGetVertexAttribdv(GLuint index, GLenum pname, GLdouble *params);
void gl4es_glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat *params);
void gl4es_glGetVertexAttribiv(GLuint index, GLenum pname, GLint *params);
void gl4es_glGetVertexAttribPointerv(GLuint index, GLenum pname, GLvoid **pointer);

#endif