//
// Created by Marcus Winter on 11/19/25.
//

#include <GL/glew.h>
#include "UI.h"

#include "../Core.h"
#include "Window.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "ImGuizmo.h"
#include "../render/Camera.h"
#include "../Transform.h"
#include <glm/gtc/type_ptr.hpp>


glm::vec2 UI::window_size = glm::vec2(400.0f, 600.0f);


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

    // Configure ImGuizmo to hide the center sphere
    ImGuizmo::Style& gizmoStyle = ImGuizmo::GetStyle();
    gizmoStyle.CenterCircleSize = 0.0f;  // Hide the white center sphere
}

glm::vec2 UI::getWindowSize() {
    return window_size;
}

void UI::update() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    // Settings window
    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize({window_size.x,window_size.y}, ImGuiCond_Once);

    ImGui::Begin("Settings");

    window_size = {ImGui::GetWindowSize().x,ImGui::GetWindowSize().y};
    ImGui::SeparatorText("Collision Debugger");

    // Object selection
    auto& shapes = m_core->getShapes();
    int selectedIndex = m_core->getSelectedObjectIndex();



    // Add Object button
    if (ImGui::Button("Add Object", ImVec2(120, 30))) {
        ImGui::OpenPopup("Add Object Popup");
    }

    // Add Object popup window
    if (ImGui::BeginPopupModal("Add Object Popup", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        m_core->addObjectGui();
        ImGui::EndPopup();
    }

    ImGui::SeparatorText("Collision info");

    m_core->collisionGui();

    if (selectedIndex == -1) {
        ImGui::Text("No object selected");
    } else {
        ImGui::Text("Selected Object: %d", selectedIndex);
    }
    m_core->selectedObjectGui(selectedIndex);

    // Gizmo operation selector
    static ImGuizmo::OPERATION currentOperation = ImGuizmo::TRANSLATE;
    static ImGuizmo::MODE currentMode = ImGuizmo::WORLD;
    //

    // if (ImGui::RadioButton("Scale", currentOperation == ImGuizmo::SCALE))
    //     currentOperation = ImGuizmo::SCALE;
    //
    // ImGui::Separator();
    // if (ImGui::RadioButton("World", currentMode == ImGuizmo::WORLD))
    //     currentMode = ImGuizmo::WORLD;
    // ImGui::SameLine();
    // if (ImGui::RadioButton("Local", currentMode == ImGuizmo::LOCAL))
    //     currentMode = ImGuizmo::LOCAL;



    // Advanced dropdown menu
    if (ImGui::CollapsingHeader("Advanced")) {
        // Add your advanced settings here
        ImGui::SeparatorText("Rotation");
        ImGui::Text("Make sure you have implemented rotation handling \nin your collision methods if using");

        ImGui::Text("Gizmo Control Mode:");
        ImGui::SameLine();
        if (ImGui::RadioButton("Translate", currentOperation == ImGuizmo::TRANSLATE))
            currentOperation = ImGuizmo::TRANSLATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Rotate", currentOperation == ImGuizmo::ROTATE))
            currentOperation = ImGuizmo::ROTATE;

        ImGui::SeparatorText("Debug Grid:");
        ImGui::Checkbox("Show Grid", &m_core->getShowGrid());

        ImGui::Separator();
    }



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

        // Draw and manipulate the gizmo for the selected object
        ImGuizmo::Manipulate(
            glm::value_ptr(view),
            glm::value_ptr(projection),
            currentOperation,
            currentMode,
            glm::value_ptr(model)
        );

        // If the gizmo was used, decompose the matrix and update the transform
        if (ImGuizmo::IsUsing()) {

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
        }
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool UI::isUsingGizmo() {
    // Use ImGuizmo's built-in functions to check if user is interacting with the gizmo
    // IsUsing() returns true when actively manipulating
    // IsOver() returns true when hovering over the gizmo

    return ImGuizmo::IsUsing() || ImGuizmo::IsOver();
}
