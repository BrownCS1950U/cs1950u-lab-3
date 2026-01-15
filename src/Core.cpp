#include "Core.h"

#include <iostream>

#include "Debug.h"
#include "Window.h"
#include "collision/Collider.h"
#include "render/Camera.h"
#include "render/Mesh.h"
#include "render/SkeletalMesh.h"
#include "render/shapes/Cone.h"
#include "render/shapes/Cube.h"
#include "render/shapes/Cylinder.h"
#include "render/shapes/Quad.h"
#include "render/shapes/Sphere.h"



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

    auto cube = gl::Cube(1);
    auto cone = gl::Cone(8,4);
    auto cylinder = gl::Cylinder(8,1);
    auto sphere = gl::Sphere(8,8);
    auto quad = gl::Quad(1);

    gl::Graphics::addShape("cube", cube.loadDrawShape());
    gl::Graphics::addShape("cone", cone.loadDrawShape());
    gl::Graphics::addShape("cylinder", cylinder.loadDrawShape());
    gl::Graphics::addShape("sphere", sphere.loadDrawShape());
    gl::Graphics::addShape("quad", quad.loadDrawShape());



    auto object = Object("sphere");
    object.transform->setPosition(glm::vec3(-1, 0, 0));
    object.collider = new SphereCollider(object.transform);


    auto object2 = Object("sphere");
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
        } else if (glm::dot(up, normalized_dir) < 0) {
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
        } else if (glm::dot(up, normalized_dir) < 0) {
            // Pointing down, rotate 180 degrees
            tip_transform.rotate(glm::pi<float>(), glm::vec3(1, 0, 0));
        }

        gl::Graphics::drawObject(tip, tip_transform, ARROW_MATERIAL);
    }
}

