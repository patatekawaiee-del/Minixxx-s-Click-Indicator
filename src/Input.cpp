#include "Input.hpp"
#include "IndicatorNode.hpp"
#include <algorithm>
#include <cstdint>

namespace ci {
    static uint32_t s_held = 0;

    void onButton(bool down, int button, bool isPlayer1) {
        int idx = (isPlayer1 ? 0 : 4) + std::clamp(button, 0, 3);
        uint32_t bit = 1u << idx;
        if (down) s_held |= bit;
        else s_held &= ~bit;
        IndicatorNode::notifyHeld(s_held != 0);
    }

    void resetHeld() {
        s_held = 0;
        IndicatorNode::notifyHeld(false);
    }
}
