#include "StateParser.h"
#include "TextureManager.h"
#include "Game.h"
#include "GameObjectFactory.h"

bool StateParser::parseState(const char* stateFile, std::string stateID, std::vector<GameObject*> *pObjects, std::vector<std::string> *pTextureIDs) {
    TiXmlDocument xmlDoc;
    if (!xmlDoc.LoadFile(stateFile)) {
        std::cerr << xmlDoc.ErrorDesc() << "\n";
        return false;
    }
    TiXmlElement* pRoot = xmlDoc.RootElement(); // <STATES>
    // pick out the relevant State (e.g. <MENU>)
    TiXmlElement* pStateRoot = 0; 
    for (TiXmlElement* e = pRoot->FirstChildElement(); e != NULL; e=e->NextSiblingElement()) {
        if (e->Value() == stateID) 
            pStateRoot = e;
    }
    // pick the <TEXTURES> node
    TiXmlElement* pTextureRoot = 0;
    for (TiXmlElement* e = pStateRoot->FirstChildElement(); e!=NULL; e=e->NextSiblingElement()) {
        if (e->Value() == std::string("TEXTURES"))
            pTextureRoot = e;
    }
    parseTextures(pTextureRoot, pTextureIDs);
    // pick the <OBJECTS> node
    TiXmlElement* pObjectRoot = 0;
    for (TiXmlElement* e = pStateRoot->FirstChildElement(); e!=NULL; e=e->NextSiblingElement()) 
        if (e->Value() == std::string("OBJECTS")) {
            pObjectRoot = e;
        }
            
    
    parseObjects(pObjectRoot, pObjects);
    return true;
}

// get children of <TEXTURES>
void StateParser::parseTextures(TiXmlElement* pStateRoot, std::vector<std::string>* pTextureIDs) {
    for (TiXmlElement* e = pStateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
        std::string filenameAttribute = e->Attribute("filename"); // "assets/button.png"
        std::string idAttribute = e->Attribute("ID"); // "playbutton"
        pTextureIDs->push_back(idAttribute); 
        TheTextureManager::Instance()->load(filenameAttribute, idAttribute, TheGame::Instance()->getRenderer());
    }
}

// create object with GameObjectFactory 
// <object type="MenuButton" x="100" y="100" width="400" height="100" textureID="playbutton" numFrames="0" callbackID="1"/>
void StateParser::parseObjects(TiXmlElement* pStateRoot, std::vector<GameObject*> *pObjects) {
    // pStateRoot: pointing to <OBJECTS>
    for (TiXmlElement* e = pStateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
        // e: pointing to <object/>
        int x, y , width, height, numFrames, callbackID, animSpeed;
        std::string textureID;
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