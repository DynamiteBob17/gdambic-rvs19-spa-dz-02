#pragma once

#include <SFML/Graphics.hpp>

namespace Util {
    inline float getScale(int referenceWidth = 1920) {
        return sf::VideoMode::getDesktopMode().width / static_cast<float>(referenceWidth);
    }

    inline float map(float value, float min, float max, float returnMin, float returnMax) {
        return (value - min) * (returnMax - returnMin) / (max - min) + returnMin;
    }
}
