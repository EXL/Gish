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

#include "../config.h"

#include "../video/opengl.h"

#include <stdio.h>
#include <string.h>

#include "../video/text.h"
#include "../video/texture.h"
#include "../sdl/event.h"

char textstring2[1024];
char textstring3[1024];
_font font;

void drawtext(char *textstring,int x,int y,int textsize,float red,float green,float blue,float alpha,...)
  {
  int count,count2;
  int variabletemp;
  //int texturenum;
  va_list ap;
  float vec[3];
  float texcoord[2];

  count=0;
  count2=0;

  va_start(ap,alpha);
  while (textstring[count]!=0 && count2<1000)
    {
    if (textstring[count]=='/')
      {
      switch (textstring[count+1])
        {
        case '/':
          count++;
          textstring2[count2++]='/';
          break;

        case 'i':
          count++;
          sprintf(textstring3,"%d",(int)va_arg(ap,int));
          memcpy(textstring2+count2,textstring3,strlen(textstring3));
          count2+=strlen(textstring3);
          break;

        case 'I':
          count++;
          variabletemp=(int)va_arg(ap,int);
          if (variabletemp>=1000000)
            {
            sprintf(textstring3,"%d",(variabletemp/1000000));
            memcpy(textstring2+count2,textstring3,strlen(textstring3));
            count2+=strlen(textstring3);
            textstring2[count2++]=',';
            }
          if (variabletemp>=1000)
            {
            if (variabletemp>=1000000)
              sprintf(textstring3,"%.3d",((variabletemp/1000)%1000));
            else
              sprintf(textstring3,"%d",((variabletemp/1000)%1000));
            memcpy(textstring2+count2,textstring3,strlen(textstring3));
            count2+=strlen(textstring3);
            textstring2[count2++]=',';
            }
          if (variabletemp>=1000)
            sprintf(textstring3,"%.3d",(variabletemp%1000));
          else
            sprintf(textstring3,"%d",variabletemp);
          memcpy(textstring2+count2,textstring3,strlen(textstring3));
          count2+=strlen(textstring3);
          break;

        case 'u':
          count++;
          sprintf(textstring3,"%u",(unsigned int)va_arg(ap,unsigned int));
          memcpy(textstring2+count2,textstring3,strlen(textstring3));
          count2+=strlen(textstring3);
          break;

        case 'f':
          count++;
          sprintf(textstring3,"%f",(double)va_arg(ap,double));
          memcpy(textstring2+count2,textstring3,strlen(textstring3));
          count2+=strlen(textstring3);
          break;

        case 'c':
          count++;
          variabletemp=va_arg(ap,int);
          if (variabletemp!=0)
            textstring2[count2++]=variabletemp;
          break;

        case 's':
          count++;
          sprintf(textstring3,"%s",va_arg(ap,char *));
          memcpy(textstring2+count2,textstring3,strlen(textstring3));
          count2+=strlen(textstring3);
          break;

        case 'x':
          count++;
          sprintf(textstring3,"%X",(unsigned int)va_arg(ap,unsigned int));
          memcpy(textstring2+count2,textstring3,strlen(textstring3));
          count2+=strlen(textstring3);
          break;
        }
      }
    else
      textstring2[count2++]=textstring[count];

    count++;
    }
  va_end(ap);

  textstring2[count2]=0;

  if ((x&TEXT_CENTER)==TEXT_CENTER)
    x=x-(count2*textsize)/2;
  if ((y&TEXT_CENTER)==TEXT_CENTER)
    y=y-(textsize)/2;
  if ((x&TEXT_END)==TEXT_END)
    x=x-(count2*textsize);
  if ((y&TEXT_END)==TEXT_END)
    y=y-textsize;
  x&=0xFFFF;
  y&=0xFFFF;

  glBindTexture(GL_TEXTURE_2D,texture[1000+font.texturenum].glname);

  count=0;
  while (textstring2[count]!=0)
    {
    if (textstring2[count]!=127)
      {
#if defined(USE_GLES)
    GLfloat quad[12];
    GLfloat tex[8];
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
#else
      glBegin(GL_QUADS);
#endif
      glColor4f(red,green,blue,alpha);

      vec[0]=(float)x+(float)count*(float)textsize;
      vec[1]=(float)y;
      convertscreenvertex(vec,font.sizex,font.sizey);
      texcoord[0]=(float)(textstring2[count]&15)*16.0f+0.5f;
      texcoord[1]=(float)(textstring2[count]>>4)*16.0f+0.5f;
      texcoord[0]/=256.0f;
      texcoord[1]/=256.0f;
#if defined(USE_GLES)
        quad[0] = vec[0];
        quad[1] = vec[1];
        quad[2] = vec[2];
        tex[0] = texcoord[0];
        tex[1] = texcoord[1];
#else
      glTexCoord2fv(texcoord);
      glVertex3fv(vec);
#endif
      vec[0]=(float)x+(float)textsize+(float)(count)*(float)textsize;
      vec[1]=(float)y;
      convertscreenvertex(vec,font.sizex,font.sizey);
      texcoord[0]=(float)(textstring2[count]&15)*16.0f+16.0f-0.5f;
      texcoord[1]=(float)(textstring2[count]>>4)*16.0f+0.5f;
      texcoord[0]/=256.0f;
      texcoord[1]/=256.0f;
  
#if defined(USE_GLES)
        quad[3] = vec[0];
        quad[4] = vec[1];
        quad[5] = vec[2];
        tex[2] = texcoord[0];
        tex[3] = texcoord[1];
#else
      glTexCoord2fv(texcoord);
      glVertex3fv(vec);
#endif
      vec[0]=(float)x+(float)textsize+(float)(count)*(float)textsize;
      vec[1]=(float)y+(float)textsize;
      convertscreenvertex(vec,font.sizex,font.sizey);
      texcoord[0]=(float)(textstring2[count]&15)*16.0f+16.0f-0.5f;
      texcoord[1]=(float)(textstring2[count]>>4)*16.0f+16.0f-0.5f;
      texcoord[0]/=256.0f;
      texcoord[1]/=256.0f;
  
#if defined(USE_GLES)
        quad[6] = vec[0];
        quad[7] = vec[1];
        quad[8] = vec[2];
        tex[4] = texcoord[0];
        tex[5] = texcoord[1];
#else
      glTexCoord2fv(texcoord);
      glVertex3fv(vec);
#endif
      vec[0]=(float)x+(float)(count)*(float)textsize;
      vec[1]=(float)y+(float)textsize;
      convertscreenvertex(vec,font.sizex,font.sizey);
      texcoord[0]=(float)(textstring2[count]&15)*16.0f+0.5f;
      texcoord[1]=(float)(textstring2[count]>>4)*16.0f+16.0f-0.5f;
      texcoord[0]/=256.0f;
      texcoord[1]/=256.0f;
#if defined(USE_GLES)
        quad[9] = vec[0];
        quad[10] = vec[1];
        quad[11] = vec[2];
        tex[6] = texcoord[0];
        tex[7] = texcoord[1];
#else
      glTexCoord2fv(texcoord);
      glVertex3fv(vec);
#endif

#if defined(USE_GLES)
        glVertexPointer(3, GL_FLOAT, 0, quad);
        glTexCoordPointer(2, GL_FLOAT, 0, tex);
        glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
#else
      glEnd();
#endif
      }
    else
      {
      glDisable(GL_TEXTURE_2D);

#if defined(USE_GLES)
    GLfloat quad[12];
    glEnableClientState(GL_VERTEX_ARRAY);
#else
      glBegin(GL_QUADS);
#endif
      glColor4f(red,green,blue,alpha);

      vec[0]=(float)x+(float)count*(float)textsize;
      vec[1]=(float)y;
      convertscreenvertex(vec,font.sizex,font.sizey);
#if defined(USE_GLES)
        quad[0] = vec[0];
        quad[1] = vec[1];
        quad[2] = vec[2];
#else
      glVertex3fv(vec);
#endif
  
      vec[0]=(float)x+(float)textsize+(float)(count)*(float)textsize;
      vec[1]=(float)y;
      convertscreenvertex(vec,font.sizex,font.sizey);
#if defined(USE_GLES)
        quad[3] = vec[0];
        quad[4] = vec[1];
        quad[5] = vec[2];
#else
      glVertex3fv(vec);
#endif
      vec[0]=(float)x+(float)textsize+(float)(count)*(float)textsize;
      vec[1]=(float)y+(float)textsize;
      convertscreenvertex(vec,font.sizex,font.sizey);
#if defined(USE_GLES)
        quad[6] = vec[0];
        quad[7] = vec[1];
        quad[8] = vec[2];
#else
      glVertex3fv(vec);
#endif
      vec[0]=(float)x+(float)(count)*(float)textsize;
      vec[1]=(float)y+(float)textsize;
      convertscreenvertex(vec,font.sizex,font.sizey);
#if defined(USE_GLES)
        quad[9] = vec[0];
        quad[10] = vec[1];
        quad[11] = vec[2];
#else
      glVertex3fv(vec);
#endif

#if defined(USE_GLES)
        glVertexPointer(3, GL_FLOAT, 0, quad);
        glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisableClientState(GL_VERTEX_ARRAY);
#else
      glEnd();
#endif
      glEnable(GL_TEXTURE_2D);
      }

    count++;
    }
  }

