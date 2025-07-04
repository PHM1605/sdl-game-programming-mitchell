#include "Camera.h"
#include "Game.h"
#include "TextureManager.h"
#include "TileLayer.h"

TileLayer::TileLayer(int tileSize, int mapWidth, int mapHeight, const std::vector<Tileset>& tilesets):
  m_tileSize(tileSize), m_tilesets(tilesets), m_position(0,0), m_velocity(0,0)
{
  m_numColumns = mapWidth; // [cell]
  m_numRows = mapHeight; // [cell]
  m_mapWidth = mapWidth; // [cell]
}

void TileLayer::update(Level* pLevel) {}

void TileLayer::render() {
  // draw only the Tiles which are 1/ value != 0 and 2/ within Camera range
  for (int i=0; i<m_numRows; i++) {
    for (int j=0; j<m_numColumns; j++) {
      int id = m_tileIDs[i][j];
      // if Tile value = 0 -> not draw
      if (id == 0) continue;
      // if column < Camera or > Camera width then don't draw
      int offset = j*m_tileSize-TheCamera::Instance()->getPosition().m_x;
      if (offset < -m_tileSize || offset > TheGame::Instance()->getGameWidth()) 
        continue;
      // draw within-range-columns
      Tileset tileset = getTilesetByID(id);
      id--; // as meaningful TilemapID starts from 1 -> subtracting 1 to make indexing 0
      TheTextureManager::Instance()->drawTile(
        tileset.name, 
        tileset.margin, tileset.spacing, 
        j*m_tileSize-TheCamera::Instance()->getPosition().m_x, i*m_tileSize-TheCamera::Instance()->getPosition().m_y, // destination to draw on screen
        m_tileSize, m_tileSize,
        (id-(tileset.firstGridID-1))/tileset.numColumns, (id-(tileset.firstGridID-1)) % tileset.numColumns,
        TheGame::Instance()->getRenderer() 
      );
    }
  }
}

// tileID e.g. 15 belongs to which Tileset => return Tileset data
// 1 Tileset only => return directly
// 2 Tilesets or more => check if tileID within range 
Tileset TileLayer::getTilesetByID(int tileID) {
  for (int i=0; i<m_tilesets.size(); i++) {
    if (i < m_tilesets.size()-1) {
      if (tileID >= m_tilesets[i].firstGridID && tileID < m_tilesets[i+1].firstGridID) 
        return m_tilesets[i];
    } else {
      return m_tilesets[i];
    }
  }
  
  std::cout << "did not find Tileset, returning empty tileset\n";
  Tileset t;
  return t;
}
// DONE