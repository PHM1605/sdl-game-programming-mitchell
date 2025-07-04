#include "base64.h"
#include "Level.h"
#include "LevelParser.h"
#include "TextureManager.h"
#include "TileLayer.h"
#include "Game.h"
#include "ObjectLayer.h"
#include "zlib.h"

Level* LevelParser::parseLevel(const char* levelFile) {
  TiXmlDocument levelDocument;
  levelDocument.LoadFile(levelFile);
  Level* pLevel = new Level();
  // pRoot: <map version="1.0" orientation="orthogonal" width="200" height="15" tilewidth="32" tileheight="32">
  TiXmlElement* pRoot = levelDocument.RootElement();
  std::cout << "Loading level:\n" << "Version: " << pRoot->Attribute("version") << "\n";
  std::cout << "Width:" << pRoot->Attribute("width") << " - Height:" << pRoot->Attribute("height") << "\n"; // height in #cells
  pRoot->Attribute("tilewidth", &m_tileSize);
  pRoot->Attribute("width", &m_width);
  pRoot->Attribute("height", &m_height);
  
  //  <properties>
  //    <property name="clouds" value="assets/prehistoric.png"/>
  //    <property name="player" value="assets/player.png"/>
  //    <property name="snail" value="assets/snail.png"/>
  //  </properties>
  TiXmlElement* pProperties = pRoot->FirstChildElement();
  for (TiXmlElement* e = pProperties->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
    if (e->Value() == std::string("property")) {
      parseTextures(e);
    }
  }

  //  <tileset firstgid="1" name="blocks1" tilewidth="32" tileheight="32" spacing="2" margin="2">
  //    <image source="blocks1.png" width="614" height="376"/>
  //  </tileset>
  for (TiXmlElement* e = pRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
    if (e->Value() == std::string("tileset"))
      parseTilesets(e, pLevel->getTilesets());
  }

  // <objectgroup name="Object Layer 1" width="200" height="15">
  //   <object name="Player" type="Player" x="496" y="199" width="32" height="32">
  //     <properties>
  //       <property name="numFrames" value="3"/>
  //       <property name="textureHeight" value="60"/>
  //       <property name="textureID" value="player"/>
  //       <property name="textureWidth" value="40"/>
  //     </properties>
  //   </object>
  //   <object name="Snail" type="Snail" x="718" y="335" width="32" height="32">
  //   ...
  // </objectgroup>  
  // <layer name="Overlay" width="200" height="15">
  //   <properties>
  //    <property name="collidable" value="true"/>
  //   </properties>
  //   <data encoding="base64" compression="zlib">
  //     xxx
  //   </data>
  // </layer>
  for (TiXmlElement* e = pRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
    if (e->Value() == std::string("objectgroup") || e->Value() == std::string("layer")) {
      if (e->FirstChildElement()->Value() == std::string("object")) {
        // parseObjectLayer(e, pLevel->getLayers(), pLevel);
      } else if (e->FirstChildElement()->Value() == std::string("data")
        || (e->FirstChildElement()->NextSiblingElement() != 0 && e->FirstChildElement()->NextSiblingElement()->Value() == std::string("data"))) {
        parseTileLayer(e, pLevel->getLayers(), pLevel->getTilesets(), pLevel->getCollisionLayers());
      }
    }
  }
  std::cout << "#tile layers: " << pLevel->getLayers()->size() << std::endl; // "Bottom", "Collision", "Overlay", "Overlay 2"
  std::cout << "#collision layers: " <<pLevel->getCollisionLayers()->size() << std::endl; // "Collision"
  std::cout << "#tilesets: " << pLevel->getTilesets()->size() << std::endl;
  return pLevel;
}

// pTextureRoot: <property name="clouds" value="assets/prehistoric.png"/>
void LevelParser::parseTextures(TiXmlElement* pTextureRoot) {
  std::cout << "adding texture " << pTextureRoot->Attribute("value") << " with ID " << pTextureRoot->Attribute("name") << std::endl;
  TheTextureManager::Instance()->load(pTextureRoot->Attribute("value"), pTextureRoot->Attribute("name"), TheGame::Instance()->getRenderer());
}

//  <tileset firstgid="1" name="blocks1" tilewidth="32" tileheight="32" spacing="2" margin="2">
//    <image source="blocks1.png" width="614" height="376"/>
//  </tileset>
void LevelParser::parseTilesets(TiXmlElement* pTilesetRoot, std::vector<Tileset>* pTilesets) {
  std::string assetsTag = "assets/";
  std::cout << "adding texture " << pTilesetRoot->FirstChildElement()->Attribute("source") << " with ID " << pTilesetRoot->Attribute("name") << std::endl;
  TheTextureManager::Instance()->load(
    assetsTag.append(pTilesetRoot->FirstChildElement()->Attribute("source")), // "blocks1.png"
    pTilesetRoot->Attribute("name"), // "blocks1"
    TheGame::Instance()->getRenderer()
  );

  Tileset tileset;
  pTilesetRoot->FirstChildElement()->Attribute("width", &tileset.width); 
  pTilesetRoot->FirstChildElement()->Attribute("height", &tileset.height);
  pTilesetRoot->Attribute("firstgid", &tileset.firstGridID);
  pTilesetRoot->Attribute("tilewidth", &tileset.tileWidth); // 32 pixels
  pTilesetRoot->Attribute("tileheight", &tileset.tileHeight); // 32 pixels
  pTilesetRoot->Attribute("spacing", &tileset.spacing); // 2 pixels
  pTilesetRoot->Attribute("margin", &tileset.margin);
  
  tileset.name = pTilesetRoot->Attribute("name");
  tileset.numColumns = tileset.width / (tileset.tileWidth + tileset.spacing);

  pTilesets->push_back(tileset);
}

