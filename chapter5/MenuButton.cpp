#include "MenuButton.h"
#include "InputHandler.h"

MenuButton::MenuButton(const LoaderParams* pParams):
  SDLGameObject(pParams) 
{
  m_currentFrame = MOUSE_OUT;
}

void MenuButton::draw() {
  SDLGameObject::draw();
}

void MenuButton::update() {
  Vector2D* pMousePos = TheInputHandler::Instance()->getMousePosition();
  if (pMousePos->getX() < (m_position.getX() + m_width) && pMousePos->getX() > m_position.getX()
  && pMousePos->getY() < (m_position.getY() + m_height) && pMousePos->getY() > m_position.getY()) {
    m_currentFrame = MOUSE_OVER; // frame 1 of Button .png
    if (TheInputHandler::Instance()->getMouseButtonState(LEFT))
      m_currentFrame = CLICKED; // frame 2 of Button .png
  }
  else 
    m_currentFrame = MOUSE_OUT; // frame 0 of Button .png
}

void MenuButton::clean() {
  SDLGameObject::clean();
}