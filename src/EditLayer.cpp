#include "EditLayer.hpp"
#include "Config.hpp"
#include <algorithm>

using namespace geode::prelude;

namespace ci {
    void EditLayer::open() {
        auto scene = CCDirector::sharedDirector()->getRunningScene();
        if (!scene) return;
        auto layer = new EditLayer();
        if (layer && layer->setup()) {
            layer->autorelease();
            scene->addChild(layer, 99999);
        } else {
            delete layer;
        }
    }

    bool EditLayer::setup() {
        if (!CCLayer::init()) return false;
        auto win = CCDirector::sharedDirector()->getWinSize();

        this->addChild(CCLayerColor::create(ccc4(0, 0, 0, 160), win.width, win.height));

        auto title = CCLabelBMFont::create("Click Indicator - mode EDIT", "bigFont.fnt");
        title->setScale(0.55f);
        title->setPosition(ccp(win.width / 2.f, win.height - 18.f));
        this->addChild(title);

        auto hint = CCLabelBMFont::create("Glisse l'indicateur pour le placer", "chatFont.fnt");
        hint->setScale(0.7f);
        hint->setPosition(ccp(win.width / 2.f, win.height - 40.f));
        this->addChild(hint);

        m_node = IndicatorNode::create(true);
        if (!m_node) return false;
        this->addChild(m_node, 5);

        m_info = CCLabelBMFont::create("", "chatFont.fnt");
        m_info->setPosition(ccp(win.width / 2.f, 112.f));
        m_info->setScale(0.7f);
        this->addChild(m_info);

        m_files = CCLabelBMFont::create("", "chatFont.fnt");
        m_files->setPosition(ccp(win.width / 2.f, 95.f));
        this->addChild(m_files);

        auto menu = CCMenu::create();
        menu->setPosition(ccp(0.f, 0.f));
        // Priorite devant la couche (-500) pour que les boutons recoivent les touches
        menu->setHandlerPriority(-501);
        this->addChild(menu, 10);

        float cx = win.width / 2.f;
        this->addButton(menu, "Taille -",   menu_selector(EditLayer::onAdjust), 1, ccp(cx - 135.f, 62.f));
        this->addButton(menu, "Taille +",   menu_selector(EditLayer::onAdjust), 2, ccp(cx - 45.f, 62.f));
        this->addButton(menu, "Opacite -",  menu_selector(EditLayer::onAdjust), 3, ccp(cx + 45.f, 62.f));
        this->addButton(menu, "Opacite +",  menu_selector(EditLayer::onAdjust), 4, ccp(cx + 135.f, 62.f));
        this->addButton(menu, "Apercu ON/OFF", menu_selector(EditLayer::onPreview), 0, ccp(cx - 100.f, 26.f));
        this->addButton(menu, "Masquer",    menu_selector(EditLayer::onHide),    0, ccp(cx, 26.f));
        this->addButton(menu, "Reset",      menu_selector(EditLayer::onReset),   0, ccp(cx + 70.f, 26.f));
        this->addButton(menu, "Fermer",     menu_selector(EditLayer::onClose),   0, ccp(win.width - 40.f, win.height - 22.f));

        this->refreshInfo();
        this->setTouchEnabled(true);
        // Mode cible (un doigt) + avalement des touches
        this->setTouchMode(kCCTouchesOneByOne);
        return true;
    }

    void EditLayer::addButton(CCMenu* menu, char const* text, SEL_MenuHandler cb, int tag, CCPoint pos) {
        auto spr = ButtonSprite::create(text);
        spr->setScale(0.5f);
        auto btn = CCMenuItemSpriteExtra::create(spr, this, cb);
        btn->setTag(tag);
        btn->setPosition(pos);
        menu->addChild(btn);
    }

    void EditLayer::refreshInfo() {
        auto cfg = loadConfig();
        auto win = CCDirector::sharedDirector()->getWinSize();
        m_info->setString(fmt::format(
            "Taille {:.1f}   Opacite {:.1f}   Masque en jeu : {}",
            cfg.scale, cfg.opacity, cfg.hidden ? "oui" : "non"
        ).c_str());

        m_files->setString(fmt::format(
            "on.png : {}   off.png : {}\nDossier : {}",
            findImage(true) ? "trouve" : "ABSENT (carre vert)",
            findImage(false) ? "trouve" : "ABSENT (carre gris)",
            imageDir().string()
        ).c_str());
        float maxW = win.width - 20.f;
        float w = m_files->getContentSize().width;
        m_files->setScale(w > 0.f ? std::min(0.6f, maxW / w) : 0.6f);
    }

    void EditLayer::registerWithTouchDispatcher() {
        // Targeted + swallow = les touches ne passent pas derriere
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -500, true);
    }

    bool EditLayer::ccTouchBegan(CCTouch* t, CCEvent*) {
        auto p = t->getLocation();
        m_dragging = m_node->containsWorldPoint(p);
        if (m_dragging) {
            m_grab = ccpSub(m_node->getPosition(), p);
        }
        // Toujours avaler pour bloquer le menu derriere
        return true;
    }

    void EditLayer::ccTouchMoved(CCTouch* t, CCEvent*) {
        if (!m_dragging) return;
        auto win = CCDirector::sharedDirector()->getWinSize();
        auto np = ccpAdd(t->getLocation(), m_grab);
        np.x = std::clamp(np.x, 0.f, win.width);
        np.y = std::clamp(np.y, 0.f, win.height);
        m_node->setPosition(np);
    }

    void EditLayer::finishDrag() {
        if (!m_dragging) return;
        m_dragging = false;
        auto win = CCDirector::sharedDirector()->getWinSize();
        auto p = m_node->getPosition();
        savePosition(p.x / win.width, p.y / win.height);
    }

    void EditLayer::ccTouchEnded(CCTouch*, CCEvent*) {
        this->finishDrag();
    }

    void EditLayer::ccTouchCancelled(CCTouch*, CCEvent*) {
        this->finishDrag();
    }

    void EditLayer::onAdjust(CCObject* sender) {
        auto cfg = loadConfig();
        switch (static_cast<CCNode*>(sender)->getTag()) {
            case 1: setScale(cfg.scale - 0.1f); break;
            case 2: setScale(cfg.scale + 0.1f); break;
            case 3: setOpacity(cfg.opacity - 0.1f); break;
            case 4: setOpacity(cfg.opacity + 0.1f); break;
            default: break;
        }
        m_node->applyConfig();
        this->refreshInfo();
    }

    void EditLayer::onPreview(CCObject*) {
        m_previewOn = !m_previewOn;
        m_node->setDown(m_previewOn);
    }

    void EditLayer::onHide(CCObject*) {
        setHidden(!loadConfig().hidden);
        this->refreshInfo();
    }

    void EditLayer::onReset(CCObject*) {
        resetAll();
        m_node->applyConfig();
        this->refreshInfo();
    }

    void EditLayer::onClose(CCObject*) {
        (void)Mod::get()->saveData();
        this->removeFromParent();
    }
}
