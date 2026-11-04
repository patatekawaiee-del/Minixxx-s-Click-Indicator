#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "../EditLayer.hpp"

using namespace geode::prelude;

class $modify(CIMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto win = CCDirector::sharedDirector()->getWinSize();
        auto menu = CCMenu::create();
        menu->setPosition(ccp(0.f, 0.f));

        auto spr = ButtonSprite::create("Indicator");
        spr->setScale(0.6f);
        auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(CIMenuLayer::onOpenIndicator));
        btn->setPosition(ccp(52.f, win.height - 22.f));
        menu->addChild(btn);

        this->addChild(menu, 100);
        return true;
    }

    void onOpenIndicator(CCObject*) {
        ci::EditLayer::open();
    }
};
