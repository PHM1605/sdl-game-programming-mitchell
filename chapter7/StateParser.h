#pragma once
#include <vector>
#include "GameObject.h"
#include "tinyxml.h"

class StateParser {
public:
    // parse the whole .xml file
    bool parseState(const char* stateFile, std::string stateID, std::vector<GameObject*> *pObjects, std::vector<std::string> *pTextureIDs);
private:
    void parseObjects(TiXmlElement* pStateRoot, std::vector<GameObject*> *pObjects); // e.g. Button, Player, Enemy
    void parseTextures(TiXmlElement* pStateRoot, std::vector<std::string> *pTextureIDs);
};