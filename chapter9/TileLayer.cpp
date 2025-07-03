#include "TileLayer.h"

TileLayer::TileLayer(int tileSize, int mapWidth, int mapHeight, const std::vector<Tileset>& tilesets):
  m_tileSize(tileSize), m_tilesets(tilesets), m_position(0,0), m_velocity(0,0)
{
  m_numColumns = mapWidth;
  m_numRows = mapHeight;
  m_mapWidth = mapWidth; 
}

void TileLayer::update(Level* pLevel) {}

void TileLayer::render() {

}

Tileset TileLayer::getTilesetByID(int tileID) {
  
}