void setuptextdisplay(void)
  {
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  glViewport(0,0,windowinfo.resolutionx,windowinfo.resolutiony);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0f,1.0f,-0.75f,0.75f,1.0f,1000.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  }

void convertscreenvertex(float vec[3],int resolutionx,int resolutiony)
  {
  vec[0]=(vec[0]-(float)(resolutionx>>1))/(float)(resolutionx>>1);
  vec[1]=((float)(resolutiony>>1)-vec[1])/(float)(resolutionx>>1);
  vec[2]=-1.0f;
  }

void drawbackground(int texturenum,int x,int y,int sizex,int sizey,int resolutionx,int resolutiony)
  {
  float vec[3];

  if ((x&TEXT_CENTER)==TEXT_CENTER)
    x=x-(sizex)/2;
  if ((y&TEXT_CENTER)==TEXT_CENTER)
    y=y-(sizey)/2;
  if ((x&TEXT_END)==TEXT_END)
    x=x-(sizex);
  if ((y&TEXT_END)==TEXT_END)
    y=y-sizey;
  x&=0xFFFF;
  y&=0xFFFF;

  glBindTexture(GL_TEXTURE_2D,texture[texturenum].glname);

#if defined(USE_GLES)
    GLfloat quad[12];
    GLfloat tex[] = {   0,0,
                        1,0,
                        1,1,
                        0,1 };
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
#else
  glBegin(GL_QUADS);
#endif
  vec[0]=(float)x;
  vec[1]=(float)y;
  convertscreenvertex(vec,resolutionx,resolutiony);

#if defined(USE_GLES)
        quad[0] = vec[0];
        quad[1] = vec[1];
        quad[2] = vec[2];
#else
  glTexCoord2f(0.0f,0.0f);
  glVertex3fv(vec);
#endif
  vec[0]=(float)(x+sizex);
  vec[1]=(float)y;
  convertscreenvertex(vec,resolutionx,resolutiony);

#if defined(USE_GLES)
        quad[3] = vec[0];
        quad[4] = vec[1];
        quad[5] = vec[2];
#else
  glTexCoord2f(1.0f,0.0f);
  glVertex3fv(vec);
#endif
  vec[0]=(float)(x+sizex);
  vec[1]=(float)(y+sizey);
  convertscreenvertex(vec,resolutionx,resolutiony);
#if defined(USE_GLES)
        quad[6] = vec[0];
        quad[7] = vec[1];
        quad[8] = vec[2];
#else
  glTexCoord2f(1.0f,1.0f);
  glVertex3fv(vec);
#endif
  vec[0]=(float)x;
  vec[1]=(float)(y+sizey);
  convertscreenvertex(vec,resolutionx,resolutiony);
#if defined(USE_GLES)
        quad[9] = vec[0];
        quad[10] = vec[1];
        quad[11] = vec[2];
#else
  glTexCoord2f(0.0f,1.0f);
  glVertex3fv(vec);
#endif

#if defined(USE_GLES)
        glVertexPointer(3, GL_FLOAT, 0, quad);
        glTexCoordPointer(2, GL_FLOAT, 0, tex);
        glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
#else
  glEnd();
#endif
  }

