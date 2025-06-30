#pragma once 
#include <string>
#include <vector>
#include <iostream>
#include "Layer.h"
#include "Player.h"
// #include "CollisionManager.h"

class TileLayer;

struct Tileset {
    int firstGridID; // firstGridID=199 means 1st cell in Tileset has number=199, 2nd cell=200,....
    int tileWidth; // of a small Tile e.g. 32 pixels
    int tileHeight; // of a small Tile e.g. 32 pixels
    int spacing;
    int margin;
    int width; // of the whole texture image
    int height; // of the whole texture image
    int numColumns;
    std::string name;
};

class Level {
public:
    ~Level();
    void update();
    void render();
    std::vector<Tileset>* getTilesets() {
        return &m_tilesets;
    }
    std::vector<Layer*>* getLayers() {
        return &m_layers;
    }
    std::vector<TileLayer*>* getCollisionLayers() { return &m_collisionLayers; }
    std::vector<TileLayer*>& getCollidableLayers() { return m_collisionLayers; }

    Player* getPlayer() { return m_pPlayer; }
    void setPlayer(Player* pPlayer) { m_pPlayer = pPlayer; }
private:
    // NOTE: we make Level's constructor private to ensure it must be created by its 'friend' class
    friend class LevelParser;
    Level();
    
    Player* m_pPlayer;

    std::vector<Tileset> m_tilesets;
    std::vector<Layer*> m_layers;
    std::vector<TileLayer*> m_collisionLayers;
};