// <objectgroup name="Object Layer 2" width="200" height="15">
//  <object name="Background" type="ScrollingBackground" x="0" y="0" width="32" height="32">
//    <properties>
//      <property name="animSpeed" value="0.2"/>
//      <property name="numFrames" value="1"/>
//      <property name="textureHeight" value="480"/>
//      <property name="textureID" value="clouds"/>
//      <property name="textureWidth" value="640"/>
//    </properties>
//  </object>
// </objectgroup>
void LevelParser::parseObjectLayer(TiXmlElement* pObjectElement, std::vector<Layer*>*pLayers, Level* pLevel) {
  ObjectLayer* pObjectLayer = new ObjectLayer();
  for (TiXmlElement* e = pObjectElement->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
    // e: <object>
    if (e->Value() == std::string("object")) {
      int x, y, width, height, numFrames, callbackID = 0, animSpeed = 0;
      std::string textureID;
      std::string type;
      // x, y: location on the whole big Map
      e->Attribute("x", &x);
      e->Attribute("y", &y);
      // type: name of class of Object we must create e.g. "ScrollingBackground"
      type = e->Attribute("type");
      GameObject* pGameObject = TheGameObjectFactory::Instance()->create(type);
      for (TiXmlElement* properties = e->FirstChildElement(); properties != NULL; properties = properties->NextSiblingElement()) {
        if (properties->Value() == std::string("properties")) {
          for (TiXmlElement* property = properties->FirstChildElement(); property != NULL; property = property->NextSiblingElement()) {
            if (property->Value() == std::string("property")) {
              if (property->Attribute("name") == std::string("numFrames"))
                property->Attribute("value", &numFrames);
              else if (property->Attribute("name") == std::string("textureID"))
                textureID = property->Attribute("value");
              else if (property->Attribute("name") == "textureWidth") 
                property->Attribute("value", &width); // of the Texture image
              else if (property->Attribute("name") == std::string("textureHeight"))
                property->Attribute("value", &height); // of the Texture image
              else if (property->Attribute("name") == std::string("callbackID"))
                property->Attribute("value", &callbackID);
              else if (property->Attribute("name") == std::string("animSpeed"))
                property->Attribute("value", &animSpeed);
            }
          }
        }
      }

      pGameObject->load(std::unique_ptr<LoaderParams>(new LoaderParams(x, y, width, height, textureID, numFrames, callbackID, animSpeed)));
      // set collision layers for every Objects
      pGameObject->setCollisionLayers(pLevel->getCollisionLayers());
      if (type == "Player") 
        pLevel->setPlayer(dynamic_cast<Player*>(pGameObject));

      pObjectLayer->getGameObjects()->push_back(pGameObject);
    }
  }
  pLayers->push_back(pObjectLayer);
}
// pTileElement:
//  <layer name="Overlay" width="200" height="15">
//    <properties>
//      <property name="collidable" value="true"/>
//    </properties>
//    <data encoding="base64" compression="zlib">
//      xxx
//    </data>
//  </layer>
// pLayers: pointing to vector of TileLayer, ObjectLayer
// pCollisionLayers: pointing to vector of TileLayers
void LevelParser::parseTileLayer(TiXmlElement* pTileElement, std::vector<Layer*> *pLayers, const std::vector<Tileset> *pTilesets, std::vector<TileLayer*> *pCollisionLayers) {
  // m_width, m_height: [cell]
  TileLayer* pTileLayer = new TileLayer(m_tileSize, m_width, m_height, *pTilesets);
  bool collidable = false;
  std::vector<std::vector<int>> data;
  std::string decodedIDs; 
  TiXmlElement* pDataNode;
  for (TiXmlElement* e = pTileElement->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
    if (e->Value() == std::string("properties")) {
      for (TiXmlElement* property = e->FirstChildElement(); property != NULL; property = property->NextSiblingElement()) {
        if (property->Value() == std::string("property")) {
          if (property->Attribute("name") == std::string("collidable"))
            collidable = true;
        }
      }
    }

    if (e->Value() == std::string("data"))
      pDataNode = e;
  }
  // pDataNode now points to <data/>
  for (TiXmlNode* e = pDataNode->FirstChild(); e != NULL; e = e->NextSibling()) {
    TiXmlText* text = e->ToText();
    std::string t = text->Value();
    decodedIDs = base64_decode(t);
  }
  // uncompress zlib compression for tileIDs data
  uLongf sizeofids = m_width * m_height * sizeof(int); // size of whole Tilemap of ints
  std::vector<int> ids(m_width * m_height);
  uncompress((Bytef*)&ids[0], &sizeofids, (const Bytef*)decodedIDs.c_str(), decodedIDs.size());
  // create buffer to store data in terms of vector<vector<int>>
  std::vector<int> layerRow(m_width);
  for (int j=0; j<m_height; j++) {
    data.push_back(layerRow);
  }
  // storing data into vector<vector<int>>
  for (int rows = 0; rows<m_height; rows++){
    for (int cols = 0; cols<m_width; cols++) {
      data[rows][cols] = ids[rows*m_width+cols];
    }
  }
  pTileLayer->setTileIDs(data);
  // store TileLayer with 'collidable' property as a separate vector
  if (collidable) {
    pCollisionLayers->push_back(pTileLayer);
  }
  pLayers->push_back(pTileLayer);
}