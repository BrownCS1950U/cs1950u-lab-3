#pragma once

class Visuals {
public:

    static void drawDebugGrid();
    static void draw3DArrow(glm::vec3 start, glm::vec3 end, float shaft_radius, float tip_radius);

    static bool m_show_grid;

private:
    static const float m_grid_size;  // Grid extends from -10 to +10
    static const float m_cell_size;   // 1 unit per cell
};