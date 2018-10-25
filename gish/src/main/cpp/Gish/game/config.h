#ifndef GISH_GAME_CONFIG_H
#define GISH_GAME_CONFIG_H
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

#include <stdio.h>

char* userpath(char *result, char *path);
void loadconfig(void);
void saveconfig(void);
void notsupportedmenu(void);
void pleasedisabletouchcontrols(void);
void optionreadint(int *ptr,char *str);
void optionwriteint(FILE *fp, int *ptr,char *str);
char* stringconcat(const char *s1, const char *s2);
/*void optionreadstring(char *ptr,char *str,int size);
void optionwritestring(FILE *fp, char *ptr,char *str,int size);*/

typedef struct {
  int resolutionx;
  int resolutiony;
  int bitsperpixel;
  int depthbits;
  int stencilbits;
  int fullscreen;
  int sound;
  int music;
  int joystick;
  } _config;

extern _config config;

#ifdef __HAIKU__
char *getHaikuSettingsPath();
#endif

#endif /* GISH_GAME_CONFIG_H */
