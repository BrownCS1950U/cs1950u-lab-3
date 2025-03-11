# Collision Debugger

This program provides a simple scene to test out your collision algorithm and visualize the MTVs returned by your implementation.

## Usage

To compile, create a directory called `build` and create Makefile using the cmake build system

    mkdir build && cd build
    cmake ..
    make

To run, make sure you are inside `build` directory, and then

    ../bin/viewer

## Todos

Go to `src/collision/collision.cpp`, and finish your collision algorithm there. Implement `updateCylinderCylinderCollisio()` and `updateEllipsoidTriangleCollision()` if you decide to do ellipsoid-triangle, or `updateGJKCollision()` for GJK-EPA. Collider definitions are available in `src/collision/collider.h`.

The scene contains a slope, 2 cylinders, and 1 ellipsoid. The program will show the MTVs for the objects once the array is filled in with data. So please make sure you update `m_mtvs` with corresponding collider object ID:

    m_mtvs[collider.id] = mtv;

You can move objects around with arrow keys and use the left panel to switch to another object to control if necessary.

To get boilerplate code and complex data structures out of the way and let you focus on algorithm implementation, ECS is not adopted in this project. You can directly access and modify colliders properties. For example:

    float radius = m_cylinder.radius;
