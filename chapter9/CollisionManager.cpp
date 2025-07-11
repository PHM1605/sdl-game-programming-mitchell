#include "CollisionManager.h"
#include "Collision.h"

void CollisionManager::checkPlayerEnemyCollision(Player* pPlayer, const std::vector<GameObject*> &objects) {
  SDL_Rect* pRect1 = new SDL_Rect();
  pRect1->x = pPlayer->getPosition().getX();
  pRect1->y = pPlayer->getPosition().getY();
  pRect1->w = pPlayer->getWidth();
  pRect1->h = pPlayer->getHeight();

  for (int i=0; i<objects.size(); i++) {
    // !updating() means Object NOT within screen
    if (objects[i]->type() != std::string("Enemy") || !objects[i]->updating()) {
      continue;
    }

    SDL_Rect* pRect2 = new SDL_Rect();
    pRect2->x = objects[i]->getPosition().getX();
    pRect2->y = objects[i]->getPosition().getY();
    pRect2->w = objects[i]->getWidth();
    pRect2->h = objects[i]->getHeight();

    if (RectRect(pRect1, pRect2)) {
      if (!objects[i]->dead() && !objects[i]->dying())
        pPlayer->collision();
    }
    delete pRect2;
  }
  delete pRect1;
}
// DONE