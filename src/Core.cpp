#include "Core.h"

#include <iostream>

#include "core/Debug.h"
#include "imgui.h"
#include "core/Window.h"
#include "Collider.h"
#include "GJK.h"
#include "collision/BoxCollider.h"
#include "collision/ConvexCollider.h"
#include "collision/CylinderCollider.h"
#include "collision/SphereCollider.h"
#include "collision/TriangleCollider.h"

#include "render/Camera.h"
#include "render/Mesh.h"
#include "render/SkeletalMesh.h"
#include "render/shapes/Cone.h"
#include "render/shapes/Cube.h"
#include "render/shapes/Cylinder.h"
#include "render/shapes/Quad.h"
#include "render/shapes/Sphere.h"


static const char* CONVEX_HULL_PATH = "Resources/Models/convex_hull.obj";
Core::Core() : m_camera(std::make_shared<gl::Camera>()), m_light(std::make_shared<gl::Light>()) {
    m_light->position = glm::vec3(0, 5, 0);

    // Initialize orbit camera position using spherical coordinates
    float azimuth_rad = glm::radians(orbit.azimuth);
    float elevation_rad = glm::radians(orbit.elevation);
    glm::vec3 pos = glm::vec3(
        orbit.distance * cos(elevation_rad) * sin(azimuth_rad),
        orbit.distance * sin(elevation_rad),
        orbit.distance * cos(elevation_rad) * cos(azimuth_rad)
    );
    m_camera->setPosition(pos);
    m_camera->setLook(glm::normalize(orbit.target - pos));

    auto cube = gl::Cube(3);
    auto cone = gl::Cone(8, 4);
    auto cylinder = gl::Cylinder(8, 1);
    auto sphere = gl::Sphere(8, 8);
    auto quad = gl::Quad(1);

    gl::Graphics::addShape("cube", cube.loadDrawShape());
    gl::Graphics::addShape("cone", cone.loadDrawShape());
    gl::Graphics::addShape("cylinder", cylinder.loadDrawShape());
    gl::Graphics::addShape("sphere", sphere.loadDrawShape());
    gl::Graphics::addShape("quad", quad.loadDrawShape());

    gl::Graphics::addShape("convex_hull", gl::Mesh::loadStaticMesh(CONVEX_HULL_PATH).objects[0].shape); // Placeholder
    convex_vertices = gl::Mesh::loadVertices(CONVEX_HULL_PATH);

    gl::Graphics::addShape("triangle", gl::Mesh::loadStaticMesh("Resources/Models/triangle.obj").objects[0].shape); // Placeholder
    auto verts = gl::Mesh::loadVertices("Resources/Models/triangle.obj");
    for (size_t i = 0; i < triangle_vertices.size(); i++) {
        triangle_vertices[i] = verts[i];
    }

    auto object = CollisionObject("sphere");
    object.transform->setPosition(glm::vec3(-1, 0, 0));
    object.collider = new SphereCollider(object.transform);


    auto object2 = CollisionObject("sphere");
    object2.transform->setPosition(glm::vec3(1, 0, 0));
    object2.collider = new SphereCollider(object2.transform);


    m_shapes.push_back(object);
    m_shapes.push_back(object2);
}

static bool animation_playing = true;

static const gl::DrawMaterial ARROW_MATERIAL = {
    .ambient = glm::vec3(1.0f, .5f, 0.0f),
    .diffuse = glm::vec3(1.0f, 0.5f, 0.0f),
    .specular = glm::vec3(0),
    .shininess = 1.f
};

static const gl::DrawMaterial CENTER_MATERIAL = {
    .ambient = glm::vec3(1.0f, .0f, 0.0f),
    .diffuse = glm::vec3(1.0f, 0.0f, 0.0f),
    .specular = glm::vec3(0),
    .shininess = 1.f
};

