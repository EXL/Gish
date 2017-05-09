#ifndef GISH_PHYSICS_OBJECT_H
#define GISH_PHYSICS_OBJECT_H
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

int lineintersectobject(float *intersectpoint,float *normal,float *scale,float *startpoint,float *endpoint,int objectnum,int *particlelist);
int lineintersectobject2(float *intersectpoint,float *normal,float *scale,float *startpoint,float *endpoint,int objectnum,int *particlelist);
int pointintersectobject(float *intersectpoint,float *normal,float *scale,float *point,int objectnum,int objectnum2,int *particlelist);

#endif /* GISH_PHYSICS_OBJECT_H */
