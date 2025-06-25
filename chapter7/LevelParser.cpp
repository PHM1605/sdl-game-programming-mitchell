#include "LevelParser.h"
#include "tinyxml.h"
#include "TextureManager.h"
#include "Game.h"

Level* LevelParser::parseLevel(const char* levelFile) {
    TiXmlDocument levelDocument;
    levelDocument.LoadFile(levelFile);
    Level* pLevel = new Level();
    // get <map version="1.0" orientation="orthogonal" width="60" height="15" tilewidth="32" tileheight="32">
    TiXmlElement* pRoot = levelDocument.RootElement();
    pRoot->Attribute("tilewidth", &m_tileSize);
    pRoot->Attribute("width", &m_width);
    pRoot->Attribute("height", &m_height);
    // get many <tileset firstgid="1" source="tileset1.tsx"/> 
    for (TiXmlElement* e=pRoot->FirstChildElement(); e!=NULL; e=e->NextSiblingElement()) {
        if (e->Value() == std::string("tileset"))
            parseTilesets(e, pLevel->getTilesets());
    }
    // get many <layer id="1" name="Tile Layer 1" width="20" height="15">
    for (TiXmlElement* e=pRoot->FirstChildElement(); e!=NULL; e=e->NextSiblingElement()) {
        if (e->Value() == std::string("layer"))
            parseTileLayer(e, pLevel->getLayers(), pLevel->getTilesets());
    }
    return pLevel;
}

void LevelParser::parseTilesets(TiXmlElement* pTilesetRoot, std::vector<Tileset>* pTilesets) {
    // pTilesetRoot: pointing 
    // <tileset firstgid="1" name="blocks1" tileWidth="32" tileHeight="32" spacing="2" margin="2"> 
    //   <image source="blocks1.png" width="614" height="376"/>
    // </tileset>
    TheTextureManager::Instance()->load(
        pTilesetRoot->FirstChildElement()->Attribute("source"),
        pTilesetRoot->Attribute("name"),
        TheGame::Instance()->getRenderer()
    );
    Tileset tileset;
    pTilesetRoot->FirstChildElement()->Attribute("width", &tileset.width); // 614
    pTilesetRoot->FirstChildElement()->Attribute("height", &tileset.height); // 376
    pTilesetRoot->Attribute("firstgid", &tileset.firstGridID);
    pTilesetRoot->Attribute("tilewidth", &tileset.tileWidth);
    pTilesetRoot->Attribute("tileheight", &tileset.tileHeight);
    pTilesetRoot->Attribute("spacing", &tileset.spacing);
    pTilesetRoot->Attribute("margin", &tileset.margin);
    tileset.name = pTilesetRoot->Attribute("name");
    tileset.numColumns = tileset.width / (tileset.tileWidth + tileset.spacing);
    pTilesets->push_back(tileset);
}

