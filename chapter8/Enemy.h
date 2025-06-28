#pragma once
#include "GameObjectFactory.h"
#include "ShooterObject.h"
#include <SDL2/SDL.h>

// Enemy base class
class Enemy : public ShooterObject {
public:
    virtual std::string type() { return "Enemy"; }

protected:
    int m_health;
    Enemy(): ShooterObject() {}
    virtual ~Enemy() {}    
};
