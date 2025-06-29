#pragma once
#include "ShooterObject.h"

class PlayerBullet: public ShooterObject {
public:
    PlayerBullet(): ShooterObject() {}
    virtual ~PlayerBullet() {}
    virtual std::string type() { return "PlayerBullet"; }
private: 
    Vector2D m_heading;
}

class EnemyBullet: public PlayerBullet {
public:
    EnemyBullet(): PlayerBullet() {}
    virtual ~EnemyBullet() {}
    virtual std::string type() { return "EnemyBullet"; }
}