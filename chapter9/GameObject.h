#pragma once 
#include <string>
#include <memory> // for std::unique_ptr
#include <vector>
#include "LoaderParams.h"
#include "Vector2D.h"

class GameObject {
public:
  virtual ~GameObject() {}

protected:
  GameObject():
    m_position(0,0), m_velocity(0,0), m_acceleration(0,0),
    m_width(0), m_height(0),
    m_currentRow(0), m_currentFrame(0), // in spritesheet
    m_bUpdating(false),
    m_bDead(false), m_bDying(false),
    m_angle(0), m_alpha(255) {}
  
  // movement variables
  Vector2D m_position;
  Vector2D m_velocity;
  Vector2D m_acceleration;
  // size
  int m_width;
  int m_height;
  // animation
  int m_currentRow;
  int m_currentFrame;
  int m_numFrames;
  std::string m_textureID;

  bool m_bUpdating;
  bool m_bDead;
  bool m_bDying;
  double m_angle;
  int m_alpha;
};