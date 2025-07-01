#pragma once
#include <string>
#include <map>
#include "GameObject.h"

class BaseCreator {
public:
    virtual GameObject* createGameObject() const = 0;
    virtual ~BaseCreator() {}
};

class GameObjectFactory {
public:
    static GameObjectFactory* Instance() {
        if (pInstance == nullptr)
            pInstance = new GameObjectFactory();
        return pInstance;
    }
    // register a Creator of typeID (e.g. GliderCreator)
    bool registerType(std::string typeID, BaseCreator* pCreator) {
        std::map<std::string, BaseCreator*>::iterator it = m_creators.find(typeID);
        // if type already registered, delete the passed-in pointer, do nothing else
        if (it != m_creators.end()) {
            delete pCreator;
            return false;
        }
        // if type not exists, add to map
        m_creators[typeID] = pCreator;
        return true;
    }

    // ask a Creator of 'typeID' (e.g. typeID="Glider") creating a GameObject 
    GameObject* create(std::string typeID) {
        std::map<std::string, BaseCreator*>::iterator it = m_creators.find(typeID);
        if (it == m_creators.end()) {
            std::cout << "could not find type: " << typeID << "\n";
            return 0;
        }
        BaseCreator* pCreator = (*it).second;
        return pCreator->createGameObject();
    }
private:
    GameObjectFactory() {}
    ~GameObjectFactory() {}
    std::map<std::string, BaseCreator*> m_creators;
    static GameObjectFactory* pInstance;
};

typedef GameObjectFactory TheGameObjectFactory;