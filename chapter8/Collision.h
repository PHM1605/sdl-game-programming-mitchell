#pragma once 
#include "SDL2/SDL.h"
#include "Vector2D.h"

// to make collision bounding-box smaller - to be less sensitive
const static int s_buffer = 4;

static bool RectRect(SDL_Rect* A, SDL_Rect* B) {
  // reduction of collision box A
  int aHBuf = A->h / s_buffer;
  int aWBuf = A->w / s_buffer;
  // reduction of collision box B
  int bHBuf = B->h / s_buffer;
  int bWBuf = B->w / s_buffer;
  // if bottom of A (moving up a bit) is less than top of B (moving down a bit) -> no collision
  if (A->y+A->h-aHBuf <= B->y+bHBuf) { return false; }
  // if top of A is more than bottom of B = no collision
  if (A->y+aHBuf >= B->y+B->h-bHBuf) { return false; }
  // if right of A is less than left of B = no collision
  if (A->x+A->w-aWBuf <= B->x+bWBuf) { return false; }
  // if left of A is more than right of B = no collision
  if (A->x+aWBuf >= B->x+B->w-bWBuf) { return false; }
  
  return false;
}