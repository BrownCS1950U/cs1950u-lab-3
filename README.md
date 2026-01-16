# Collision Detection Lab - 3D Game Engines

## Overview

This lab is designed to help debug your collision method implementations. You will implement collision detection for primitive shapes and the GJK/EPA algorithm for convex shapes.

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

## Lab Structure

This lab is divided into two parts:


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


### GJK/EPA Tutorials
- [GJK](https://winter.dev/articles/gjk-algorithm)
- [EPA](https://winter.dev/articles/epa-algorithm)

---

## Submission Guidelines

## Help

- GJK: https://winter.dev/articles/gjk-algorithm
- EPA: 

---

## Credits

This collision debugger was built using:
- **GLFW** - Window and input handling
- **GLEW** - OpenGL extension loading
- **GLM** - Mathematics library
- **ImGui** - UI framework
- **ImGuizmo** - 3D manipulation gizmos
- **Assimp** - Model loading (optional for future extensions)

---

## License

This lab is provided for educational purposes. Students may use and modify the code for learning. Redistribution or commercial use is prohibited without permission.

---