void draw3DArrow(glm::vec3 start, glm::vec3 end, float shaft_radius, float tip_radius) {
    auto tip = gl::Graphics::getShape("cone");
    auto shaft = gl::Graphics::getShape("cylinder");

    // Calculate arrow properties
    glm::vec3 direction = end - start;
    float total_length = glm::length(direction);

    // Avoid drawing if arrow is too small
    if (total_length < 0.001f) return;

    glm::vec3 normalized_dir = glm::normalize(direction);

    // Calculate shaft length (total minus tip)

    float tip_length = total_length / 4.f; // Tip is 25% of total length
    float shaft_length = total_length - tip_length;
    if (shaft_length < 0.001f) shaft_length = total_length * 0.7f; // Fallback: 70% shaft, 30% tip
    float actual_tip_length = total_length - shaft_length;

    // Draw Center (sphere)
    {
        Transform center_transform;
        center_transform.setPosition(start);
        center_transform.setScale(glm::vec3(shaft_radius * 2.0f)); // Diameter
        gl::Graphics::drawObject(gl::Graphics::getShape("sphere"), center_transform, CENTER_MATERIAL);
    }

    // Draw shaft (cylinder)
    {
        Transform shaft_transform;

        // Position at midpoint of shaft
        glm::vec3 shaft_midpoint = start + normalized_dir * (shaft_length * 0.5f);
        shaft_transform.setPosition(shaft_midpoint);

        // Scale: radius for XZ, length for Y (cylinder is 1 unit tall by default)
        shaft_transform.setScale(glm::vec3(shaft_radius, shaft_length, shaft_radius));

        // Rotate to align with direction
        // Default cylinder points up (0,1,0), we need to rotate to point along normalized_dir
        glm::vec3 up(0, 1, 0);
        glm::vec3 axis = glm::cross(up, normalized_dir);
        float axis_length = glm::length(axis);

        if (axis_length > 0.001f) {
            axis = glm::normalize(axis);
            float angle = acos(glm::clamp(glm::dot(up, normalized_dir), -1.0f, 1.0f));
            shaft_transform.rotate(angle, axis);
        }
        else if (glm::dot(up, normalized_dir) < 0) {
            // Pointing down, rotate 180 degrees
            shaft_transform.rotate(glm::pi<float>(), glm::vec3(1, 0, 0));
        }

        gl::Graphics::drawObject(shaft, shaft_transform, ARROW_MATERIAL);
    }

    // Draw tip (cone)
    {
        Transform tip_transform;

        // Position at base of cone (top of shaft)
        glm::vec3 tip_base = start + normalized_dir * shaft_length;
        // Cone's base is at y=0, tip at y=1, so offset by half height to position correctly
        glm::vec3 tip_center = tip_base + normalized_dir * (actual_tip_length * 0.5f);
        tip_transform.setPosition(tip_center);

        // Scale: radius for XZ, length for Y
        tip_transform.setScale(glm::vec3(tip_radius, actual_tip_length, tip_radius));

        // Rotate to align with direction (same as shaft)
        glm::vec3 up(0, 1, 0);
        glm::vec3 axis = glm::cross(up, normalized_dir);
        float axis_length = glm::length(axis);

        if (axis_length > 0.001f) {
            axis = glm::normalize(axis);
            float angle = acos(glm::clamp(glm::dot(up, normalized_dir), -1.0f, 1.0f));
            tip_transform.rotate(angle, axis);
        }
        else if (glm::dot(up, normalized_dir) < 0) {
            // Pointing down, rotate 180 degrees
            tip_transform.rotate(glm::pi<float>(), glm::vec3(1, 0, 0));
        }

        gl::Graphics::drawObject(tip, tip_transform, ARROW_MATERIAL);
    }
}

float getObjectScaleFactor(glm::vec3 size) {
    return 1.f / std::max({size.x, size.y, size.z});
}

