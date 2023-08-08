#include <glad/gl.h>
#include <glad/egl.h>

#include <stdio.h>
#include "Gvs.h"
#include "../../0.1.3j-core/src/client/gui/Font.h"
#include "../../0.1.3j-core/src/client/gui/GuiComponent.h"
#include "../../0.1.3j-core/src/NinecraftApp.h"
#include "../../0.1.3j-core/src/client/input/keyboard/Keyboard.h"
#include "AppPlatform_linux.h"

NinecraftApp *app;

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ENTER || key == GLFW_KEY_KP_ENTER) {
        if (action == GLFW_PRESS) {
            Keyboard::feed(app->options.keyMenuOk.keyCode, 1);
        } else if (action == GLFW_RELEASE) {
            Keyboard::feed(app->options.keyMenuOk.keyCode, 0);
        }
    }
    if (key == GLFW_KEY_UP) {
        if (action == GLFW_PRESS) {
            Keyboard::feed(app->options.keyMenuPrevious.keyCode, 1);
        } else if (action == GLFW_RELEASE) {
            Keyboard::feed(app->options.keyMenuPrevious.keyCode, 0);
        }
    }
    if (key == GLFW_KEY_DOWN) {
        if (action == GLFW_PRESS) {
            Keyboard::feed(app->options.keyMenuNext.keyCode, 1);
        } else if (action == GLFW_RELEASE) {
            Keyboard::feed(app->options.keyMenuNext.keyCode, 0);
        }
    }
}

int main() {
    if (!glfwInit()) {
        puts("Failed to initialize glfw");
        return -1;
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);

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

    app = new NinecraftApp();
    app->init(&context);

    app->setSize(720, 480);

    glfwSetKeyCallback(window, keyboard_callback);

    while (!glfwWindowShouldClose(window)) {
        app->update();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
