#pragma once
#include <string>

#include "collision/Collider.h"
#include "render/Graphics.h"
#include "render/SkeletalMesh.h"

class Collider;

namespace gl {
    class Camera;
    struct Light;
    struct DrawShape;
}

struct Object {
    explicit Object(const std::string& name) {
        shape = gl::Graphics::getShape(name);
        transform = std::make_shared<Transform>();
        material = gl::defaultMaterial;
    }

    explicit Object(gl::DrawShape* shape) : shape(shape) {
        transform = std::make_shared<Transform>();
        material = gl::defaultMaterial;
    }

    MTV mtv;
    Collider* collider = nullptr;
    const gl::DrawShape* shape;
    std::shared_ptr<Transform> transform;
    gl::DrawMaterial material;
};

struct OrbitCameraControls {
    // Orbit camera controls (spherical coordinates)
    float azimuth = 0.0f; // Angle around Y axis (latitude, in degrees)
    float elevation = 45.0f; // Angle from XZ plane (longitude, in degrees)
    float distance = 5.0f; // Distance from origin
    const glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f); // Always look at origin
};

class Core {
public:
    Core();
    ~Core() = default;
    void draw();

    void update(double delta_time);
    void controller(double delta_time);

    void keyPressed(int key);
    void mouseButton(int button, int action);

    void onScroll(double xoffset, double yoffset);

    // Accessors for UI/ImGuizmo
    std::shared_ptr<gl::Camera> getCamera() const { return m_camera; }
    std::vector<Object>& getShapes() { return m_shapes; }
    int getSelectedObjectIndex() const { return m_selected_object_index; }
    void setSelectedObjectIndex(int index) { m_selected_object_index = index; }

    // Grid control accessors
    bool& getShowGrid() { return m_show_grid; }

private:
    void drawDebugGrid();

    std::shared_ptr<gl::Camera> m_camera;
    std::shared_ptr<gl::Light> m_light;
    std::vector<Object> m_shapes;
    int m_selected_object_index = 0;
    OrbitCameraControls orbit;

    // Grid settings
    bool m_show_grid = true;
    const float m_grid_size = 10.0f;  // Grid extends from -10 to +10
    const float m_cell_size = 1.0f;   // 1 unit per cell
};
