#include "Camera.h"
#include "Game.h"

Camera* Camera::s_pCamera = new Camera();

Camera::Camera():
  m_pTarget(nullptr), m_position(0,0)
{}

Camera::~Camera() {
  delete m_pTarget;
}

// Camera top-left is adjusted so that Player is at screen center
const Vector2D Camera::getPosition() const {
  if (m_pTarget != 0) {
    Vector2D pos(m_pTarget->m_x - TheGame::Instance()->getGameWidth()/2, 0);
    if (pos.m_x < 0)
      pos.m_x = 0;
    return pos;
  }

  return m_position;
}

void Camera::update(Vector2D velocity) {
  m_position += velocity;
  if (m_position.m_x < 0) {
    m_position.m_x = 0;
  }
}
// DONE