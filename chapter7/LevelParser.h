#pragma once 
#include "Level.h"

class LevelParser {
public:
    Level* parseLevel(const char* levelFile);
private: 
    int m_tileSize; 
    int m_width;
    int m_height;
};