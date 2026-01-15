#pragma once
#include "GLFW/glfw3.h"

class Core;

class UI {

public:
    UI(GLFWwindow* window, Core* core);

    void initialize(GLFWwindow* window);
    void update();
    static bool isUsingGizmo() { return using_gizmo; }

private:
    Core* m_core;
    static bool using_gizmo;
};
