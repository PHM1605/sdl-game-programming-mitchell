#pragma once 
#include "Level.h"
#include "tinyxml.h"

class LevelParser {
public:
    Level* parseLevel(const char* levelFile);

private: 
    // get <tileset> elements
    void parseTilesets(TiXmlElement* pTilesetRoot, std::vector<Tileset>* pTilesets);
    // get <layer> elements
    void parseTileLayer(TiXmlElement* pTileElement, std::vector<Layer*> *pLayers, const std::vector<Tileset> *pTilesets);
    void parseTextures(TiXmlElement* pTextureRoot);
    void parseObjectLayer(TiXmlElement* pObjectElement, std::vector<Layer*> *pLayers);
    int m_tileSize; 
    int m_width;
    int m_height;
};