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

#include "../sdl/sdl.h"

#include <string.h>

#include "../input/keyboard.h"

unsigned char keyboard[SDL_NUM_SCANCODES];
unsigned char prevkeyboard[SDL_NUM_SCANCODES];

void checkkeyboard(void)
  {
  memcpy(prevkeyboard,keyboard,SDL_NUM_SCANCODES);

  memcpy(keyboard, SDL_GetKeyboardState(NULL), SDL_NUM_SCANCODES);
#if defined(PANDORA)
  if (keyboard[SDLK_HOME]) {
        keyboard[SDLK_HOME] = 0;
        keyboard[SDLK_a] = 1;
  }

  if (keyboard[SDLK_END]) {
        keyboard[SDLK_END] = 0;
        keyboard[SDLK_b] = 1;
  }

  if (keyboard[SDLK_PAGEUP]) {
        keyboard[SDLK_PAGEUP] = 0;
        keyboard[SDLK_c] = 1;
  }

  if (keyboard[SDLK_PAGEDOWN]) {
        keyboard[SDLK_PAGEDOWN] = 0;
        keyboard[SDLK_d] = 1;
  }
#elif defined(GCW)
  if (keyboard[SDLK_SPACE]) {
        keyboard[SDLK_SPACE] = 0;
        keyboard[SDLK_a] = 1;
  }

  if (keyboard[SDLK_LSHIFT]) {
        keyboard[SDLK_LSHIFT] = 0;
        keyboard[SDLK_SPACE] = 1;
  }

  if (keyboard[SDLK_LCTRL]) {
        keyboard[SDLK_LCTRL] = 0;
        keyboard[SDLK_s] = 1;
  }

  if (keyboard[SDLK_LALT]) {
        keyboard[SDLK_LALT] = 0;
        keyboard[SDLK_d] = 1;
  }
#endif

  if (keyboard[SDL_SCANCODE_KP_ENTER])
    keyboard[SDL_SCANCODE_RETURN]=1;
  if (keyboard[SDL_SCANCODE_RCTRL])
    keyboard[SDL_SCANCODE_LCTRL]=1;
  if (keyboard[SDL_SCANCODE_RSHIFT])
    keyboard[SDL_SCANCODE_LSHIFT]=1;

  if (keyboard[SDL_SCANCODE_KP_4])
    keyboard[SDL_SCANCODE_LEFT]=1;
  if (keyboard[SDL_SCANCODE_KP_6])
    keyboard[SDL_SCANCODE_RIGHT]=1;
  if (keyboard[SDL_SCANCODE_KP_8])
    keyboard[SDL_SCANCODE_UP]=1;
  if (keyboard[SDL_SCANCODE_DOWN])
    keyboard[SDL_SCANCODE_DOWN]=1;
  }

