#include "AppPlatform_linux.h"
#include <iostream>
#include "Gvs.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

TextureData AppPlatform_linux::loadTexture(const std::string &name, bool alpha) {
    std::string path = "assets/" + name;
    TextureData tex;
    tex.pixels = stbi_load((char *)path.c_str(), &tex.width, &tex.height, NULL, STBI_rgb_alpha);
    return tex;
}

int AppPlatform_linux::getScreenHeight() {
    int height;
    glfwGetWindowSize(window, NULL, &height);
    return height;
}

int AppPlatform_linux::getScreenWidth() {
    int width;
    glfwGetWindowSize(window, &width, NULL);
    return width;
}

void AppPlatform_linux::playSound(const std::string &name, float volume, float pitch) {
    audioEngine.play(name, volume, pitch);
}

bool AppPlatform_linux::isTouchscreen() {
	return true;
}

int32_t AppPlatform_linux::checkLicense() {
    return 1;
}

bool AppPlatform_linux::hasBuyButtonWhenInvalidLicense() {
    return false;
}

void AppPlatform_linux::buyGame() {}