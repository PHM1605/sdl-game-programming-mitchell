#pragma once 
#include <vector>
#include "Layer.h"
#include "Level.h"
#include "Vector2D.h"

class TileLayer: public Layer {
public:
  TileLayer(int tileSize, const std::vector<Tileset>& tilesets);
  virtual ~TileLayer() {}
  virtual void update(Level* pLevel); // will do nothing in Platformer game
  virtual void render();
private:
  int m_numColumns;
  int m_numRows;
  int m_tileSize;
  int m_mapWidth; // #pixels of the big Map
  Vector2D m_position; // position of Game screen on Map
  Vector2D m_velocity;
  const std::vector<Tileset>& m_tilesets;
  std::vector<std::vector<int>> m_tileIDs;
};