void drawmousecursor(int texturenum,int x,int y,int textsize,float red,float green,float blue,float alpha)
  {
  float vec[3];

  glBindTexture(GL_TEXTURE_2D,texture[texturenum].glname);

#if defined(USE_GLES)
    GLfloat quad[12];
    GLfloat tex[] = {   0,0,
                        1,0,
                        1,1,
                        0,1 };
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
#else
  glBegin(GL_QUADS);
#endif
  vec[0]=(float)x-(float)textsize;
  vec[1]=(float)y-(float)textsize;
  convertscreenvertex(vec,font.sizex,font.sizey);

  glColor4f(red,green,blue,alpha);
#if defined(USE_GLES)
        quad[0] = vec[0];
        quad[1] = vec[1];
        quad[2] = vec[2];
#else
  glTexCoord2f(0.0f,0.0f);
  glVertex3fv(vec);
#endif
  vec[0]=(float)x+(float)textsize;
  vec[1]=(float)y-(float)textsize;
  convertscreenvertex(vec,font.sizex,font.sizey);
#if defined(USE_GLES)
        quad[3] = vec[0];
        quad[4] = vec[1];
        quad[5] = vec[2];
#else
  glTexCoord2f(1.0f,0.0f);
  glVertex3fv(vec);
#endif
  vec[0]=(float)x+(float)textsize;
  vec[1]=(float)y+(float)textsize;
  convertscreenvertex(vec,font.sizex,font.sizey);
#if defined(USE_GLES)
        quad[6] = vec[0];
        quad[7] = vec[1];
        quad[8] = vec[2];
#else
  glTexCoord2f(1.0f,1.0f);
  glVertex3fv(vec);
#endif
  vec[0]=(float)x-(float)textsize;
  vec[1]=(float)y+(float)textsize;
  convertscreenvertex(vec,font.sizex,font.sizey);
#if defined(USE_GLES)
        quad[9] = vec[0];
        quad[10] = vec[1];
        quad[11] = vec[2];
#else
  glTexCoord2f(0.0f,1.0f);
  glVertex3fv(vec);
#endif

#if defined(USE_GLES)
        glVertexPointer(3, GL_FLOAT, 0, quad);
        glTexCoordPointer(2, GL_FLOAT, 0, tex);
        glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
#else
  glEnd();
#endif
  }

