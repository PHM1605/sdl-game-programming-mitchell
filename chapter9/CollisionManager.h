#pragma once 

#include <vector>
#include "Player.h"

// trigger player.collision() when happens
class CollisionManager {
public:
  void checkPlayerEnemyCollision(Player* pPlayer, const std::vector<GameObject*>& objects);
};