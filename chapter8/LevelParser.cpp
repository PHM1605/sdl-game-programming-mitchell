#include "LevelParser.h"
#include "tinyxml.h"
#include "TextureManager.h"
#include "Game.h"
#include "TileLayer.h"
#include "base64.h"
#include "zlib.h"
#include "ObjectLayer.h"
#include <string>

Level* LevelParser::parseLevel(const char* levelFile) {
    TiXmlDocument levelDocument;    
    levelDocument.LoadFile(levelFile);
    Level* pLevel = new Level();
    // pRoot: <map version="1.0" orientation="orthogonal" width="60" height="15" tilewidth="32" tileheight="32">
    // width, height: of entire TileMap, in terms of cells
    TiXmlElement* pRoot = levelDocument.RootElement();
    pRoot->Attribute("tilewidth", &m_tileSize);
    pRoot->Attribute("width", &m_width);
    pRoot->Attribute("height", &m_height);
    // we know <properties> is the 1st property; saving Textures
    TiXmlElement* pProperties = pRoot->FirstChildElement();
    // get many <property name="helicopter" value="assets/helicopter.png"/>
    for (TiXmlElement* e = pProperties->FirstChildElement(); e!=NULL; e=e->NextSiblingElement()) {
        if (e->Value() == std::string("property"))
            parseTextures(e);
    }    
    // get many <tileset firstgid="1" source="tileset1.tsx"/> 
    for(TiXmlElement* e = pRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
    {
        if (e->Value() == std::string("tileset")) {
            parseTilesets(e, pLevel->getTilesets());
        }
    }
    // Option 1:
    //      <objectgroup name="Object Layer 2" width="150" height="15">
    //          <object name="Background" type="ScrollingBackground" x="0" y="0" width="32" height="32">
    //              <properties>
    //                  <property name="animSpeed" value="0.2"/>
    //                  <property name="numFrames" value="1"/>
    //                  <property name="textureHeight" value="480"/>
    //                  <property name="textureID" value="clouds2"/>
    //                  <property name="textureWidth" value="640"/>
    //              </properties>
    //          </object>
    //      </objectgroup>
    // Option 2:
    //      <layer name="Bottom" width="150" height="15">
    //          <data encoding="base64" compression="zlib">
    //          ...</data>
    //      </layer>  
    // Option 3:
    //      <layer name="Collision" width="150" height="15">
    //          <properties>
    //              <property name="collidable" value="true"/>
    //          </properties>
    //          <data encoding="base64" compression="zlib">
    //          ...</data>
    //      </layer>
    for (TiXmlElement* e=pRoot->FirstChildElement(); e!=NULL; e=e->NextSiblingElement()) {
        if (e->Value() == std::string("objectgroup") || e->Value() == std::string("layer")) {
            // Object Layer (Option 1)
            if (e->FirstChildElement()->Value() == std::string("object")) {
                parseObjectLayer(e, pLevel->getLayers(), pLevel);
            }  
            // // <layer> with <data> only (Option 2) OR <layer> with Collidable <properties> 
            // else if (e->FirstChildElement()->Value() == std::string("data") || 
            //     (e->FirstChildElement()->NextSiblingElement()!=0 && e->FirstChildElement()->NextSiblingElement()->Value() == std::string("data")) ) {
            //     parseTileLayer(e, pLevel->getLayers(), pLevel->getTilesets(), pLevel->getCollisionLayers());
            // }
        }            
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

void LevelParser::parseTileLayer(TiXmlElement* pTileElement, std::vector<Layer*>* pLayers, const std::vector<Tileset>* pTilesets, std::vector<TileLayer*>* pCollisionLayers) {
    bool collidable = false;
    TiXmlElement* pDataNode; 
    // pTileElement points to: <layer id="1" name="Tile Layer 1" width="20" height="15">    
    for (TiXmlElement* e = pTileElement->FirstChildElement(); e!=NULL; e=e->NextSiblingElement()) {
        // OR (collidable) <properties> before <data/>
        if (e->Value() == std::string("properties")) {
            // many <property>s
            for (TiXmlElement* property = e->FirstChildElement(); property!=NULL; property=property->NextSiblingElement()) {
                if (property->Value() == std::string("property")) {
                    if (property->Attribute("name") == std::string("collidable"))
                        collidable = true;
                }
            }
        }
        // get <data encoding="base64" compression="zlib">...</data>
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
    pTileLayer->setMapWidth(m_width);
    if (collidable) {
        pCollisionLayers->push_back(pTileLayer);
    }
    pLayers->push_back(pTileLayer);
}

// <property name="helicopter" value="helicopter.png"/>
void LevelParser::parseTextures(TiXmlElement* pTextureRoot) {
    bool tmp = TheTextureManager::Instance()->load(pTextureRoot->Attribute("value"), pTextureRoot->Attribute("name"), TheGame::Instance()->getRenderer());
}
// pObjectElement: <objectgroup id="6" name="Object Layer 1">
void LevelParser::parseObjectLayer(TiXmlElement* pObjectElement, std::vector<Layer*>* pLayers, Level* pLevel) {
    ObjectLayer* pObjectLayer = new ObjectLayer();
    
    for (TiXmlElement* e = pObjectElement->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
        // e: <object name="Glider1" type="Glider" x="1505" y="149" width="32" height="32">
        if (e->Value() == std::string("object")) {
            // numFrames: how many frames that GameObject's animation has
            // width, height: of 1 frame of animation of helicopter's .png
            int numFrames, width, height;
            int callbackID = 0, animSpeed = 0;
            // x, y: position of GameObject on the whole Map
            double x, y;
            // string that maps name & a Texture file
            std::string textureID;
            std::string type;
            e->Attribute("x", &x);
            e->Attribute("y", &y);
            type = e->Attribute("type");
            GameObject* pGameObject = TheGameObjectFactory::Instance()->create(type); // "Player"/"Glider"
            
            // properties: <properties>
            for (TiXmlElement* properties=e->FirstChildElement(); properties!=NULL; properties=properties->NextSiblingElement()) {
                if (properties->Value() == std::string("properties")) {
                    // property: <property name="numFrames" value="4"/>
                    for (TiXmlElement* property=properties->FirstChildElement(); property!=NULL; property=property->NextSiblingElement()) {
                        if (property->Value() == std::string("property")) {
                            // <property name="numFrames" value="4"/>
                            if (property->Attribute("name") == std::string("numFrames"))
                                property->Attribute("value", &numFrames);
                            // <property name="textureHeight" value="55"/>
                            else if (property->Attribute("name") == std::string("textureHeight")) 
                                property->Attribute("value", &height);
                            // <property name="textureWidth" value="128"/>
                            else if (property->Attribute("name") == std::string("textureWidth"))
                                property->Attribute("value", &width);
                            // <property name="textureID" value="helicopter"/>
                            else if (property->Attribute("name") == std::string("textureID")){
                                textureID = property->Attribute("value");
                            }
                                
                            else if (property->Attribute("name") == std::string("callbackID"))
                                property->Attribute("value", &callbackID);
                            else if (property->Attribute("name") == std::string("animSpeed"))
                                property->Attribute("value", &animSpeed);
                        }
                    }
                }
            }
            pGameObject->load(std::unique_ptr<LoaderParams>(
                new LoaderParams((int)x, (int)y, width, height, textureID, numFrames, callbackID, animSpeed)));
            if (type=="Player")
                pLevel->setPlayer(dynamic_cast<Player*>(pGameObject));
            pObjectLayer->getGameObjects()->push_back(pGameObject);
        }
    }
    pLayers->push_back(pObjectLayer);
}