#pragma once
#include "LoaderParams.h"
#include "Vector2D.h"
#include <memory>

class GameObject {
public:
	virtual ~GameObject() {}
	virtual void load(std::unique_ptr<LoaderParams> const &pParams) = 0;
	virtual void draw() = 0;
	virtual void update() = 0;
	virtual void clean() = 0;
	virtual void collision() = 0; // handle collision
	virtual std::string type() = 0; // get object type
	Vector2D& getPosition() { return m_position; }
	int getWidth() { return m_width; }
	int getHeight() { return m_height; }
	// object moves backward means screen moves forward
	void scroll(float scrollSpeed) {
		m_position.setX(m_position.getX() - scrollSpeed);
	}
	// updating when object is within GameScreen
	bool updating() { return m_bUpdating; }
	void setUpdating(bool updating) { m_bUpdating = updating; }
	// dead status
	bool dead() { return m_bDead; }
	bool dying() { return m_bDying; }
	
protected:
	GameObject():
		m_position(0, 0), m_velocity(0, 0), m_acceleration(0, 0),
		m_width(0), m_height(0),
		m_currentRow(0), m_currentFrame(0),
		m_bUpdating(false), m_bDead(false), m_bDying(false),
		m_angle(0), m_alpha(255)
	{}
	// movement variables
	Vector2D m_position;
	Vector2D m_velocity; 
	Vector2D m_acceleration;
	// size variables
	int m_width;
	int m_height;
	// animation variables
	int m_currentRow;
	int m_currentFrame;
	int m_numFrames;
	std::string m_textureID;
	// common boolean state variables
	bool m_bUpdating; // true when object is within game screen
	bool m_bDead;
	bool m_bDying;
	// rotation 
	double m_angle;
	// blending 
	int m_alpha;
};