void Core::selectedObjectGui(int index) {
    if (index < 0 || index >= m_shapes.size()) return;
    auto& obj = m_shapes[index];

    ImGui::SliderFloat3("Pos    ", glm::value_ptr(obj.transform->position_), -5.0f, 5.0f);
    ImGui::SameLine();
    if (ImGui::Button("Reset##Pos")) obj.transform->setPosition(glm::vec3(0.f));

    auto obj_scale = getObjectScaleFactor(obj.shape->max - obj.shape->min);
    float max_scale = 3.f * obj_scale;
    float min_scale = 0.5f * obj_scale;

    ImGui::SliderFloat3("Scale  ", glm::value_ptr(obj.transform->scale_), min_scale, max_scale);
    ImGui::SameLine();
    if (ImGui::Button("Reset##Scale")) obj.transform->setScale(glm::vec3(1.f*obj_scale));
    glm::vec3 color = obj.material.diffuse;
    if (ImGui::ColorEdit3("Color", glm::value_ptr(color))) {
        obj.setColor(color);
    }

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6, 0, 0, 1));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7, 0, 0, 1));
    if (ImGui::Button("Remove Object")) {
        removeObject(index);
    }
    ImGui::PopStyleColor(2);

    if (ImGui::Button("Resolve collision")) {
        resolveCollision(index);
    }
}

void Core::collisionGui() {


    ImGui::Checkbox("GJK Collision mode", &Collider::gjk_mode);
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Overrides the basic primitive collision functions with your GJK implementation.");
    }
    for (size_t i = 0; i < m_shapes.size(); i++) {
        auto& obj = m_shapes[i];
        if (obj.collider) {
            ImGui::Text("Object %zu Collisions:", i);
            if (obj.isColliding()) {
                for (const auto& [other_index, mtv] : obj.mtv_map) {
                    if (mtv.collision) {
                        // ImGui::BulletText()
                        auto vector = mtv.normal * mtv.depth;
                        ImGui::BulletText("Object %zu: MTV = (%.2f, %.2f, %.2f)",
                                          other_index,
                                          vector.x,
                                          vector.y,
                                          vector.z);
                    }
                }
            }
            else {
                ImGui::BulletText("No collisions");
            }
        }
        ImGui::Separator();
    }
}

void Core::addObjectGui() {
    ImGui::Text("Select Object Type:");
    ImGui::Separator();

    static int selectedObjectType = SPHERE;

    ImGui::RadioButton("Sphere", &selectedObjectType, SPHERE);
    ImGui::RadioButton("Cube", &selectedObjectType, BOX);
    ImGui::RadioButton("Cylinder", &selectedObjectType, CYLINDER);
    ImGui::RadioButton("Convex Hull", &selectedObjectType, CONVEXHULL);
    ImGui::RadioButton("Triangle", &selectedObjectType, TRIANGLE);
    ImGui::Separator();

    // Buttons at the bottom
    if (ImGui::Button("Confirm", ImVec2(120, 0))) {
        switch (selectedObjectType) {
        default: case SPHERE: {
            auto object = CollisionObject("sphere");
            object.collider = new SphereCollider(object.transform);
            m_shapes.push_back(object);
            break;
        }
        case CYLINDER: {
            auto object = CollisionObject("cylinder");
            object.collider = new CylinderCollider(object.transform);
            m_shapes.push_back(object);
            break;
        }
        case BOX: {
            auto object = CollisionObject("cube");
            object.collider = new BoxCollider(object.transform);
            m_shapes.push_back(object);
            break;
        }
        case TRIANGLE: {
            auto object = CollisionObject("triangle");
            object.collider = new TriangleCollider(object.transform, triangle_vertices);
            m_shapes.push_back(object);
            break;

        }
        case CONVEXHULL: {
            auto object = CollisionObject("convex_hull");
            object.transform->setScale(glm::vec3(getObjectScaleFactor(object.shape->max - object.shape->min)));
            object.collider = new ConvexCollider(object.transform, convex_vertices);
            m_shapes.push_back(object);
        }
        }
        ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel", ImVec2(120, 0))) {
        ImGui::CloseCurrentPopup();
    }
}

