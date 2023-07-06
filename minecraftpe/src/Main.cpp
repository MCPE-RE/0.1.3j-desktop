#include <glad/gl.h>
#include <glad/egl.h>

#include <stdio.h>
#include "Gvs.h"
#include "../../0.1.3j-core/src/client/gui/Font.h"
#include "../../0.1.3j-core/src/client/gui/GuiComponent.h"
#include "../../0.1.3j-core/src/NinecraftApp.h"
#include "AppPlatform_linux.h"


int main() {
    if (!glfwInit()) {
        puts("Failed to initialize glfw");
        return -1;
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
#if defined(_WIN32) || defined(__APPLE__)
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
#else
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
#endif

    window = glfwCreateWindow(720, 480, "Minecraft: Pocket Edition", NULL, NULL);
    if (!window) {
        puts("Failed to create glfw context");
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        printf("Failed to initialize OpenGL context\n");
        return -1;
    }

    AppPlatform_linux platform;

    AppContext context = {NULL, NULL, NULL, &platform, false};

    NinecraftApp *app = new NinecraftApp();
    app->init(&context);

    app->setSize(720, 480);

    while (true) {
        app->update();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
