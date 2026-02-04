# ImGuizmo Integration

ImGuizmo has been successfully integrated into your CMake project!

## What Was Set Up

1. **CMakeLists.txt for ImGuizmo** (`Extra/ImGuizmo/CMakeLists.txt`)
   - Creates a static library target called `ImGuizmo`
   - Links against `imgui` (required dependency)
   - Includes all core ImGuizmo features:
     - ImGuizmo (3D transformation gizmos)
     - ImCurveEdit (curve editor)
     - ImGradient (gradient editor)
     - ImSequencer (timeline/sequencer)
     - GraphEditor (node graph editor)

2. **External Dependencies Configuration** (`External/CMakeLists.txt`)
   - Added `DEP_IMGUIZMO` option (enabled by default when `ENABLE_ALL_DEPENDENCIES` is ON)
   - Configured to build ImGuizmo from `Extra/ImGuizmo/`
   - Automatically validates that ImGui is available

3. **Main Project Configuration** (`CMakeLists.txt`)
   - Added `ImGuizmo` to the link libraries
   - Your executable now has access to all ImGuizmo features

## Usage in Your Code

To use ImGuizmo in your project, simply include the header:

```cpp
#include <ImGuizmo.h>
```

### Basic Example

```cpp
// In your render loop, after ImGui::NewFrame() and before ImGui::Render()
ImGuizmo::BeginFrame();

// Set the drawing area (usually your viewport)
ImGuizmo::SetRect(0, 0, windowWidth, windowHeight);

// Manipulate a transformation matrix
ImGuizmo::Manipulate(
    &cameraView[0][0],              // Camera view matrix (float[16])
    &cameraProjection[0][0],        // Camera projection matrix (float[16])
    ImGuizmo::OPERATION::TRANSLATE, // Operation: TRANSLATE, ROTATE, SCALE
    ImGuizmo::MODE::WORLD,          // Mode: WORLD or LOCAL
    &objectMatrix[0][0]             // Object transform matrix (modified)
);
```

### Available Operations

- `ImGuizmo::TRANSLATE` - Move objects
- `ImGuizmo::ROTATE` - Rotate objects
- `ImGuizmo::SCALE` - Scale objects
- `ImGuizmo::BOUNDS` - Bounding box manipulation

### Available Modes

- `ImGuizmo::WORLD` - Transform in world space
- `ImGuizmo::LOCAL` - Transform in local/object space

### Additional Features

See `Extra/ImGuizmo/USAGE_EXAMPLE.cpp` for more detailed examples including:
- ViewManipulate (camera orbit controls)
- Grid drawing
- Cube drawing for debugging
- Keyboard shortcuts for switching operations

## Other ImGuizmo Components

If you want to use the additional components:

```cpp
#include <ImCurveEdit.h>    // Curve editor
#include <ImGradient.h>     // Gradient editor
#include <ImSequencer.h>    // Timeline/sequencer
#include <GraphEditor.h>    // Node graph editor
```

## CMake Options

You can disable optional components in `Extra/ImGuizmo/CMakeLists.txt`:

```cmake
option(IMGUIZMO_INCLUDE_CURVE_EDIT "Include ImCurveEdit" ON)
option(IMGUIZMO_INCLUDE_GRADIENT "Include ImGradient" ON)
option(IMGUIZMO_INCLUDE_SEQUENCER "Include ImSequencer" ON)
option(IMGUIZMO_INCLUDE_GRAPH_EDITOR "Include GraphEditor" ON)
```

## Rebuilding

If you make changes to the ImGuizmo source or CMake configuration:

```bash
cmake --build cmake-build-debug --target ImGuizmo
```

Or rebuild the entire project:

```bash
cmake -B cmake-build-debug
cmake --build cmake-build-debug
```

## Resources

- [ImGuizmo GitHub Repository](https://github.com/CedricGuillemet/ImGuizmo)
- [ImGuizmo Demo/Examples](https://github.com/CedricGuillemet/ImGuizmo/tree/master/example)

## Notes

- ImGuizmo requires ImGui to be initialized and running
- Call `ImGuizmo::BeginFrame()` once per frame before using any ImGuizmo functions
- Matrices are expected in column-major format (OpenGL style)
- The library is compiled with C++20 standard (matching your project)

