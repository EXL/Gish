#ifndef GISH_GAME_BOSS_H
#define GISH_GAME_BOSS_H
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

void createboss(int type,float position[3]);
void bosssimulation(void);
void bosstimetolive(void);
void deleteboss(int bossnum);

typedef struct
  {
  int type;
  float position[3];
  float velocity[3];
  int timetolive;
  int direction;
  int texturenum;
  int animationnum;
  int animationtype;
  int frame;
  float size[2];
  float framedelay;
  } _boss;

extern int numofbosses;
extern _boss boss[16];

#endif /* GISH_GAME_BOSS_H */
