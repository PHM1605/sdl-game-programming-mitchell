#include "CollisionManager.h"
#include "Collision.h"
#include "BulletHandler.h"

void CollisionManager::checkPlayerEnemyBulletCollision(Player* pPlayer) {
    // Create collision-box of Player
    SDL_Rect* pRect1 = new SDL_Rect();
    pRect1->x = pPlayer->getPosition().getX();
    pRect1->y = pPlayer->getPosition().getY();
    pRect1->w = pPlayer->getWidth();
    pRect1->h = pPlayer->getHeight();

    for (int i=0; i<TheBulletHandler::Instance()->getEnemyBullets().size(); i++) {
        EnemyBullet* pEnemyBullet = TheBulletHandler::Instance()->getEnemyBullets()[i];
        // Create collision-box of each Bullet
        SDL_Rect* pRect2 = new SDL_Rect();
        pRect2->x = pEnemyBullet->getPosition().getX();
        pRect2->y = pEnemyBullet->getPosition().getY();
        pRect2->w = pEnemyBullet->getWidth();
        pRect2->h = pEnemyBullet->getHeight();
        // Bullet touches Player or not
        if (RectRect(pRect1, pRect2)) {
            if (!pPlayer->dying() && !pEnemyBullet->dying()) {
                pEnemyBullet->collision();
                pPlayer->collision();
            }
        }
        delete pRect2;
    }
    delete pRect1;
}

void CollisionManager::checkEnemyPlayerBulletCollision(const std::vector<GameObject*> &objects) {
    for (int i=0; i<objects.size(); i++) {
        GameObject* pObject = objects[i];
        for (int j=0; j < TheBulletHandler::Instance()->getPlayerBullets().size(); j++) {
            // if the object is not "Enemy" OR object is off-screen
            if (pObject->type() != std::string("Enemy") || !pObject->updating())
                continue;
            
            // Create collision-box of each Enemy-object
            SDL_Rect* pRect1 = new SDL_Rect();
            pRect1->x = pObject->getPosition().getX();
            pRect1->y = pObject->getPosition().getY();
            pRect1->w = pObject->getWidth();
            pRect1->h = pObject->getHeight();
            // Create collision-box of each Player-Bullet
            PlayerBullet* pPlayerBullet = TheBulletHandler::Instance()->getPlayerBullets()[j];
            SDL_Rect* pRect2 = new SDL_Rect();
            pRect2->x = pPlayerBullet->getPosition().getX();
            pRect2->y = pPlayerBullet->getPosition().getY();
            pRect2->w = pPlayerBullet->getWidth();
            pRect2->h = pPlayerBullet->getHeight();
            // Check Enemy and Player-Bullet collision
            if (RectRect(pRect1, pRect2)) {
                if (!pObject->dying() && !pPlayerBullet->dying()) {
                    pPlayerBullet->collision();
                    pObject->collision();
                }
            }
            delete pRect1;
            delete pRect2;
        }
    }
}

void CollisionManager::checkPlayerEnemyCollision(Player* pPlayer, const std::vector<GameObject*> &objects) {
    // Create collision-box of Player
    SDL_Rect* pRect1 = new SDL_Rect();
    pRect1->x = pPlayer->getPosition().getX();
    pRect1->y = pPlayer->getPosition().getY();
    pRect1->w = pPlayer->getWidth();
    pRect1->h = pPlayer->getHeight();

    for (int i=0; i<objects.size(); i++) {
        // when Game+Object is not Enemy OR Object is not within game screen
        if (objects[i]->type() != std::string("Enemy") || !objects[i]->updating()) {
            continue;
        }
        // Create collision-box of each GameObject
        SDL_Rect* pRect2 = new SDL_Rect();
        pRect2->x = objects[i]->getPosition().getX();
        pRect2->y = objects[i]->getPosition().getY();
        pRect2->w = objects[i]->getWidth();
        pRect2->h = objects[i]->getHeight();
        // GameObject touches Player or not
        if (RectRect(pRect1, pRect2)) {
            if (!objects[i]->dead() && !objects[i]->dying()) {
                pPlayer->collision();
            }
        }
        delete pRect2;
    }
    delete pRect1;
}

void CollisionManager::checkPlayerTileCollision(Player* pPlayer, const std::vector<TileLayer*>& collisionLayers) {
    for (std::vector<TileLayer*>::const_iterator it = collisionLayers.begin(); it != collisionLayers.end(); ++it) {
        TileLayer* pTileLayer = (*it);
        std::vector<std::vector<int>> tiles = pTileLayer->getTileIDs();
        Vector2D layerPos = pTileLayer->getPosition(); // position of Game screen on TileLayer
        int x, y, tileColumn, tileRow, tileid = 0;
        // x, y: location of Game screen on TileLayer (in cell number)
        x = layerPos.getX() / pTileLayer->getTileSize();
        y = layerPos.getY() / pTileLayer->getTileSize();
        if (pPlayer->getVelocity().getX() >= 0 || pPlayer->getVelocity().getY() >= 0) {
            // bottom right of Player toucher which cell
            tileColumn = (pPlayer->getPosition().getX() + pPlayer->getWidth()) / pTileLayer->getTileSize();
            tileRow = (pPlayer->getPosition().getY() + pPlayer->getHeight()) / pTileLayer->getTileSize();
            tileid = tiles[tileRow + y][tileColumn + x];
        } else if (pPlayer->getVelocity().getX() < 0 || pPlayer->getVelocity().getY() < 0) {
            // top left of Player touches which cell
            tileColumn = pPlayer->getPosition().getX() / pTileLayer->getTileSize();
            tileRow = pPlayer->getPosition().getY() / pTileLayer->getTileSize();
            tileid = tiles[tileRow + y][tileColumn + x];
        }
        if (tileid != 0) {
            pPlayer->collision();
        }
    }
}