#pragma once
#include <Geode/Geode.hpp>

namespace ci {
    class IndicatorNode : public cocos2d::CCNode {
    public:
        static IndicatorNode* create(bool editing);
        ~IndicatorNode() override;

        void setDown(bool down);
        void applyConfig();        // taille, opacite, position, visibilite
        void reloadImages();       // relit on.png / off.png
        bool containsWorldPoint(cocos2d::CCPoint const& p);

        static void setActive(IndicatorNode* node);
        static void notifyHeld(bool anyHeld);

    private:
        bool setup(bool editing);
        void tick(float dt);
        cocos2d::CCNode* buildState(bool on);

        cocos2d::CCNode* m_on = nullptr;
        cocos2d::CCNode* m_off = nullptr;
        cocos2d::CCSize m_half = { 24.f, 24.f };
        bool m_editing = false;
    };
}
