#include "IndicatorNode.hpp"
#include "Config.hpp"
#include <algorithm>
#include <cmath>

using namespace geode::prelude;

namespace ci {
    static IndicatorNode* s_active = nullptr;

    // tag 1 = CCSprite (image), tag 0 = CCLayerColor (carre de secours)
    static void setNodeOpacity(CCNode* n, GLubyte o) {
        if (!n) return;
        if (n->getTag() == 1) {
            static_cast<CCSprite*>(n)->setOpacity(o);
        } else {
            static_cast<CCLayerColor*>(n)->setOpacity(o);
        }
    }

    IndicatorNode* IndicatorNode::create(bool editing) {
        auto ret = new IndicatorNode();
        if (ret && ret->setup(editing)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    IndicatorNode::~IndicatorNode() {
        if (s_active == this) s_active = nullptr;
    }

    void IndicatorNode::setActive(IndicatorNode* node) {
        s_active = node;
    }

    void IndicatorNode::notifyHeld(bool anyHeld) {
        if (s_active) s_active->setDown(anyHeld);
    }

    bool IndicatorNode::setup(bool editing) {
        if (!CCNode::init()) return false;
        m_editing = editing;
        this->reloadImages();
        this->applyConfig();
        this->setDown(false);
        if (!editing) {
            // Relit les reglages 4x/s pour suivre le menu Geode (negligeable)
            this->schedule(schedule_selector(IndicatorNode::tick), 0.25f);
        }
        return true;
    }

    void IndicatorNode::tick(float) {
        this->applyConfig();
    }

    CCNode* IndicatorNode::buildState(bool on) {
        if (auto path = findImage(on)) {
            // Chemin absolu : fonctionne sur iOS (Documents sandbox du launcher)
            auto const str = path->string();
            // Force le rechargement si le fichier a change
            CCTextureCache::sharedTextureCache()->removeTextureForKey(str.c_str());
            if (auto spr = CCSprite::create(str.c_str())) {
                spr->setTag(1);
                return spr;
            }
            log::warn("Click Indicator: echec chargement image {}", str);
        }
        // Image absente ou illisible : carre de secours
        auto layer = CCLayerColor::create(
            on ? ccc4(80, 255, 120, 255) : ccc4(60, 60, 60, 200),
            48.f, 48.f
        );
        layer->ignoreAnchorPointForPosition(false);
        layer->setAnchorPoint(ccp(0.5f, 0.5f));
        layer->setTag(0);
        return layer;
    }

    void IndicatorNode::reloadImages() {
        if (m_on) {
            m_on->removeFromParent();
            m_on = nullptr;
        }
        if (m_off) {
            m_off->removeFromParent();
            m_off = nullptr;
        }
        m_on = this->buildState(true);
        m_off = this->buildState(false);

        auto a = m_on->getContentSize();
        auto b = m_off->getContentSize();
        float w = std::max(a.width, b.width);
        float h = std::max(a.height, b.height);
        // La plus grande dimension vaut 56 points a taille 1.0
        float base = 56.f / std::max(1.f, std::max(w, h));

        m_on->setScale(base);
        m_off->setScale(base);
        m_on->setPosition(ccp(0.f, 0.f));
        m_off->setPosition(ccp(0.f, 0.f));
        this->addChild(m_off);
        this->addChild(m_on);
        m_half = CCSize(w * base * 0.5f, h * base * 0.5f);
    }

    void IndicatorNode::applyConfig() {
        auto cfg = loadConfig();
        this->setVisible(m_editing || (cfg.enabled && !cfg.hidden));
        this->setScale(cfg.scale);

        auto op = static_cast<GLubyte>(std::clamp(cfg.opacity, 0.f, 1.f) * 255.f);
        setNodeOpacity(m_on, op);
        setNodeOpacity(m_off, op);

        auto win = CCDirector::sharedDirector()->getWinSize();
        this->setPosition(ccp(cfg.x * win.width, cfg.y * win.height));
    }

    void IndicatorNode::setDown(bool down) {
        if (m_on) m_on->setVisible(down);
        if (m_off) m_off->setVisible(!down);
    }

    bool IndicatorNode::containsWorldPoint(CCPoint const& p) {
        auto local = this->convertToNodeSpace(p);
        float pad = 12.f / std::max(0.1f, this->getScale());
        return std::abs(local.x) <= m_half.width + pad
            && std::abs(local.y) <= m_half.height + pad;
    }
}
