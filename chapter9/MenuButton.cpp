#include "MenuButton.h"
#include "InputHandler.h"

MenuButton::MenuButton():
  PlatformerObject(), m_callback(0), m_bReleased(true)
{}

void MenuButton::load(std::unique_ptr<LoaderParams> const &pParams) {
  PlatformerObject::load(pParams);
  m_callbackID = pParams->getCallbackID(); // callbackID of Button will be set in 'conan.xml' file
  m_currentFrame = MOUSE_OUT;
}

void MenuButton::draw() {
  PlatformerObject::draw();
}

void MenuButton::update() {
  Vector2D* pMousePos = TheInputHandler::Instance()->getMousePosition();
  if (pMousePos->getX() < (m_position.getX() + m_width) && pMousePos->getX() > m_position.getX()
  && pMousePos->getY() < (m_position.getY() + m_height) && pMousePos->getY() > m_position.getY()) {
    // left-click for the first time
    if (TheInputHandler::Instance()->getMouseButtonState(LEFT) && m_bReleased) {
      m_currentFrame = CLICKED;
      if (m_callback != 0)
        m_callback();
      m_bReleased = false; // prevent running this block again when keep pressing LEFT
    } else if (!TheInputHandler::Instance()->getMouseButtonState(LEFT)) {
      m_bReleased = true;
      m_currentFrame = MOUSE_OVER;
    }
  } else {
    m_currentFrame = MOUSE_OUT;
  }
}

void MenuButton::clean() {
  PlatformerObject::clean();
}
// DONE