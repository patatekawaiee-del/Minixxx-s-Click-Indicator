#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../IndicatorNode.hpp"
#include "../Input.hpp"

using namespace geode::prelude;

// Point d'entree unique des clics / appuis.
// Appele par le moteur pour chaque press/release, tous modes de jeu
// (cube, ship, wave, ufo, ball, robot, spider) et toutes sources
// (tactile iOS, manette, clavier).
// Compatible JIT-less : pure $modify, aucun hook dynamique.
class $modify(CIBaseGameLayer, GJBaseGameLayer) {
    void handleButton(bool down, int button, bool isPlayer1) {
        GJBaseGameLayer::handleButton(down, button, isPlayer1);
        ci::onButton(down, button, isPlayer1);
    }
};

class $modify(CIPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        if (auto node = ci::IndicatorNode::create(false)) {
            // m_uiLayer est fixe a l'ecran (ne suit pas la camera du niveau)
            if (m_uiLayer) {
                m_uiLayer->addChild(node, 9999);
                ci::IndicatorNode::setActive(node);
            }
        }
        ci::resetHeld();
        return true;
    }

    void resetLevel() {
        PlayLayer::resetLevel();
        ci::resetHeld();
    }

    void onQuit() {
        ci::IndicatorNode::setActive(nullptr);
        ci::resetHeld();
        PlayLayer::onQuit();
    }
};
