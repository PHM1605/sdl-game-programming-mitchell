#pragma once
#include "ShooterObject.h"

class PlayerBullet: public ShooterObject {
public:
    PlayerBullet(): ShooterObject() {}
    virtual ~PlayerBullet() {}
    virtual std::string type() { return "PlayerBullet"; }
    virtual void load(std::unique_ptr<LoaderParams> pParams, Vector2D heading) {
        ShooterObject::load(std::move(pParams));
        m_heading = heading;
    }
    virtual void draw() {
        ShooterObject::draw();
    }
    virtual void collision() {
        m_bDead = true;
    }
    virtual void update() {
        m_velocity.setX(m_heading.getX());
        m_velocity.setY(m_heading.getY());
        ShooterObject::update();
    }
    virtual void clean() {
        ShooterObject::clean();
    }
private: 
    Vector2D m_heading;
};

class EnemyBullet: public PlayerBullet {
public:
    EnemyBullet(): PlayerBullet() {}
    virtual ~EnemyBullet() {}
    virtual std::string type() { return "EnemyBullet"; }
};