void Core::drawDebugGrid() {
    if (!m_show_grid) return;

    // Colors for axes (standard RGB convention)
    const glm::vec3 X_AXIS_POS_COLOR(1.0f, 0.0f, 0.0f);  // Red
    const glm::vec3 X_AXIS_NEG_COLOR(0.5f, 0.0f, 0.0f);  // Dark Red
    const glm::vec3 Y_AXIS_POS_COLOR(0.0f, 1.0f, 0.0f);  // Green
    const glm::vec3 Y_AXIS_NEG_COLOR(0.0f, 0.5f, 0.0f);  // Dark Green
    const glm::vec3 Z_AXIS_POS_COLOR(0.0f, 0.0f, 1.0f);  // Blue
    const glm::vec3 Z_AXIS_NEG_COLOR(0.0f, 0.0f, 0.5f);  // Dark Blue
    const glm::vec3 GRID_COLOR(0.7);        // Gray

    const float AXIS_LINE_WIDTH = 3.0f;
    const float GRID_LINE_WIDTH = 1.0f;

    // Draw XZ plane grid
    for (float i = -m_grid_size; i <= m_grid_size; i += m_cell_size) {
        // Lines parallel to X axis
        if (i==0.0f) continue; // Skip axis line, drawn later
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

void Core::draw() {
    gl::Graphics::usePhongShader();
    gl::Graphics::setCameraUniforms(m_camera.get());
    gl::Graphics::setLight(*m_light);

    // Draw debug grid and axes
    drawDebugGrid();

    for (size_t i = 0; i < m_shapes.size(); i++) {
        auto& obj = m_shapes[i];
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        gl::Graphics::drawObject(obj.shape, *obj.transform, obj.material);
        for (size_t j = i; j < m_shapes.size(); j++) {
            if (i == j) continue;
            auto& other = m_shapes[j];
            if (obj.collider && other.collider) {
                obj.mtv = obj.collider->getMTV(*other.collider);
                other.mtv = other.collider->getMTV(*obj.collider);
            }
        }

        // Draw mtv
        if (obj.mtv.collision && obj.mtv.mtv) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            glm::vec3 center = obj.transform->getPosition();
            glm::vec3 mtv_vector = *obj.mtv.mtv;
            glm::vec3 arrow_end = center + mtv_vector;

            // Draw arrow with appropriate sizing
            float shaft_radius = 0.05f;
            float tip_radius = 0.1f;

            draw3DArrow(center, arrow_end, shaft_radius, tip_radius);
        }
    }

}

static glm::vec2 rotation(0.0f, 0.0f);
static auto last_mouse_pos = Window::getMousePosition();
void Core::update(double delta_time) {
    controller(delta_time);



    if (!Window::isCursorVisible()) {
        auto mouse_pos = Window::getMousePosition();
        auto d_mouse = 0.1f*(last_mouse_pos - mouse_pos);
        last_mouse_pos = mouse_pos;
        rotation.x += d_mouse.y;
        rotation.y += d_mouse.x;
        rotation.x = glm::clamp(rotation.x, -89.0f, 89.0f);


        glm::vec3 newFront;
        newFront.x = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
        newFront.y = sin(glm::radians(rotation.x));
        newFront.z = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
        auto newLook = glm::normalize(newFront);

        m_camera->setLook(newLook);
    }
}

void Core::controller(double delta_time) {
    float mod = 4.f*delta_time;

    auto camXZ = [](glm::vec3 look) {
        auto cam_xz = look * glm::vec3(1,0,1);
        cam_xz = glm::normalize(cam_xz);
        return cam_xz;
    };
    if (Window::key(GLFW_KEY_W)) {
        m_camera->setPosition(m_camera->getPosition() + mod * camXZ(m_camera->getLook()));
    }
    if (Window::key(GLFW_KEY_S)) {
        m_camera->setPosition(m_camera->getPosition() - mod * camXZ(m_camera->getLook()));
    }
    if (Window::key(GLFW_KEY_A)) {
        m_camera->setPosition(m_camera->getPosition() - mod * camXZ(m_camera -> getRight()));
    }
    if (Window::key(GLFW_KEY_D)) {
        m_camera->setPosition(m_camera->getPosition() + mod * camXZ(m_camera -> getRight()));
    }
    if (Window::key(GLFW_KEY_SPACE)) {
        m_camera->setPosition(m_camera->getPosition() + mod * m_camera->getUp());
    }
    if (Window::key(GLFW_KEY_LEFT_SHIFT)) {
        m_camera->setPosition(m_camera->getPosition() - mod * m_camera -> getUp());
    }

    if (Window::key(GLFW_KEY_ENTER)) {
        Window::hideMouse();
    }
    if (Window::key(GLFW_KEY_ESCAPE)) {
        Window::showMouse();
    }

    if (Window::mouseButton(GLFW_MOUSE_BUTTON_1) && !UI::isUsingGizmo()) {
        auto mouse_pos = Window::getMousePosition();
        auto d_mouse = last_mouse_pos - mouse_pos;
        last_mouse_pos = mouse_pos;

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

void Core::keyPressed(int key) {
    switch (key) {
    case GLFW_KEY_P: {
        animation_playing = !animation_playing;
        break;
    }
    case GLFW_KEY_TAB: {
        // Cycle through objects
        m_selected_object_index = (m_selected_object_index + 1) % m_shapes.size();
        break;
    }
    case GLFW_KEY_LEFT_BRACKET: {
        // Previous object
        m_selected_object_index = (m_selected_object_index - 1 + m_shapes.size()) % m_shapes.size();
        break;
    }
    case GLFW_KEY_RIGHT_BRACKET: {
        // Next object
        m_selected_object_index = (m_selected_object_index + 1) % m_shapes.size();
        break;
    }
    case GLFW_KEY_1:
    case GLFW_KEY_2:
    case GLFW_KEY_3:
    case GLFW_KEY_4:
    case GLFW_KEY_5:
    case GLFW_KEY_6:
    case GLFW_KEY_7:
    case GLFW_KEY_8:
    case GLFW_KEY_9: {
        // Select object by number (1-9)
        int index = key - GLFW_KEY_1;
        if (index < m_shapes.size()) {
            m_selected_object_index = index;
        }
        break;
    }
    }
}

void Core::mouseButton(int button, int action) {
    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
        last_mouse_pos = Window::getMousePosition();

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



