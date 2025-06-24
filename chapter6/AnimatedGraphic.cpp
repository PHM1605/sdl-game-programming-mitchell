#include <SDL2/SDL.h>
#include "AnimatedGraphic.h"

AnimatedGraphic::AnimatedGraphic(): SDLGameObject() {}

void AnimatedGraphic::update() {
  m_currentFrame = int(SDL_GetTicks()/1000*m_animSpeed) % m_numFrames;
}

void AnimatedGraphic::draw() {
  SDLGameObject::draw();
}

void AnimatedGraphic::clean() {}

void AnimatedGraphic::load(const LoaderParams* pParams) {
  SDLGameObject::load(pParams);
  m_animSpeed = pParams->getAnimSpeed();
}