#pragma once

#include "../../0.1.3j-core/src/AppPlatform.h"

class AppPlatform_linux : public AppPlatform {
public:
	int getScreenHeight();

	int getScreenWidth();

	TextureData loadTexture(const std::string &name, bool alpha);

    void playSound(const std::string &name, float volume, float pitch);

    bool isTouchscreen();

    int32_t checkLicense();

    bool hasBuyButtonWhenInvalidLicense();

    void buyGame();
};
