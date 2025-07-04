#include "AnimatedGraphic.h"
#include "Game.h"
#include "GameOverState.h"
#include "InputHandler.h"
#include "MainMenuState.h"
#include "MenuButton.h"
#include "PlayState.h"
#include "StateParser.h"
#include "TextureManager.h"

const std::string  GameOverState::s_gameOverID = "GAMEOVER";

void GameOverState::s_gameOverToMain() {
    TheGame::Instance()->getStateMachine()->changeState(new MainMenuState());
}

void GameOverState::s_restartPlay() {
    TheGame::Instance()->getStateMachine()->changeState(new PlayState());
}

void GameOverState::update() {
    if (m_loadingComplete && !m_gameObjects.empty()) {
        for (int i = 0; i < m_gameObjects.size(); i++)
            m_gameObjects[i]->update();
    }
}

void GameOverState::render() {
    if (m_loadingComplete && !m_gameObjects.empty()) {
        for (int i=0; i<m_gameObjects.size(); i++) 
            m_gameObjects[i]->draw();
    }
}

bool GameOverState::onEnter() {
    StateParser stateParser;
    stateParser.parseState("assets/conan.xml", s_gameOverID, &m_gameObjects, &m_textureIDList); // fill in the last 2 arguments
    m_callbacks.push_back(0);
    m_callbacks.push_back(s_gameOverToMain);
    m_callbacks.push_back(s_restartPlay);
    setCallbacks(m_callbacks);
    m_loadingComplete = true;
    std::cout << "entering GameOverState\n";
    return true;
}

bool GameOverState::onExit() {
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
    
    std::cout << "exiting GameOverState\n";
    return true;
}

void GameOverState::setCallbacks(const std::vector<Callback>& callbacks) {
    for (int i=0; i<m_gameObjects.size(); i++) {
        if (dynamic_cast<MenuButton*>(m_gameObjects[i])) {
            MenuButton* pButton = dynamic_cast<MenuButton*>(m_gameObjects[i]);
            pButton->setCallback(callbacks[pButton->getCallbackID()]);
        }
    }
}
// DONE