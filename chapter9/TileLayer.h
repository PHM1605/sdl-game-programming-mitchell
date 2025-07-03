#pragma once 
#include <vector>
#include "Layer.h"
#include "Level.h"
#include "Vector2D.h"

class TileLayer: public Layer {
public:
  TileLayer(int tileSize, int mapWidth, int mapHeight, const std::vector<Tileset>& tilesets);
  virtual ~TileLayer() {}
  virtual void update(Level* pLevel); // will do nothing in Platformer game
  virtual void render();

  void setTileIDs(const std::vector<std::vector<int>>& data) { m_tileIDs = data; }
  const std::vector<std::vector<int>>& getTileIDs() { return m_tileIDs; }
  
  int getTileSize() { return m_tileSize; }
  void setTileSize(int tileSize) { m_tileSize = tileSize; }
  
  int getMapWidth() { return m_mapWidth; }
  void setMapWidth(int mapWidth) { m_mapWidth = mapWidth; }
  
  const Vector2D getPosition() { return m_position; }
  void setPosition(Vector2D position) { m_position = position; }

  Tileset getTilesetByID(int tileID);
private:
// m_mapWidth = m_numColumns: whole TileLayer width, in terms of #cells
  int m_mapWidth; 
  int m_numColumns;
  int m_numRows;
  int m_tileSize;

  Vector2D m_position; // position of Game screen on Map
  Vector2D m_velocity;
  Vector2D m_acceleration;

  float diff;

  const std::vector<Tileset>& m_tilesets;
  std::vector<std::vector<int>> m_tileIDs;
};
// DONE