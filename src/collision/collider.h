#ifndef VIEWER_COLLIDER_H
#define VIEWER_COLLIDER_H

#include <glm/glm.hpp>
#include <iostream>
#include <utility>
#include "../tiny_obj_loader.h"

struct Collider {
public:
    Collider(int id, glm::vec3 pos) :
        id(id), initPos(pos), pos(pos) {};
    int id;
    const glm::vec3 initPos;
    glm::vec3 pos;
};

struct CylinderCollider : public Collider {
public:
    CylinderCollider(int id, glm::vec3 pos, float radius, float height) :
        Collider(id, pos), radius(radius), height(height) {}

    float radius;
    float height;
};

struct EllipsoidCollider : public Collider {
public:
    EllipsoidCollider(int id, glm::vec3 pos, glm::vec3 radii) :
        Collider(id, pos), radii(radii) {}

    glm::vec3 radii;
};

struct ConvexCollider : public Collider {
public:
    ConvexCollider(int id, glm::vec3 pos, std::string filepath) :
            Collider(id, pos), filepath(std::move(filepath)) {
        loadFromFile();
    }

    std::string filepath;
    std::vector<glm::vec3> vertices;

private:
    void loadFromFile() {
        tinyobj::ObjReaderConfig config;
        config.triangulation_method = "earcut";
        config.triangulate = true;
        config.vertex_color = false;

        tinyobj::ObjReader reader;
        if (!reader.ParseFromFile(filepath, config)) {
            if (!reader.Error().empty()) {
                std::cerr << "TinyObjReader Error: " << reader.Error() << '\n';
            }
            return;
        }

        if (!reader.Warning().empty()) {
            std::cout << "TinyObjReader Warning: " << reader.Warning() << '\n';
        }

        const auto& attrib = reader.GetAttrib();
        const auto& shapes = reader.GetShapes();

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                // Extract vertex position
                glm::vec3 vertex(
                        attrib.vertices[3 * index.vertex_index + 0],
                        attrib.vertices[3 * index.vertex_index + 1],
                        attrib.vertices[3 * index.vertex_index + 2]
                );

                vertices.push_back(vertex);
            }
        }
    }
};

struct StaticMesh {
public:
    StaticMesh(std::string filepath) :
        filepath(std::move(filepath)) {
        loadFromFile();
    }

    struct Triangle {
        glm::vec3 v0;
        glm::vec3 v1;
        glm::vec3 v2;
        glm::vec3 normal;
    };

    std::string filepath;
    std::vector<Triangle> triangles;

private:
    void loadFromFile() {
        tinyobj::ObjReaderConfig config;
        config.triangulation_method = "earcut";
        config.triangulate = true;
        config.vertex_color = false;

        tinyobj::ObjReader reader;
        if (!reader.ParseFromFile(filepath, config)) {
            if (!reader.Error().empty()) {
                std::cerr << "TinyObjReader Error: " << reader.Error() << '\n';
            }
            return;
        }

        if (!reader.Warning().empty()) {
            std::cout << "TinyObjReader Warning: " << reader.Warning() << '\n';
        }

        const auto& attrib = reader.GetAttrib();
        const auto& shapes = reader.GetShapes();

        for (const auto& shape : shapes) {
            size_t indexOffset = 0;
            for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f) {
                int fv = shape.mesh.num_face_vertices[f];
                if (fv != 3) {
                    throw std::runtime_error("Non-triangular face detected.");
                }

                glm::vec3 vertices[3];
                glm::vec3 normal;
                for (size_t v = 0; v < 3; ++v) {
                    tinyobj::index_t idx = shape.mesh.indices[indexOffset + v];

                    // Extract vertex position
                    vertices[v] = glm::vec3(
                            attrib.vertices[3 * idx.vertex_index + 0],
                            attrib.vertices[3 * idx.vertex_index + 1],
                            attrib.vertices[3 * idx.vertex_index + 2]
                    );

                    // Extract normal if available
                    if (idx.normal_index >= 0) {
                        normal = glm::vec3(
                                attrib.normals[3 * idx.normal_index + 0],
                                attrib.normals[3 * idx.normal_index + 1],
                                attrib.normals[3 * idx.normal_index + 2]
                        );
                    }
                }

                Triangle tri{};
                tri.v0 = vertices[0];
                tri.v1 = vertices[1];
                tri.v2 = vertices[2];
                tri.normal = normal;
                triangles.push_back(tri);

                indexOffset += fv;
            }
        }
    }
};

#endif //VIEWER_COLLIDER_H
