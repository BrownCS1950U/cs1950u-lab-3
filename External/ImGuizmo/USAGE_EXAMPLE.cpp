// Example usage of ImGuizmo in your project
// Include this in your source files where you want to use ImGuizmo

#include <ImGuizmo.h>

// Example: Using ImGuizmo to manipulate a transformation matrix
void ExampleImGuizmoUsage()
{
    // Your camera matrices
    float cameraView[16];     // Your camera view matrix
    float cameraProjection[16]; // Your camera projection matrix

    // The object's transformation matrix you want to manipulate
    float objectMatrix[16];

    // Setup ImGuizmo for the current frame
    ImGuizmo::BeginFrame();

    // Set the drawing area (usually your viewport)
    ImGuizmo::SetRect(0, 0, 800, 600); // Example: full window

    // Draw and manipulate the gizmo
    ImGuizmo::Manipulate(
        cameraView,              // Camera view matrix
        cameraProjection,        // Camera projection matrix
        ImGuizmo::OPERATION::TRANSLATE, // Operation: TRANSLATE, ROTATE, SCALE, or BOUNDS
        ImGuizmo::MODE::WORLD,   // Mode: WORLD or LOCAL
        objectMatrix             // The matrix to manipulate (will be modified)
    );

    // Optional: Check if the gizmo is being used
    if (ImGuizmo::IsUsing())
    {
        // The user is currently manipulating the object
    }

    // Optional: Check if the gizmo is hovered
    if (ImGuizmo::IsOver())
    {
        // The mouse is over the gizmo
    }
}

// Example: Using ImGuizmo with keyboard shortcuts to switch operations
void ExampleWithKeyboardShortcuts()
{
    static ImGuizmo::OPERATION currentOperation = ImGuizmo::TRANSLATE;
    static ImGuizmo::MODE currentMode = ImGuizmo::WORLD;

    // Keyboard shortcuts (assuming you have input handling)
    // if (IsKeyPressed('W')) currentOperation = ImGuizmo::TRANSLATE;
    // if (IsKeyPressed('E')) currentOperation = ImGuizmo::ROTATE;
    // if (IsKeyPressed('R')) currentOperation = ImGuizmo::SCALE;
    // if (IsKeyPressed('T')) currentMode = (currentMode == ImGuizmo::WORLD) ? ImGuizmo::LOCAL : ImGuizmo::WORLD;

    // Then use currentOperation and currentMode in your Manipulate call
}

// Example: Using ImGuizmo ViewManipulate for camera control
void ExampleViewManipulate()
{
    float cameraView[16];
    float cameraDistance = 10.0f; // Distance from camera to target

    // Draw a view manipulation widget in the corner of your viewport
    ImGuizmo::ViewManipulate(
        cameraView,              // Camera view matrix (will be modified)
        cameraDistance,          // Distance to target
        ImVec2(700, 0),         // Position in screen space
        ImVec2(100, 100),       // Size of the widget
        0x10101010              // Background color (optional)
    );
}

// Example: Using additional ImGuizmo features
void ExampleAdvancedFeatures()
{
    // Enable/disable drawing of the gizmo
    ImGuizmo::Enable(true);

    // Set the orthographic view (for 2D or orthographic projection)
    ImGuizmo::SetOrthographic(false);

    // Draw a grid
    float gridMatrix[16]; // Identity matrix for the grid
    ImGuizmo::DrawGrid(
        gridMatrix,             // Camera view matrix
        gridMatrix,             // Camera projection matrix
        gridMatrix,             // Grid transform matrix
        10.0f                   // Grid size
    );

    // Draw a cube (useful for debugging)
    float cubeMatrix[16]; // The cube's transform
    ImGuizmo::DrawCubes(
        gridMatrix,             // Camera view matrix
        gridMatrix,             // Camera projection matrix
        &cubeMatrix,            // Array of cube matrices
        1                       // Number of cubes
    );
}

