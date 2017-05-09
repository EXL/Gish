#ifndef GISH_PHYSICS_BOND_H
#define GISH_PHYSICS_BOND_H
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

void bondsimulation(void);
void bonddragsimulation(void);
void checkbonds(void);
void createbond(int part1,int part2,int type,int objectnum);
void deletebond(int bondnum);

typedef struct
  {
  int part1;
  int part2;
  int type;
  int timetolive;
  float length;
  float maxlength;
  float elasticity;
  float compression;
  float tension;
  int stresstype;
  float stress;
  int maxstresstype;
  float maxstress;
  int sounddelay;
  int cycles;
  int objectnum;
  float bondnormal[3];
  float veclength;
  float oomass;
  } _bond;

typedef struct
  {
  float elasticity;
  float compression;
  float tension;
  int cycles;
  int rendertype;
  float rendersize;
  } _bondtype;

extern int numofbonds;
extern _bond bond[16384];
extern _bondtype bondtype[256];

#endif /* GISH_PHYSICS_BOND_H */
