#pragma once 
#include "Level.h"
#include "tinyxml.h"

class LevelParser {
public:
  Level* parseLevel(const char* stateFile);

private:
  void parseTextures(TiXmlElement* pTextureRoot);
  void parseTilesets(TiXmlElement* pTilesetRoot, std::vector<Tileset>* pTilesets);
  // pLayers: ObjectLayer, TileLayer
  void parseObjectLayer(TiXmlElement* pObjectElement, std::vector<Layer*> *pLayers, Level* pLevel);
  void parseTileLayer(TiXmlElement* pTileElement, std::vector<Layer*>* pLayers, const std::vector<Tileset>* pTilesets, std::vector<TileLayer*> *pCollisionLayers);

  int m_tileSize; // 32 pixels
  // width, height: of entire TileMap, in terms of cells
  int m_width;
  int m_height;
};
// DONE