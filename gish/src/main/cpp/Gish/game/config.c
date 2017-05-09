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

#include "../sdl/sdl.h"

#include <stdio.h>
#include <limits.h>

#ifdef LINUX
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include "../game/config.h"
#include "../game/options.h"
#include "../game/socket.h"
#include "../input/keyboard.h"
#include "../input/mouse.h"
#include "../menu/menu.h"
#include "../parser/parser.h"
#include "../sdl/event.h"
#include "../video/text.h"

_config config;

char* userpath(char *result, char *path)
  {
#ifdef LINUX
#if defined(PANDORA)
  char *home=getenv("PWD");
#else
  char *home=getenv("HOME");
#endif

  if (!home)
    return path;

  if (snprintf(result,PATH_MAX,"%s/.gish",home) < 0)
    return path;

  /* Ignore failure. May exist already. */
  mkdir(result, S_IRWXU | S_IRWXG | S_IRWXO);

  if (snprintf(result,PATH_MAX,"%s/.gish/%s",home,path) < 0)
    return path;

  return result;
#else
  return path;
#endif
  }

void loadconfig(void)
  {
  int count,count2;
  char tempstr[32];
  char path[PATH_MAX];

#if defined(PANDORA)
  config.resolutionx=800;
  config.resolutiony=480;
  config.bitsperpixel=16;
  config.depthbits=16;
#elif defined(GCW)
  config.resolutionx=320;
  config.resolutiony=240;
  config.bitsperpixel=16;
  config.depthbits=16;
#else
  config.resolutionx=800;
  config.resolutiony=600;
  config.bitsperpixel=32;
  config.depthbits=24;
#endif
  config.stencilbits=8;
  config.fullscreen=0;
  config.sound=1;
  config.music=1;
  config.joystick=1;

  option.sound=1;
  option.music=1;
  option.soundvolume=1.0f;
  option.musicvolume=1.0f;

  control[0].key[KEYALIAS_LEFT]=SCAN_LEFT;
  control[0].key[KEYALIAS_RIGHT]=SCAN_RIGHT;
  control[0].key[KEYALIAS_DOWN]=SCAN_DOWN;
  control[0].key[KEYALIAS_UP]=SCAN_UP;
  control[0].key[KEYALIAS_STICK]=SCAN_A;
  control[0].key[KEYALIAS_JUMP]=SCAN_SPACE;
  control[0].key[KEYALIAS_SLIDE]=SCAN_S;
  control[0].key[KEYALIAS_HEAVY]=SCAN_D;
  control[0].joysticknum=0;
  control[0].axis[0]=0;
  control[0].axis[1]=1;
  for (count=0;count<4;count++)
    control[0].button[count]=-1;
  for (count=0;count<4;count++)
    control[0].button[count+4]=count;
  control[0].button[8]=5;

  control[1].joysticknum=-1;
  control[1].axis[0]=0;
  control[1].axis[1]=1;
  for (count=0;count<4;count++)
    control[1].button[count]=-1;
  for (count=0;count<4;count++)
    control[1].button[count+4]=count;

  control[2].joysticknum=-1;
  control[2].axis[0]=0;
  control[2].axis[1]=1;
  for (count=0;count<4;count++)
    control[2].button[count]=-1;
  for (count=0;count<4;count++)
    control[2].button[count+4]=count;

  control[3].joysticknum=-1;
  control[3].axis[0]=0;
  control[3].axis[1]=1;
  for (count=0;count<4;count++)
    control[3].button[count]=-1;
  for (count=0;count<4;count++)
    control[3].button[count+4]=count;

  loadtextfile(userpath(path,"config.txt"));
  optionreadint(&config.resolutionx,"screenwidth=");
  optionreadint(&config.resolutiony,"screenheight=");
  optionreadint(&config.bitsperpixel,"bitsperpixel=");
  optionreadint(&config.depthbits,"depthbits=");
  optionreadint(&config.stencilbits,"stencilbits=");
  optionreadint(&config.fullscreen,"fullscreen=");
  optionreadint(&config.sound,"sound=");
  optionreadint(&config.music,"music=");
  optionreadint(&config.joystick,"joystick=");

  optionreadint(&option.sound,"soundon=");
  optionreadint(&option.music,"musicon=");
  count=-1;
  optionreadint(&count,"soundvolume=");
  if (count!=-1)
    option.soundvolume=(float)count/100.0f;
  count=-1;
  optionreadint(&count,"musicvolume=");
  if (count!=-1)
    option.musicvolume=(float)count/100.0f;

  for (count=0;count<CONTROLS_LENGTH;count++)
    {
    for (count2=0;count2<KEYALIAS_LENGTH;count2++)
      {
      sprintf(tempstr,"player%dkey%d=",count+1,count2+1);
      optionreadint((int*)&control[count].key[count2],tempstr);
      }

    sprintf(tempstr,"player%djoysticknum=",count+1);
    optionreadint(&control[count].joysticknum,tempstr);

    for (count2=0;count2<4;count2++)
      {
      sprintf(tempstr,"player%daxis%d=",count+1,count2+1);
      optionreadint(&control[count].axis[count2],tempstr);
      }
    for (count2=0;count2<16;count2++)
      {
      sprintf(tempstr,"player%dbutton%d=",count+1,count2+1);
      optionreadint(&control[count].button[count2],tempstr);
      }
    }

  windowinfo.resolutionx=config.resolutionx;
  windowinfo.resolutiony=config.resolutiony;
  windowinfo.bitsperpixel=config.bitsperpixel;
  windowinfo.depthbits=config.depthbits;
  windowinfo.stencilbits=config.stencilbits;
  windowinfo.fullscreen=config.fullscreen;
  }