void Core::removeObject(int index) {
    if (index < 0 || index >= m_shapes.size()) return;

    m_shapes.erase(m_shapes.begin() + index);
    if (m_selected_object_index == index) {
        m_selected_object_index = -1; // Deselect if the selected object was removed
    }
    else if (m_selected_object_index > index) {
        m_selected_object_index--; // Adjust selected index if necessary
    }

    for (auto& obj : m_shapes) {
        obj.mtv_map.clear();
    }
}

void Core::clearObjects() {
    m_shapes.clear();
    m_selected_object_index = -1;
}

void Core::updateCollisions() {
    for (size_t i = 0; i < m_shapes.size(); i++) {
        auto& obj = m_shapes[i];
        for (size_t j = i; j < m_shapes.size(); j++) {
            if (i == j) continue;
            if (auto& other = m_shapes[j]; obj.collider && other.collider) {
                MTV mtv;
                if (Collider::gjk_mode) {
                    mtv = GJK::getMTV(obj.collider, other.collider);
                } else {
                    mtv = obj.collider->getMTV(other.collider);
                }
                obj.mtv_map[j] = mtv;
                other.mtv_map[i] = mtv.invert();
            }
        }
    }
}

void Core::resolveCollision(int index) {
    if (index < 0 || index >= m_shapes.size()) return;
    auto& obj = m_shapes[index];

    if (!obj.isColliding()) return;


    for (int i = 0; i < m_shapes.size(); i++) {
        if (i == index) continue;
        if (obj.mtv_map.find(i) != obj.mtv_map.end()) {
            auto mtv = obj.mtv_map[i];
            if (mtv.collision) {
                // Move object out of collision along MTV
                obj.transform->setPosition(obj.transform->getPosition() + mtv.normal * mtv.depth);
                updateCollisions();
            }
        }
    }
}

void Core::drawDebugGrid() {
    if (!m_show_grid) return;

    // Colors for axes (standard RGB convention)
    const glm::vec3 X_AXIS_POS_COLOR(1.0f, 0.0f, 0.0f); // Red
    const glm::vec3 X_AXIS_NEG_COLOR(0.5f, 0.0f, 0.0f); // Dark Red
    const glm::vec3 Y_AXIS_POS_COLOR(0.0f, 1.0f, 0.0f); // Green
    const glm::vec3 Y_AXIS_NEG_COLOR(0.0f, 0.5f, 0.0f); // Dark Green
    const glm::vec3 Z_AXIS_POS_COLOR(0.0f, 0.0f, 1.0f); // Blue
    const glm::vec3 Z_AXIS_NEG_COLOR(0.0f, 0.0f, 0.5f); // Dark Blue
    const glm::vec3 GRID_COLOR(0.7); // Gray

    const float AXIS_LINE_WIDTH = 3.0f;
    const float GRID_LINE_WIDTH = 1.0f;

    // Draw XZ plane grid
    for (float i = -m_grid_size; i <= m_grid_size; i += m_cell_size) {
        // Lines parallel to X axis
        if (i == 0.0f) continue; // Skip axis line, drawn later
        gl::Graphics::drawLine3D(
            glm::vec3(-m_grid_size, 0.0f, i),
            glm::vec3(m_grid_size, 0.0f, i),
            GRID_COLOR,
            GRID_LINE_WIDTH
        );

        // Lines parallel to Z axis
        gl::Graphics::drawLine3D(
            glm::vec3(i, 0.0f, -m_grid_size),
            glm::vec3(i, 0.0f, m_grid_size),
            GRID_COLOR,
            GRID_LINE_WIDTH
        );
    }

    // Draw thick axis lines on top
    // X-axis (Red)
    gl::Graphics::drawLine3D(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(m_grid_size, 0.0f, 0.0f),
        X_AXIS_POS_COLOR,
        AXIS_LINE_WIDTH
    );
    gl::Graphics::drawLine3D(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(-m_grid_size, 0.0f, 0.0f),
        X_AXIS_NEG_COLOR,
        AXIS_LINE_WIDTH
    );

    // Y-axis (Green)
    gl::Graphics::drawLine3D(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, m_grid_size, 0.0f),
        Y_AXIS_POS_COLOR,
        AXIS_LINE_WIDTH
    );
    gl::Graphics::drawLine3D(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, -m_grid_size, 0.0f),
        Y_AXIS_NEG_COLOR,
        AXIS_LINE_WIDTH
    );

    // Z-axis (Blue)
    gl::Graphics::drawLine3D(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, m_grid_size),
        Z_AXIS_POS_COLOR,
        AXIS_LINE_WIDTH
    );
    gl::Graphics::drawLine3D(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, -m_grid_size),
        Z_AXIS_NEG_COLOR,
        AXIS_LINE_WIDTH
    );
}

