#pragma once
#include <Geode/Geode.hpp>
#include <filesystem>
#include <optional>

namespace ci {
    struct Config {
        bool enabled = true;
        bool hidden = false;
        float scale = 1.f;
        float opacity = 1.f;
        float x = 0.88f; // fraction de la largeur de l'ecran
        float y = 0.20f; // fraction de la hauteur de l'ecran
    };

    Config loadConfig();
    void savePosition(float x, float y);
    void setScale(float v);
    void setOpacity(float v);
    void setHidden(bool v);
    void resetAll();

    std::filesystem::path imageDir();
    std::optional<std::filesystem::path> findImage(bool on);
}
