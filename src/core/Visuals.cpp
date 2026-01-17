#include "Visuals.h"

#include "../render/Graphics.h"
#include <string>

bool Visuals::m_show_grid = true;
const float Visuals::m_grid_size = 10.0f;  // Grid extends
const float Visuals::m_cell_size = 1.0f;   // 1 unit per cell

void Visuals::drawDebugGrid() {
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

void Visuals::draw3DArrow(glm::vec3 start, glm::vec3 end, float shaft_radius, float tip_radius) {
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