constexpr float GIZMO_INDICATOR_RADIUS = 0.05f; // 10% of default size
void Core::drawSelectionIndicators() {
    auto sphere = gl::Graphics::getShape("sphere");

    for (size_t i = 0; i < m_shapes.size(); i++) {
        const auto& obj = m_shapes[i];

        Transform indicatorTransform;
        indicatorTransform.setPosition(obj.transform->getPosition());

        // Small sphere - 10% of default size
        indicatorTransform.setScale(glm::vec3(2 * GIZMO_INDICATOR_RADIUS));

        // Color: white for unselected, yellow for selected
        gl::DrawMaterial indicatorMaterial;
        if (i == m_selected_object_index) {
            indicatorMaterial.ambient = glm::vec3(1.0f, 1.0f, 0.0f); // Yellow
            indicatorMaterial.diffuse = glm::vec3(1.0f, 1.0f, 0.0f);
        }
        else {
            indicatorMaterial.ambient = glm::vec3(1.0f, 1.0f, 1.0f); // White
            indicatorMaterial.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
        }
        indicatorMaterial.specular = glm::vec3(0.5f);
        indicatorMaterial.shininess = 32.0f;

        // Draw as solid sphere
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        gl::Graphics::drawObject(sphere, indicatorTransform, indicatorMaterial);
    }
}

void Core::draw() {
    gl::Graphics::usePhongShader();
    gl::Graphics::setCameraUniforms(m_camera.get());
    gl::Graphics::setLight(*m_light);

    // Draw debug grid and axes
    drawDebugGrid();
    updateCollisions();

    for (size_t i = 0; i < m_shapes.size(); i++) {
        auto& obj = m_shapes[i];
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        gl::Graphics::drawObject(obj.shape, *obj.transform, obj.material);


        // Draw mtv
        for (const auto& [i, mtv] : obj.mtv_map) {
            if (mtv.collision) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

                glm::vec3 center = obj.transform->getPosition();
                glm::vec3 mtv_vector = mtv.normal * mtv.depth;
                glm::vec3 arrow_end = center + mtv_vector;

                // Draw arrow with appropriate sizing
                float shaft_radius = 0.05f;
                float tip_radius = 0.1f;

                draw3DArrow(center, arrow_end, shaft_radius, tip_radius);
            }
        }
    }

    // Draw selection indicators (small spheres at object centers)
    drawSelectionIndicators();
}

static bool withinUIWindow(const glm::vec2& mouse_pos) {
    // Use ImGui's built-in functions to check if mouse is over any ImGui window
    // This includes popups, tooltips, color pickers, etc.
    return ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) ||
        ImGui::IsAnyItemActive() ||
        ImGui::IsAnyItemHovered();
}


static auto last_mouse_pos = Window::getMousePosition();

void Core::update(double delta_time) {
    controller();
}

