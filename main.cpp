#include <iostream>
#include <GLFW/glfw3.h>

#include "src/core/Window.h"


int main() {
    Window::initialize(1280, 720, "Collision Debugger");
    while (Window::isActive()) {
        Window::update();
    }
    Window::shutDown();

    return 0;
}
