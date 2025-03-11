#include "window.h"

int main(int argc, char *argv[])
{
    gl::Window::initialize();

    while (gl::Window::isActive())
    {
        gl::Window::update();
    }

    glfwTerminate();
    return 0;
}