#include "Mesh.h"
#include "Utils.h"

namespace custom_scene
{

Mesh::Mesh()
{
}

Mesh::Mesh(const Geometry& geometry, std::function<void (Vertex&)> processor)
{
    auto isNormalsPresent = geometry.normals.data.empty();
    auto isColorsPresent = geometry.colors.data.empty();
    auto isTexturesPresent = geometry.textures.data.empty();

    auto pointsIndexPtr = geometry.points.indices.begin();
    auto normalsIndexPtr = geometry.normals.indices.begin();
    auto colorsIndexPtr = geometry.colors.indices.begin();
    auto texturesIndexPtr = geometry.textures.indices.begin();

    while (pointsIndexPtr != geometry.points.indices.end())
    {
        Vertex vertex;

        vertex.position = geometry.points.data.at(*pointsIndexPtr++);

        vertex.normal =  isNormalsPresent
                ? Point3f{0, 0, 0}
                : geometry.normals.data.at(*normalsIndexPtr++);
        vertex.color = isColorsPresent
                ? Point3f{0, 0, 0}
                : geometry.colors.data.at(*colorsIndexPtr++);
        vertex.texture = isTexturesPresent
                ? Point2f{0,0}
                : geometry.textures.data.at(*texturesIndexPtr++);

        if (processor)
        {
            processor(vertex);
        }

        mVertices.push_back(vertex);
    }

    mIndices = geometry.points.indices;
}

const std::vector<Vertex>& Mesh::getVertices() const
{
    return mVertices;
}

const std::vector<uint>& Mesh::getIndices() const
{
    return mIndices;
}

uint Mesh::getElementsCount() const
{
    return mIndices.empty() ? mVertices.size() : mIndices.size();
}

Mesh& Mesh::operator+=(const Mesh& rhv)
{
    auto indexCount = mIndices.size();

    std::copy(rhv.mVertices.begin(),
              rhv.mVertices.end(),
              std::back_inserter(mVertices));

    std::transform(rhv.mIndices.begin(),
                   rhv.mIndices.end(),
                   std::back_inserter(mIndices),
                   [&](uint value){
                        return value + indexCount;});
    return *this;
}

}
