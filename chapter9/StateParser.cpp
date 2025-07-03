#include <string>
#include <vector>
#include "Game.h"
#include "GameObjectFactory.h"
#include "StateParser.h"
#include "TextureManager.h"

// parse file 'conan.xml'
// <MENU>
//   <TEXTURES>
//     <texture filename="assets/button.png" ID="playbutton"/>
//     <texture filename="assets/exit.png" ID="exitbutton"/>
//   </TEXTURES>
  
//   <OBJECTS>
//     <object type="MenuButton" x="100" y="100" width="400" height="100" textureID="playbutton" numFrames="0" callbackID="1"/>
//     <object type="MenuButton" x="100" y="300" width="400" height="100" textureID="exitbutton" numFrames="0" callbackID="2"/>
//   </OBJECTS>
// </MENU>

// stateID: "MENU"/ "PLAY"/ "PAUSE"/ "GAMEOVER"
// pObjects, pTextureIDs: parsing result of <OBJECTS>
// pTextureIDs: parsing result of <TEXTURES>
bool StateParser::parseState(const char* stateFile, std::string stateID, std::vector<GameObject*>* pObjects, std::vector<std::string>* pTextureIDs) {
  TiXmlDocument xmlDoc;
  if (!xmlDoc.LoadFile(stateFile)) {
    std::cerr << xmlDoc.ErrorDesc() << "\n";
    return false;
  }
  TiXmlElement* pRoot = xmlDoc.RootElement(); // <States>

  // Get <MENU> tag 
  TiXmlElement* pStateRoot = nullptr;
  for (TiXmlElement *e = pRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
    if (e->Value() == stateID) {
      pStateRoot = e; // <MENU>
      break;
    }      
  }

  // Get <TEXTURES> tag
  TiXmlElement* pTextureRoot = nullptr;
  for (TiXmlElement* e = pStateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
    if (e->Value() == std::string("TEXTURES")) {
      pTextureRoot = e; // <TEXTURES>
      break;
    }
  }
  parseTextures(pTextureRoot, pTextureIDs);
  // Get <OBJECTS> tag
  TiXmlElement* pObjectRoot = nullptr;
  for (TiXmlElement* e = pStateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
    if (e->Value() == std::string("OBJECTS")) {
      pObjectRoot = e;
      break;
    }
  }
  parseObjects(pObjectRoot, pObjects);
  return true;
}

// pStateRoot: <TEXTURES> tag
// pTextureIDs: store parsing result
void StateParser::parseTextures(TiXmlElement* pStateRoot, std::vector<std::string> *pTextureIDs) {
  for (TiXmlElement* e = pStateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
    // e: <texture filename="assets/button.png" ID="playbutton"/>
    std::string filenameAttribute = e->Attribute("filename");
    std::string idAttribute = e->Attribute("ID");
    pTextureIDs->push_back(idAttribute);
    TheTextureManager::Instance()->load(filenameAttribute, idAttribute, TheGame::Instance()->getRenderer());
  }
}

// pStateRoot: <OBJECTS> tag
// pObjects: store passing result
void StateParser::parseObjects(TiXmlElement* pStateRoot, std::vector<GameObject*>* pObjects) {
  for (TiXmlElement* e = pStateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
    int x, y, width, height, numFrames, callbackID, animSpeed;
    std::string textureID;
    // e: <object type="MenuButton" x="100" y="100" width="400" height="100" textureID="playbutton" numFrames="0" callbackID="1"/>
    e->Attribute("x", &x);
    e->Attribute("y", &y);
    e->Attribute("width", &width);
    e->Attribute("height", &height);
    e->Attribute("numFrames", &numFrames);
    e->Attribute("callbackID", &callbackID);
    e->Attribute("animSpeed", &animSpeed);
    textureID = e->Attribute("textureID");

    GameObject* pGameObject = TheGameObjectFactory::Instance()->create(e->Attribute("type"));
    pGameObject->load(std::unique_ptr<LoaderParams>(new LoaderParams(x, y, width, height, textureID, numFrames, callbackID, animSpeed))); 
    pObjects->push_back(pGameObject);
  }
}
// DONE