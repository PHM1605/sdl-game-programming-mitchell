#include "TileLayer.h"
#include "TextureManager.h"
#include "Game.h"

TileLayer::TileLayer(int tileSize, const std::vector<Tileset>& tilesets):
    m_tileSize(tileSize), m_tilesets(tilesets), m_position(0,0), m_velocity(0,0)
{
    m_numColumns = (TheGame::Instance()->getGameWidth() / m_tileSize);
    m_numRows = (TheGame::Instance()->getGameHeight() / m_tileSize);
}