void saveconfig(void)
  {
  int count,count2;
  char tempstr[32];
  FILE *fp;
  char path[PATH_MAX];

  config.resolutionx=windowinfo.resolutionx;
  config.resolutiony=windowinfo.resolutiony;
  config.bitsperpixel=windowinfo.bitsperpixel;
  config.depthbits=windowinfo.depthbits;
  config.stencilbits=windowinfo.stencilbits;
  config.fullscreen=windowinfo.fullscreen;

  if ((fp=fopen(userpath(path,"config.txt"),"wb"))==NULL)
    return;

  optionwriteint(fp, &config.resolutionx,"screenwidth=");
  optionwriteint(fp, &config.resolutiony,"screenheight=");
  optionwriteint(fp, &config.bitsperpixel,"bitsperpixel=");
  optionwriteint(fp, &config.depthbits,"depthbits=");
  optionwriteint(fp, &config.stencilbits,"stencilbits=");
  optionwriteint(fp, &config.fullscreen,"fullscreen=");
  optionwriteint(fp, &config.sound,"sound=");
  optionwriteint(fp, &config.music,"music=");
  optionwriteint(fp, &config.joystick,"joystick=");

  optionwriteint(fp, &option.sound,"soundon=");
  optionwriteint(fp, &option.music,"musicon=");
  count=option.soundvolume*100.0f;
  optionwriteint(fp, &count,"soundvolume=");
  count=option.musicvolume*100.0f;
  optionwriteint(fp, &count,"musicvolume=");

  for (count=0;count<CONTROLS_LENGTH;count++)
    {
    for (count2=0;count2<KEYALIAS_LENGTH;count2++)
      {
      sprintf(tempstr,"player%dkey%d=",count+1,count2+1);
      optionwriteint(fp, (int*)&control[count].key[count2],tempstr);
      }

    sprintf(tempstr,"player%djoysticknum=",count+1);
    optionwriteint(fp, &control[count].joysticknum,tempstr);

    for (count2=0;count2<4;count2++)
      {
      sprintf(tempstr,"player%daxis%d=",count+1,count2+1);
      optionwriteint(fp, &control[count].axis[count2],tempstr);
      }
    for (count2=0;count2<16;count2++)
      {
      sprintf(tempstr,"player%dbutton%d=",count+1,count2+1);
      optionwriteint(fp, &control[count].button[count2],tempstr);
      }
    }

  fclose(fp);
  }

