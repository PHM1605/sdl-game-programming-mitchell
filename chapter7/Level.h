#pragma once 
#include <string>
#include <vector>
#include "Layer.h"

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
private:
    // NOTE: we make Level's constructor private to ensure it must be created by its 'friend' class
    Level();
    friend class LevelParser;
    std::vector<Tileset> m_tilesets;
    std::vector<Layer*> m_layers;
};