void Core::controller() {
    if (Window::mouseButton(GLFW_MOUSE_BUTTON_1)) {
        if (UI::isUsingGizmo() && m_selected_object_index >= 0) {
            return;
        }
        auto mouse_pos = Window::getMousePosition();
        auto d_mouse = last_mouse_pos - mouse_pos;
        last_mouse_pos = mouse_pos;

        if (withinUIWindow(mouse_pos)) return;

        // Horizontal mouse movement (X) = azimuth (latitude, around Y axis)
        orbit.azimuth += d_mouse.x * 0.5f; // 0.5 sensitivity

        // Vertical mouse movement (Y) = elevation (longitude, angle from XZ plane)
        orbit.elevation -= d_mouse.y * 0.5f; // 0.5 sensitivity
        orbit.elevation = glm::clamp(orbit.elevation, -89.0f, 89.0f); // Prevent gimbal lock
    }

    float azimuth_rad = glm::radians(orbit.azimuth);
    float elevation_rad = glm::radians(orbit.elevation);
    glm::vec3 pos = glm::vec3(
        orbit.distance * cos(elevation_rad) * sin(azimuth_rad),
        orbit.distance * sin(elevation_rad),
        orbit.distance * cos(elevation_rad) * cos(azimuth_rad)
    );
    m_camera->setPosition(pos);
    m_camera->setLook(glm::normalize(orbit.target - pos));
}

void Core::mouseButton(int button, int action) {
    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
        last_mouse_pos = Window::getMousePosition();

        // Don't process clicks if we're interacting with UI or gizmo
        if ((UI::isUsingGizmo() && m_selected_object_index >= 0) || withinUIWindow(Window::getMousePosition())) {
            return;
        }

        // Get mouse position in NDC (-1 to 1)
        auto windowSize = Window::getWindowSize();
        glm::vec2 mousePos = Window::getMousePosition();
        float x = (2.0f * mousePos.x) / windowSize.x - 1.0f;
        float y = 1.0f - (2.0f * mousePos.y) / windowSize.y;

        // Create ray from camera through mouse position
        glm::vec4 rayClip(x, y, -1.0f, 1.0f);
        glm::mat4 projInv = glm::inverse(m_camera->getProjection(Window::getAspectRatio()));
        glm::vec4 rayEye = projInv * rayClip;
        rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

        glm::mat4 viewInv = glm::inverse(m_camera->getViewMatrix());
        glm::vec3 rayWorld = glm::vec3(viewInv * rayEye);
        rayWorld = glm::normalize(rayWorld);

        glm::vec3 rayOrigin = m_camera->getPosition();

        // Test ray against all selection sphere indicators
        float closestDist = std::numeric_limits<float>::max();
        int closestObject = -1;
        float indicatorRadius = GIZMO_INDICATOR_RADIUS; // Same as sphere scale in drawSelectionIndicators

        for (size_t i = 0; i < m_shapes.size(); i++) {
            glm::vec3 sphereCenter = m_shapes[i].transform->getPosition();

            // Ray-sphere intersection
            glm::vec3 oc = rayOrigin - sphereCenter;
            float a = glm::dot(rayWorld, rayWorld);
            float b = 2.0f * glm::dot(oc, rayWorld);
            float c = glm::dot(oc, oc) - indicatorRadius * indicatorRadius;
            float discriminant = b * b - 4 * a * c;

            if (discriminant >= 0) {
                float t = (-b - sqrt(discriminant)) / (2.0f * a);
                if (t > 0 && t < closestDist) {
                    closestDist = t;
                    closestObject = static_cast<int>(i);
                }
            }
        }

        // If we clicked on an indicator, select/deselect that object
        if (closestObject >= 0) {
            m_selected_object_index = closestObject; // Select clicked object
        }
        else {
            // Clicked on empty space - deselect
            m_selected_object_index = -1;
        }
    }
}

void Core::onScroll(double xoffset, double yoffset) {
    // if (withinUIWindow(Window::getMousePosition())) {
    //     return;
    // }
    float mod = 1.f * yoffset;
    orbit.distance -= mod;
    orbit.distance = glm::clamp(orbit.distance, 1.0f, 100.0f);
}
