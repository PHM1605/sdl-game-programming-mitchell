#include "LevelParser.h"
#include "tinyxml.h"
#include "TextureManager.h"
#include "Game.h"
#include "TileLayer.h"
#include "base64.h"
#include "zlib.h"
#include <string>

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
    for(TiXmlElement* e = pRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
    {
        if (e->Value() == std::string("tileset")) {
            parseTilesets(e, pLevel->getTilesets());
        }
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

void LevelParser::parseTileLayer(TiXmlElement* pTileElement, std::vector<Layer*>* pLayers, const std::vector<Tileset>* pTilesets) {
    // pTileElement points to: <layer id="1" name="Tile Layer 1" width="20" height="15">    
    // get <data encoding="base64" compression="zlib">...</data>
    TiXmlElement* pDataNode; 
    for (TiXmlElement* e = pTileElement->FirstChildElement(); e!=NULL; e=e->NextSiblingElement()) {
        if (e->Value() == std::string("data")) 
            pDataNode = e;
    }
    // storing IDs of Tiles on the Map
    std::string decodedIDs; 
    for (TiXmlNode* e=pDataNode->FirstChild(); e!=NULL; e=e->NextSibling()) {
        TiXmlText* text = e->ToText();
        std::string t = text->Value();
        decodedIDs = base64_decode(t);
    }    
    // uncompress zlib compression
    uLongf numGids = m_width * m_height * sizeof(int); // #bytes to store whole Map of ints
    std::vector<unsigned> gids(numGids); // increase the storage from bytes->uints
    uncompress((Bytef*)&gids[0], &numGids, (const Bytef*)decodedIDs.c_str(), decodedIDs.size());
    // parse from 'gids' to 'data' output
    std::vector<std::vector<int>> data;
    std::vector<int> layerRow(m_width);
    for (int j=0; j<m_height; j++)
        data.push_back(layerRow);
    for (int rows=0; rows<m_height; rows++) {
        for (int cols =0; cols<m_width; cols++) {
            data[rows][cols] = gids[rows*m_width+cols];
        }
    }
    // setup data into TileLayer
    TileLayer* pTileLayer = new TileLayer(m_tileSize, *pTilesets);
    pTileLayer->setTileIDs(data);
    pLayers->push_back(pTileLayer);
}