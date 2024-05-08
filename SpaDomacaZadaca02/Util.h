#pragma once

#include <SFML/Graphics.hpp>

namespace Util {
	inline float getScale(int referenceWidth = 1920) {
		return sf::VideoMode::getDesktopMode().width / static_cast<float>(referenceWidth);
	}

	inline float map(float value, float min, float max, float returnMin, float returnMax) {
		return (value - min) * (returnMax - returnMin) / (max - min) + returnMin;
	}

	inline sf::Color HSBtoRGB(float hue, float saturation, float brightness) {
		float r, g, b;

		int i = floor(hue * 6);
		float f = hue * 6 - i;
		float p = brightness * (1 - saturation);
		float q = brightness * (1 - f * saturation);
		float t = brightness * (1 - (1 - f) * saturation);

		switch (i % 6) {
		case 0: r = brightness; g = t, b = p; break;
		case 1: r = q; g = brightness; b = p; break;
		case 2: r = p; g = brightness; b = t; break;
		case 3: r = p; g = q; b = brightness; break;
		case 4: r = t; g = p; b = brightness; break;
		case 5: r = brightness; g = p; b = q; break;
		default: r = brightness; g = t, b = p; break;
		}

		return sf::Color(r * 255, g * 255, b * 255);
	}
}
