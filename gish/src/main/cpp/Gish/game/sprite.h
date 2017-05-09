#ifndef GISH_GAME_SPRITE_H
#define GISH_GAME_SPRITE_H
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

void createsprite(int points,float position[3]);
void spritetimetolive(void);
void deletesprite(int spritenum);
void spritesimulation(void);

typedef struct
  {
  int type;
  int timetolive;
  char text[64];
  float position[3];
  float red;
  float green;
  float blue;
  float alpha;
  int size;
  } _sprite;

extern int numofsprites;
extern _sprite sprite[256];

#endif /* GISH_GAME_SPRITE_H */
