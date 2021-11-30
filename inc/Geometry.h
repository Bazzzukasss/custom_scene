#pragma once

#include "Common.h"

namespace custom_scene
{

struct Vertex
{
    Point3f position;
    Point3f normal;
    Point3f color;
    Point2f texture;
};

struct Geometry
{
    template<typename T>
    struct Attribute
    {
        std::vector<T> data;
        std::vector<uint> indices;

        Attribute& operator+=(const Attribute& rhv);
    };

    Attribute<Point3f> points;
    Attribute<Point3f> normals;
    Attribute<Point3f> colors;
    Attribute<Point2f> textures;

    void calculateNormals();
    Geometry& operator+=(const Geometry& rhv);
};

}
