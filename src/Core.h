#pragma once
#include <string>

#include "Collider.h"
#include "render/Graphics.h"
#include "render/SkeletalMesh.h"

class Collider;

namespace gl {
    class Camera;
    struct Light;
    struct DrawShape;
}

struct CollisionObject {
    explicit CollisionObject(const std::string& name) {
        shape = gl::Graphics::getShape(name);
        transform = std::make_shared<Transform>();
        material = gl::defaultMaterial;
    }

    explicit CollisionObject(gl::DrawShape* shape) : shape(shape) {
        transform = std::make_shared<Transform>();
        material = gl::defaultMaterial;
    }

    void setColor(const glm::vec3 color) {
        material.diffuse = color;
        material.ambient = color;
        material.specular = color;
    }

    bool isColliding() {
        for (const auto& [i, mtv] : mtv_map) {
            if (mtv.collision) {
                return true;
            }
        }
        return false;
    }

    std::unordered_map<size_t, MTV> mtv_map;
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
    void mouseButton(int button, int action);
    void onScroll(double xoffset, double yoffset);

    std::shared_ptr<gl::Camera> getCamera() const { return m_camera; }
    std::vector<CollisionObject>& getShapes() { return m_shapes; }
    int getSelectedObjectIndex() const { return m_selected_object_index; }
    void setSelectedObjectIndex(int index) { m_selected_object_index = index; }
    void selectedObjectGui(int index);
    bool& getShowGrid() { return m_show_grid; }
    void collisionGui();

    // GUI popup to add new object
    void addObjectGui();


private:
    void controller();
    void drawDebugGrid();
    void drawSelectionIndicators();
    void removeObject(int index);
    void clearObjects();
    void updateCollisions();
    void resolveCollision(int index);

    std::shared_ptr<gl::Camera> m_camera;
    std::shared_ptr<gl::Light> m_light;
    std::vector<CollisionObject> m_shapes;
    int m_selected_object_index = -1;
    OrbitCameraControls orbit;

    std::vector<glm::vec3> convex_vertices;
    std::array<glm::vec3, 3> triangle_vertices;
    // Grid settings
    bool m_show_grid = true;
    const float m_grid_size = 10.0f;  // Grid extends from -10 to +10
    const float m_cell_size = 1.0f;   // 1 unit per cell
};
