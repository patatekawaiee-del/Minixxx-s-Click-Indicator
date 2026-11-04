#include "Config.hpp"
#include <algorithm>

using namespace geode::prelude;

namespace ci {
    Config loadConfig() {
        auto m = Mod::get();
        Config c;
        c.enabled = m->getSettingValue<bool>("enabled");
        c.hidden = m->getSettingValue<bool>("hidden");
        c.scale = static_cast<float>(m->getSettingValue<double>("scale"));
        c.opacity = static_cast<float>(m->getSettingValue<double>("opacity"));
        c.x = m->getSavedValue<float>("pos-x", 0.88f);
        c.y = m->getSavedValue<float>("pos-y", 0.20f);
        return c;
    }

    void savePosition(float x, float y) {
        auto m = Mod::get();
        m->setSavedValue<float>("pos-x", x);
        m->setSavedValue<float>("pos-y", y);
        // Ecriture immediate : iOS peut suspendre / tuer l'app sans preavis
        (void)m->saveData();
    }

    void setScale(float v) {
        Mod::get()->setSettingValue<double>("scale", std::clamp<double>(v, 0.1, 4.0));
    }

    void setOpacity(float v) {
        Mod::get()->setSettingValue<double>("opacity", std::clamp<double>(v, 0.1, 1.0));
    }

    void setHidden(bool v) {
        Mod::get()->setSettingValue<bool>("hidden", v);
    }

    void resetAll() {
        setScale(1.f);
        setOpacity(1.f);
        setHidden(false);
        savePosition(0.88f, 0.20f);
    }

    std::filesystem::path imageDir() {
        return Mod::get()->getSaveDir();
    }

    std::optional<std::filesystem::path> findImage(bool on) {
        auto const name = on ? "on.png" : "off.png";
        std::error_code ec;
        // Priorite : dossier de sauvegarde du mod (modifiable a l'execution)
        // puis resources embarquees dans le .geode
        for (auto const& dir : { Mod::get()->getSaveDir(), Mod::get()->getResourcesDir() }) {
            auto p = dir / name;
            if (std::filesystem::exists(p, ec) && !ec) {
                return p;
            }
        }
        return std::nullopt;
    }
}
