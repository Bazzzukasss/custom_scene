#include "Geometry.h"
#include "Utils.h"

namespace custom_scene
{

void Geometry::calculateNormals()
{
    auto poligonsCount = points.indices.size() / 3;
    for (uint poligon = 0; poligon < poligonsCount; poligon++)
    {
        uint p = poligon * 3;
        auto& p1 =  points.data.at(points.indices.at(p));
        auto& p2 =  points.data.at(points.indices.at(p + 1));
        auto& p3 =  points.data.at(points.indices.at(p + 2));

        const auto& normal = utils::calculateNormal(p1, p2, p3);

        normals.data.push_back(utils::toPoint3(normal));
        normals.indices.push_back(poligon);
        normals.indices.push_back(poligon);
        normals.indices.push_back(poligon);
    }
}

Geometry& Geometry::operator+=(const Geometry& rhv)
{
    points += rhv.points;
    normals += rhv.normals;
    colors += rhv.colors;
    textures += rhv.textures;

    return *this;
}

template<typename T>
Geometry::Attribute<T>& Geometry::Attribute<T>::operator+=(
        const Geometry::Attribute<T>& rhv)
{
    auto indexCount = indices.size();

    std::copy(rhv.data.begin(),
              rhv.data.end(),
              std::back_inserter(data));

    std::transform(rhv.indices.begin(),
                   rhv.indices.end(),
                   std::back_inserter(indices),
                   [&](uint value){
                        return value + indexCount;});

    return *this;
}

}
