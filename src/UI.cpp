//
// Created by Marcus Winter on 11/19/25.
//

#include <GL/glew.h>
#include "UI.h"

#include "Core.h"
#include "Window.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "ImGuizmo.h"
#include "render/Camera.h"
#include "render/Transform.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

bool UI::using_gizmo = false;

UI::UI(GLFWwindow* window, Core* core) : m_core(core) {
    initialize(window);
}

/**
 * Initialize ImGui UI for the given GLFW window.
 * @param window - Pointer to the GLFW window.
 */
void UI::initialize(GLFWwindow* window) {
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410 core");
    ImGui::StyleColorsClassic();
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg].w = 0.7f;
}

void UI::update() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    // Settings window
    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize({250, 300});
    ImGui::Begin("Settings");
    ImGui::Text("Collision Debugger");
    ImGui::Separator();

    // Object selection
    auto& shapes = m_core->getShapes();
    int selectedIndex = m_core->getSelectedObjectIndex();

    ImGui::Text("Selected Object: %d", selectedIndex + 1);
    ImGui::Text("Total Objects: %zu", shapes.size());
    ImGui::Separator();

    // Gizmo operation selector
    static ImGuizmo::OPERATION currentOperation = ImGuizmo::TRANSLATE;
    static ImGuizmo::MODE currentMode = ImGuizmo::WORLD;

    ImGui::Text("Gizmo Controls:");
    if (ImGui::RadioButton("Translate", currentOperation == ImGuizmo::TRANSLATE))
        currentOperation = ImGuizmo::TRANSLATE;
    if (ImGui::RadioButton("Rotate", currentOperation == ImGuizmo::ROTATE))
        currentOperation = ImGuizmo::ROTATE;
    if (ImGui::RadioButton("Scale", currentOperation == ImGuizmo::SCALE))
        currentOperation = ImGuizmo::SCALE;

    ImGui::Separator();
    if (ImGui::RadioButton("World", currentMode == ImGuizmo::WORLD))
        currentMode = ImGuizmo::WORLD;
    ImGui::SameLine();
    if (ImGui::RadioButton("Local", currentMode == ImGuizmo::LOCAL))
        currentMode = ImGuizmo::LOCAL;

    ImGui::Separator();
    ImGui::Text("Debug Grid:");
    ImGui::Checkbox("Show Grid", &m_core->getShowGrid());

    ImGui::End();

    // ImGuizmo manipulation
    if (!shapes.empty() && selectedIndex >= 0 && selectedIndex < shapes.size()) {
        auto camera = m_core->getCamera();
        auto& selectedObject = shapes[selectedIndex];

        // Get camera matrices
        glm::mat4 view = camera->getViewMatrix();
        glm::mat4 projection = camera->getProjection(Window::getAspectRatio());

        // Get object transform as a model matrix
        glm::mat4 model = selectedObject.transform->getModelMatrix();

        // Set ImGuizmo drawing area to the entire viewport
        auto windowSize = Window::getWindowSize();
        ImGuizmo::SetRect(0, 0, windowSize.x, windowSize.y);

        // Draw and manipulate the gizmo
        ImGuizmo::Manipulate(
            glm::value_ptr(view),
            glm::value_ptr(projection),
            currentOperation,
            currentMode,
            glm::value_ptr(model)
        );

        // If the gizmo was used, decompose the matrix and update the transform
        if (ImGuizmo::IsUsing()) {
            using_gizmo = true;
            // Extract translation
            glm::vec3 position(model[3][0], model[3][1], model[3][2]);

            // Extract scale
            glm::vec3 scale;
            scale.x = glm::length(glm::vec3(model[0]));
            scale.y = glm::length(glm::vec3(model[1]));
            scale.z = glm::length(glm::vec3(model[2]));

            // Extract rotation matrix by removing scale
            glm::mat4 rotationMatrix = model;
            rotationMatrix[0] /= scale.x;
            rotationMatrix[1] /= scale.y;
            rotationMatrix[2] /= scale.z;
            rotationMatrix[3] = glm::vec4(0, 0, 0, 1);

            // Update the transform
            selectedObject.transform->setPosition(position);
            selectedObject.transform->setScale(scale);
            selectedObject.transform->setRotation(rotationMatrix);
        } else {
            using_gizmo = false;
        }
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}


