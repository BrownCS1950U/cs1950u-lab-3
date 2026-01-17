# Collision Debugger Lab

## Overview

This lab is designed to help debug your collision method implementations before integrating into your engine. You will implement collision detection for primitive shapes and the GJK/EPA algorithm for any convex shapes.

### Building
1. Ensure you have your Dependencies folder set up in the parent directory. See setup instructions.
2. Build the project using your preferred IDE or build system.
3. Run the executable.

### Controls

**Camera:**
- Left Mouse Drag: Rotate camera around origin
- Scroll: Zoom in/out

**Object Selection:**
- Click white sphere indicators to select objects
- Click empty space to deselect
- TAB: Cycle to next object
- `[` / `]`: Previous/Next object
- `1-9`: Select object by number

**Object Manipulation (ImGuizmo):**
- Select an object to show manipulation gizmo
- Drag arrows to translate

**Debug Visualization:**
- Orange arrows show MTV (Minimum Translation Vector) when objects collide
- Yellow sphere = selected object
- White spheres = unselected objects
- Grid shows XZ plane with colored axes (Red=X, Green=Y, Blue=Z)

---

## Part 1: Basic Shape Collision Detection

### Objective
Implement collision detection methods for simple shapes: Sphere, AABB, and Cylinder. Each method should return whether a collision occurred and the Minimum Translation Vector (MTV) if applicable.

### Files to Modify
- `src/Collider.cpp`  
Feel free to add more classes / helper functions as needed.

## Part 2: GJK/EPA Algorithm

### Objective
Implement the Gilbert-Johnson-Keerthi (GJK) algorithm for collision detection and the Expanding Polytope Algorithm (EPA) for calculating MTV.
Make sure to fill in the support functions for different convex shapes, and feel free to add more (e.g cones, capsules) if desired.

### Files to Modify
- `src/GJK.h`
- `src/GJK.cpp`  
Feel free to add more classes / helper functions as needed.

*Use the "GJK Collision mode" checkbox in the UI to override basic shape collision detection with GJK/EPA.*

### GJK/EPA Tutorials
- [GJK](https://winter.dev/articles/gjk-algorithm)
- [EPA](https://winter.dev/articles/epa-algorithm)

### Notes on using in your Game Engine
Once you have a working GJK/EPA implementation, it is important to use it only on convex shapes. For concave shapes, decompose them into convex parts or use a bounding volume to approximate the collider.  
You can also use GJK/EPA for collisions with triangle meshes by treating each triangle as a convex shape, but this can be computationally expensive. Spatial organization data structures (Platformer 2) will drastically optimize these collision checks.

---

## Submission Guidelines

Show us your implementation in class or at hours. Once your code works, integrate the collision functions into your engine.

---

## Credits

This collision debugger was built using:
- **GLFW** - Window and input handling
- **GLEW** - OpenGL extension loading
- **GLM** - Mathematics library
- **ImGui** - UI framework
- **ImGuizmo** - 3D manipulation gizmos
- **Assimp** - Model loading

---





<!-- Created by Marcus Winter -->