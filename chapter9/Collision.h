#pragma once 
#include "SDL2/SDL.h"
#include "Vector2D.h"

const static int s_buffer = 4; // reduce Collision-box to be less than Sprite-shape

static bool RectRect(SDL_Rect* A, SDL_Rect* B) {
  int aHBuf = A->h / s_buffer;
  int aWBuf = A->w / s_buffer;
  int bHBuf = B->h / s_buffer;
  int bWBuf = B->w / s_buffer;

  // if bottom A less than top B -> no collision
  if (A->y + A->h - aHBuf <= B->y + bHBuf) { return false; }
  // if top A more than bottom B -> no collision
  if (A->y + aHBuf >= B->y + B->h - bHBuf) { return false; }
  // if right A less than left B -> no collision 
  if (A->x + A->w - aWBuf <= B->x + bWBuf) { return false; }
  // if left A more than right B -> no collision
  if (A->x + aWBuf >= B->x + B->w - bWBuf) { return false; }

  return true; // collision
}
// DONE