void notsupportedmenu(void)
  {
  int count;
  char *glvendor;
  char *glrenderer;
  char *glversion;
  char *ext;
  FILE *fp;
  char path[PATH_MAX];

  glvendor=(char *) glGetString(GL_VENDOR);
  glrenderer=(char *) glGetString(GL_RENDERER);
  glversion=(char *) glGetString(GL_VERSION);
  ext=(char *) glGetString(GL_EXTENSIONS);

  resetmenuitems();

  while (!menuitem[0].active && !windowinfo.shutdown)
    {
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    numofmenuitems=0;
    createmenuitem("Exit",8,8,16,1.0f,1.0f,1.0f,1.0f);
    setmenuitem(MO_HOTKEY,SCAN_ESC);
    count=352;
    createmenuitem("NVIDIA Drivers",(320|TEXT_CENTER),count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("ATI Drivers",(320|TEXT_CENTER),count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    createmenuitem("Intel Drivers",(320|TEXT_CENTER),count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;

    checksystemmessages();
    checkkeyboard();
    checkmouse();
    checkmenuitems();

    setuptextdisplay();

    count=224;
    drawtext("Multitexture required",(320|TEXT_CENTER),count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    count+=16;
    drawtext("Make sure you have the",(320|TEXT_CENTER),count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    drawtext("latest video card drivers",(320|TEXT_CENTER),count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;
    drawtext("for your computer",(320|TEXT_CENTER),count,16,1.0f,1.0f,1.0f,1.0f);
    count+=16;

    count=432;
    drawtext("GL_VERSION: /s",8,count,12,1.0f,1.0f,1.0f,1.0f,glversion);
    count+=12;
    drawtext("GL_VENDOR: /s",8,count,12,1.0f,1.0f,1.0f,1.0f,glvendor);
    count+=12;
    drawtext("GL_RENDERER: /s",8,count,12,1.0f,1.0f,1.0f,1.0f,glrenderer);
    count+=12;

    drawmenuitems();

    drawmousecursor(768+font.cursornum,mouse.x,mouse.y,16,1.0f,1.0f,1.0f,1.0f);

#if defined(USE_GLES)
    EGL_SwapBuffers();
#else
    SDL_GL_SwapBuffers();
#endif

    if (menuitem[1].active)
      {
      launchwebpage("www.nvidia.com/content/drivers/drivers.asp");
      menuitem[1].active=0;
      }
    if (menuitem[2].active)
      {
      launchwebpage("ati.amd.com/support/driver.html");
      menuitem[2].active=0;
      }
    if (menuitem[3].active)
      {
      launchwebpage("downloadcenter.intel.com");
      menuitem[3].active=0;
      }
    }

  resetmenuitems();

  if ((fp=fopen(userpath(path,"glreport.txt"),"wb"))==NULL)
    return;

  fprintf(fp,"%s\r\n",glversion);
  fprintf(fp,"%s\r\n",glvendor);
  fprintf(fp,"%s\r\n",glrenderer);
  fprintf(fp,"%s\r\n",ext);
  }

void optionreadint(int *ptr,char *str)
  {
  if (findstring(str))
    *ptr=getint();

  parser.textloc=0;
  }

void optionwriteint(FILE *fp, int *ptr,char *str)
  {
  fprintf(fp,"%s%d\r\n",str,*ptr);
  }
/*
void optionreadstring(char *ptr,char *str,int size)
  {
  if (findstring(str))
    getstring(ptr,size);

  parser.textloc=0;
  }

void optionwritestring(FILE *fp, char *ptr,char *str,int size)
  {
  fprintf(fp,"%s%s\r\n",str,ptr);
  }
*/
