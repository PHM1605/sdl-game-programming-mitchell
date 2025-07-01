#pragma once 

class Level;

class Layer {
public:
   virtual ~Layer() {}
   virtual void render() = 0;
   virtual void update(Level* pLevel) = 0; // will do nothing in Platformer game (we don't scroll Game Map)
};