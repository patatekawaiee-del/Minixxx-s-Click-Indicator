#pragma once
#include <Geode/Geode.hpp>
#include "IndicatorNode.hpp"

namespace ci {
    // Mode EDIT : s'ouvre depuis le menu principal, sans lancer de partie.
    class EditLayer : public cocos2d::CCLayer {
    public:
        static void open();

    private:
        bool setup();
        void registerWithTouchDispatcher() override;
        bool ccTouchBegan(cocos2d::CCTouch* t, cocos2d::CCEvent* e) override;
        void ccTouchMoved(cocos2d::CCTouch* t, cocos2d::CCEvent* e) override;
        void ccTouchEnded(cocos2d::CCTouch* t, cocos2d::CCEvent* e) override;
        void ccTouchCancelled(cocos2d::CCTouch* t, cocos2d::CCEvent* e) override;

        void addButton(cocos2d::CCMenu* menu, char const* text, cocos2d::SEL_MenuHandler cb,
                       int tag, cocos2d::CCPoint pos);
        void refreshInfo();
        void finishDrag();

        void onAdjust(cocos2d::CCObject* sender);
        void onPreview(cocos2d::CCObject*);
        void onHide(cocos2d::CCObject*);
        void onReset(cocos2d::CCObject*);
        void onClose(cocos2d::CCObject*);

        IndicatorNode* m_node = nullptr;
        cocos2d::CCLabelBMFont* m_info = nullptr;
        cocos2d::CCLabelBMFont* m_files = nullptr;
        cocos2d::CCPoint m_grab = { 0.f, 0.f };
        bool m_dragging = false;
        bool m_previewOn = false;
    };
}
