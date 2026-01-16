#pragma once
#include "GLFW/glfw3.h"

class Core;

class UI {

public:
    UI(GLFWwindow* window, Core* core);

    void initialize(GLFWwindow* window);
    void update();
    static bool isUsingGizmo();
    static glm::vec2 getWindowSize();
private:


    static glm::vec2 window_size;
    Core* m_core;
};
