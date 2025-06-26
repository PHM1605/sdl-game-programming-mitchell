#include "TileLayer.h"
#include "TextureManager.h"
#include "Game.h"

TileLayer::TileLayer(int tileSize, const std::vector<Tileset>& tilesets):
    m_tileSize(tileSize), m_tilesets(tilesets), m_position(0,0), m_velocity(0,0)
{
    m_numColumns = (TheGame::Instance()->getGameWidth() / m_tileSize);
    m_numRows = (TheGame::Instance()->getGameHeight() / m_tileSize);
}

// during scrolling 
void TileLayer::update() {
    m_position += m_velocity;
}

void TileLayer::render() {
    // m_position: position of Game-Window-top-left on the big Map (in pixels)
    // x, y: Game-Window-top-left belongs to which Tile on the Tilemap (counting from 0)
    // x2, y2: Game-Window-top-left lies at which pixel-location in that 32x32 Tile
    int x, y, x2, y2 = 0;
    x = m_position.getX() / m_tileSize;
    y = m_position.getY() / m_tileSize;
    x2 = int(m_position.getX()) % m_tileSize;
    y2 = int(m_position.getY()) % m_tileSize;
    // loop over the Game screen cells
    for (int i=0; i<m_numRows; i++) {
        for (int j=0; j<m_numColumns; j++) {
            int id = m_tileIDs[i+y][j+x];
            if (id==0) continue; // if Tile value (id) = 0 at that position->nothing to render
            Tileset tileset = getTilesetByID(id);
            id--; // 'id' counts from 1->198 for Tileset1, 199->396 for Tileset2...
            TheTextureManager::Instance()->drawTile(
                tileset.name,
                tileset.margin, tileset.spacing,
                (j*m_tileSize)-x2, (i*m_tileSize)-y2, // destination to draw to (on the Game screen, in pixels)
                m_tileSize, m_tileSize,
                (id-(tileset.firstGridID-1))/tileset.numColumns, // which column in the Tileset image; firstGridID counts from 1
                (id-(tileset.firstGridID-1))%tileset.numColumns, // which row in the Tileset image
                TheGame::Instance()->getRenderer() 
            );
        }
    }
}

Tileset TileLayer::getTilesetByID(int tileID) {
    for (int i=0; i<m_tilesets.size(); i++) {
        // if the next Tileset exists
        if (i+1 <= m_tilesets.size()-1) {
            if (tileID >= m_tilesets[i].firstGridID && tileID < m_tilesets[i+1].firstGridID)
                return m_tilesets[i];
        } 
        // ... otherwise the needed tileID is within current Tileset
        else {
            return m_tilesets[i];
        }
    }
    std::cout << "did not find tileset, returning empty tileset\n";
    Tileset t;
    return t;
}