#pragma once

#include "Geometry.h"

namespace custom_scene
{

/**
 * The Mesh Class
 * @brief The class converts the geometry (the set of points) to the mesh (the set of vertices).
 * Meshes is used for rendering of the scene's items.
 */
class Mesh
{
public:
    Mesh();
    Mesh(const Geometry& geometry, std::function<void(Vertex&)> processor);

    /** getters */
    const std::vector<Vertex>& getVertices() const;
    const std::vector<uint>& getIndices() const;
    uint getElementsCount() const;

    Mesh& operator+=(const Mesh& rhv);

private:
    void calculateNormals();

private:
    std::vector<Vertex> mVertices;
    std::vector<uint> mIndices;
};

}
