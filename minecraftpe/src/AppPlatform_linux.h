#ifndef APPPLATFORMLINUX_H_
#define APPPLATFORMLINUX_H_

#include "../../0.1.3j-core/src/AppPlatform.h"

class AppPlatform_linux : public AppPlatform {
public:
	int getScreenHeight();

	int getScreenWidth();

	TextureData loadTexture(const std::string &name, bool alpha);
};

#endif