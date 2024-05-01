#pragma once

#include <SFML/Graphics.hpp>

namespace Util {
    inline float getScale(int referenceWidth = 1920) {
        return sf::VideoMode::getDesktopMode().width / static_cast<float>(referenceWidth);
    }
}
