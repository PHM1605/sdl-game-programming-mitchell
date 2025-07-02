#pragma once 

#include <vector>
#include "Player.h"

class CollisionManager {
public:
  void checkPlayerEnemyCollision(Player* pPlayer, const std::vector<GameObject*>& objects);
};