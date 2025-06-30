#pragma once 
#include"Layer.h"
#include "Level.h"
#include "Vector2D.h"
#include <vector>

// 1 Layer of the whole Map
class TileLayer: public Layer {
public:
    TileLayer(int tileSize, const std::vector<Tileset> &tilesets);
    virtual ~TileLayer() {}

    virtual void update(Level* pLevel);
    virtual void render();
    // set TileIDs for the whole Map
    void setTileIDs(const std::vector<std::vector<int>>& data) { m_tileIDs = data; }
    void setTileSize(int tileSize) { m_tileSize = tileSize; }
    void setMapWidth(int mapWidth) { m_mapWidth = mapWidth; }
    int getTileSize() { return m_tileSize; }
    const std::vector<std::vector<int>>& getTileIDs() { return m_tileIDs; }
    Tileset getTilesetByID(int tileID);
    const Vector2D getPosition() { return m_position; }
private:
    // number of rows/columns of Game screen (usually smaller than number of rows/cols in full Map)
    int m_numColumns;
    int m_numRows;
    int m_tileSize; // e.g. small Tile of 32 pixels
    int m_mapWidth;
    // position/velocity of Game screen on this TileLayer
    Vector2D m_position;
    Vector2D m_velocity;
    // 2 Tilesets textures that create the Map
    const std::vector<Tileset> &m_tilesets;
    std::vector<std::vector<int>> m_tileIDs;
};