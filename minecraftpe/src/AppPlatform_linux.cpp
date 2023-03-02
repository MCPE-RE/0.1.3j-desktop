#include "AppPlatform_linux.h"
#include <iostream>
#include "Gvs.h"
#include <SOIL2.h>

TextureData AppPlatform_linux::loadTexture(const std::string &name, bool alpha) {
    std::string path = "assets/" + name;
    TextureData tex;
    tex.pixels = SOIL_load_image((char *)path.c_str(), &tex.width, &tex.height, 0, alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
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