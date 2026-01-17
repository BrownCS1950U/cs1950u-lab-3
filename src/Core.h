#pragma once
#include <string>

#include "Collider.h"
#include "render/Graphics.h"

class Collider;

namespace gl {
    class Camera;
    struct Light;
    struct DrawShape;
}

struct CollisionObject {
    explicit CollisionObject(const std::string& name);
    explicit CollisionObject(gl::DrawShape* shape);
    void setColor(const glm::vec3 color);
    bool isColliding();
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
    void collisionGui();

    // GUI popup to add new object
    void addObjectGui();


private:
    void controller();

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

};
