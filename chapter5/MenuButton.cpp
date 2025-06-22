#include "MenuButton.h"
#include "InputHandler.h"

MenuButton::MenuButton(const LoaderParams* pParams, void(*callback)()):
  SDLGameObject(pParams), m_callback(callback)
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
    // when left-clicking for the first time (not when keep-constant-clicking)
    if (TheInputHandler::Instance()->getMouseButtonState(LEFT) && m_bReleased) {
      m_currentFrame = CLICKED; // frame 2 of Button .png
      m_callback(); // call callback function when click
      m_bReleased = false;
    }
    // when mouse-over but not left-clicking 
    else if (!TheInputHandler::Instance()->getMouseButtonState(LEFT)) {
      m_bReleased = true;
      m_currentFrame = MOUSE_OVER;
    }      
  }
  // mouse outside 
  else 
    m_currentFrame = MOUSE_OUT; // frame 0 of Button .png
}

void MenuButton::clean() {
  SDLGameObject::clean();
}