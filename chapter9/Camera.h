#pragma once 

#include <iostream>
#include "Vector2D.h"

class Camera {
public:
  // to make Camera a Singleton
  static Camera* Instance() {
    if(s_pCamera == 0)
      s_pCamera = new Camera();
    return s_pCamera;
  }

  void update(Vector2D velocity);
  void setTarget(Vector2D* target) { m_pTarget = target; }
  void setPosition(const Vector2D& position) { m_position = position; }
  const Vector2D getPosition() const;

private:
  Camera();
  ~Camera();

  Vector2D* m_pTarget; // Camera's target (don't really understand this, but seems like it isn't set anywhere, so no effect)
  Vector2D m_position; // Camera's position in full Tilemap
  static Camera* s_pCamera;
};

typedef Camera TheCamera;
// DONE