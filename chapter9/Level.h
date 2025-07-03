#pragma once 
#include <string>
#include <vector>
#include "Player.h"
#include "Layer.h"

class TileLayer;

struct Tileset {
  int firstGridID;
  int tileWidth; 
  int tileHeight;
  int spacing;
  int margin;
  int width; // of whole Tileset .png
  int height; // of whole Tileset .png
  int numColumns;
  std::string name; 
};

class Level {
public:
  ~Level();

  void update();
  void render();

  std::vector<Tileset>* getTilesets() { return &m_tilesets; }
  std::vector<Layer*>* getLayers() { return &m_layers; }
  std::vector<TileLayer*>* getCollisionLayers() { return &m_collisionLayers; }
  const std::vector<TileLayer*>& getCollidableLayers() { return m_collisionLayers; }

  Player* getPlayer() { return m_pPlayer; }
  void setPlayer(Player* pPlayer) { m_pPlayer = pPlayer; }

private:
  friend class LevelParser;
  Level() {} // put constructor in private, to create 'Level' only with LevelParser
  
  Player* m_pPlayer;
  
  std::vector<Layer*> m_layers;
  std::vector<Tileset> m_tilesets; // will be push_back() in LevelParser (parseTilesets() function)
  std::vector<TileLayer*> m_collisionLayers;
};
// DONE