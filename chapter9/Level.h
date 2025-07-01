#pragma once 
#include <string>

class TileLayer;

struct Tileset {
  int firstGridID;
  int tileWidth; 
  int tileHeight;
  int spacing;
  int margin;
  int width; // of whole Tileset .png
  int height; // of whole Tileset .png
  int numColumns;
  std::string name; 
};