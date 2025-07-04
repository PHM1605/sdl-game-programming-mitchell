#include "AnimatedGraphic.h"

AnimatedGraphic::AnimatedGraphic():
    PlatformerObject() 
{}

void AnimatedGraphic::load(std::unique_ptr<LoaderParams> const &pParams) {
    PlatformerObject::load(pParams);
    m_animSpeed = pParams->getAnimSpeed();
}

void AnimatedGraphic::draw() {
    PlatformerObject::draw();
}

void AnimatedGraphic::update() {
    m_currentFrame = int(SDL_GetTicks() / (1000/m_animSpeed)) % m_numFrames;
}

void AnimatedGraphic::clean() {
    PlatformerObject::clean();
}