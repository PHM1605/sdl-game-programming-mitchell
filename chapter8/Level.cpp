#include "Level.h"

Level::Level() {
}

Level::~Level() {
    for (int i=0; i<m_layers.size(); i++){
        delete m_layers[i];
    }
    m_layers.clear();
}

void Level::render() {
    for (int i=0; i < m_layers.size(); i++)
        m_layers[i]->render();
}

void Level::update() {
    for (int i=0; i < m_layers.size(); i++)
        m_layers[i]->update();
}