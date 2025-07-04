#include "Game.h"
#include "InputHandler.h"
#include "MainMenuState.h"
#include "MenuButton.h"
#include "PauseState.h"
#include "StateParser.h"
#include "TextureManager.h"

const std::string PauseState::s_pauseID = "PAUSE";

void PauseState::s_pauseToMain() {
    TheGame::Instance()->getStateMachine()->changeState(new MainMenuState());
}

void PauseState::s_resumePlay() {
    TheGame::Instance()->getStateMachine()->popState();
}

void PauseState::update() {
    if (m_loadingComplete && !m_gameObjects.empty()) {
        for (int i=0; i<m_gameObjects.size(); i++) {
            m_gameObjects[i]->update();
        }
    }
}

void PauseState::render() {
    if (m_loadingComplete && !m_gameObjects.empty()) {
        for (int i=0; i<m_gameObjects.size(); i++) {
            m_gameObjects[i]->draw();
        }
    }
}

bool PauseState::onEnter() {
    StateParser stateParser;
    stateParser.parseState("assets/conan.xml", s_pauseID, &m_gameObjects, &m_textureIDList); // filling in the last 2 arguments

    m_callbacks.push_back(0);
    m_callbacks.push_back(s_pauseToMain);
    m_callbacks.push_back(s_resumePlay);
    setCallbacks(m_callbacks);
    m_loadingComplete = true;
    
    std::cout << "entering PauseState\n";
    return true;
}

bool PauseState::onExit() {
    // clear GameObjects
    if (m_loadingComplete && !m_gameObjects.empty()) {
        for (int i=0; i<m_gameObjects.size(); i++) {
            m_gameObjects[i]->clean();
            delete m_gameObjects[i];
        }
        m_gameObjects.clear();
    }
    // clear TextureManager
    for (int i=0; i<m_textureIDList.size(); i++) {
        TheTextureManager::Instance()->clearFromTextureMap(m_textureIDList[i]);
    }
    TheInputHandler::Instance()->reset();

    std::cout << "exiting PauseState\n";
    return true;
}

void PauseState::setCallbacks(const std::vector<Callback> &callbacks) {
    if (!m_gameObjects.empty()) {
        for (int i=0; i<m_gameObjects.size(); i++) {
            if (dynamic_cast<MenuButton*>(m_gameObjects[i])) {
                MenuButton* pButton = dynamic_cast<MenuButton*>(m_gameObjects[i]);
                pButton->setCallback(callbacks[pButton->getCallbackID()]);
            }
        }
    }
}
// DONE