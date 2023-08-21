#include <glad/gl.h>
#include <glad/egl.h>

#include <stdio.h>
#include "Gvs.h"
#include "../../0.1.3j-core/src/client/gui/Font.h"
#include "../../0.1.3j-core/src/client/gui/GuiComponent.h"
#include "../../0.1.3j-core/src/NinecraftApp.h"
#include "../../0.1.3j-core/src/client/input/keyboard/Keyboard.h"
#include "../../0.1.3j-core/src/client/input/mouse/Mouse.h"
#include "../../0.1.3j-core/src/client/input/Multitouch.h"
#include "AppPlatform_linux.h"

NinecraftApp *app;

int32_t getKey(int key) {
    switch (key) {
    case GLFW_KEY_ENTER:
    case GLFW_KEY_KP_ENTER:
        return app->options.keyMenuOk.keyCode;
    case GLFW_KEY_UP:
        return app->options.keyMenuPrevious.keyCode;
    case GLFW_KEY_DOWN:
        return app->options.keyMenuNext.keyCode;
    case GLFW_KEY_BACKSPACE:
        return app->options.keyMenuCancel.keyCode;
    case GLFW_KEY_T:
        return app->options.keyChat.keyCode;
    case GLFW_KEY_E:
        return app->options.keyInventory.keyCode;
    case GLFW_KEY_Q:
        return app->options.keyDrop.keyCode;
    case GLFW_KEY_W:
        return app->options.keyForward.keyCode;
    case GLFW_KEY_A:
        return app->options.keyLeft.keyCode;
    case GLFW_KEY_S:
        return app->options.keyBack.keyCode;
    case GLFW_KEY_D:
        return app->options.keyRight.keyCode;
    case GLFW_KEY_SPACE:
        return app->options.keyJump.keyCode;
    case GLFW_KEY_LEFT_SHIFT:
    case GLFW_KEY_RIGHT_SHIFT:
        return app->options.keySneak.keyCode;
    case GLFW_KEY_F:
        return app->options.keyFog.keyCode;
    case GLFW_KEY_C:
        return app->options.keyUse.keyCode;
    case GLFW_KEY_X:
        return app->options.keyDestory.keyCode;
    }
    return -1;
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    int32_t mcKey = getKey(key);
    if (mcKey != -1) {
        if (action == GLFW_PRESS) {
            Keyboard::feed(mcKey, 1);
        } else if (action == GLFW_RELEASE) {
            Keyboard::feed(mcKey, 0);
        }
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        if (action == GLFW_PRESS) {
            Mouse::feed(1, 1, (uint16_t)x, (uint16_t)y);
            Multitouch::feed(1, 1, (uint16_t)x, (uint16_t)y, 0);
        } else if (action == GLFW_RELEASE) {
            Mouse::feed(1, 0, (uint16_t)x, (uint16_t)y);
            Multitouch::feed(1, 0, (uint16_t)x, (uint16_t)y, 0);
        }
    }
}

void mousePositionCallback(GLFWwindow* window, double xpos, double ypos) {
    Mouse::feed(0, 0, (uint16_t)xpos, (uint16_t)ypos);
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

    audioEngine.init();
    audioEngine.addPath("./sfx/");

    AppPlatform_linux platform;

    AppContext context = {NULL, NULL, NULL, &platform, false};

    app = new NinecraftApp();
    app->init(&context);

    app->setSize(720, 480);

    glfwSetKeyCallback(window, keyboardCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, mousePositionCallback);

    while (!glfwWindowShouldClose(window)) {
        audioEngine.tick();
        app->update();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    audioEngine.deinit();
}
