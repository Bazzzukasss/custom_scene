#include "Generator.h"
#include "Utils.h"

namespace custom_scene
{

using namespace utils;

Geometry Generator::generate(const figures::Cube& figure)
{
    Geometry geometry;

    auto s = figure.size;

    std::vector<Point3f> points = {
        {-s, -s, -s}, {-s, s, -s}, {s, s, -s}, {s, -s, -s},
        {-s, -s, s}, {-s, s, s}, {s, s, s}, {s, -s, s}
    };

    geometry.points = {
        points,
        {
            0, 1, 2, 0, 2, 3, // top
            5, 4, 7, 5, 7, 6, // bottom
            5, 1, 0, 5, 0, 4, // left
            7, 3, 2, 7, 2, 6, // right
            4, 0, 3, 4, 3, 7, // front
            1, 6, 6, 1, 6, 2  // back
        }
    };

    geometry.normals = {
        {
            toPoint3(calculateNormal(points[0], points[1], points[2])), // top
            toPoint3(calculateNormal(points[5], points[4], points[7])), // bottom
            toPoint3(calculateNormal(points[5], points[1], points[0])), // left
            toPoint3(calculateNormal(points[7], points[3], points[2])), // right
            toPoint3(calculateNormal(points[4], points[0], points[3])), // front
            toPoint3(calculateNormal(points[1], points[6], points[6])), // back
        },
        {
            0, 1, 2, 3, 4, 5, 6, 7
        }
    };

    geometry.textures = {
        {
            {0, 0}, {0, 1}, {1, 1}, {1, 0}
        },
        {
            0, 1, 2, 0, 2, 3, // top
            0, 1, 2, 0, 2, 3, // bottom
            0, 1, 2, 0, 2, 3, // left
            0, 1, 2, 0, 2, 3, // right
            0, 1, 2, 0, 2, 3, // front
            0, 1, 2, 0, 2, 3  // back
        }
    };

    return geometry;
}

Geometry Generator::generate(const figures::Quad& figure)
{
    Geometry geometry;

    geometry.points = {
        {figure.p1, figure.p2, figure.p3, figure.p4},
        {0, 1, 2, 0, 2, 3}
    };

    geometry.normals = {
        {toPoint3(calculateNormal(figure.p1, figure.p2, figure.p3))},
        { 0, 0, 0, 0, 0, 0}
    };

    geometry.textures = {
        {{0, 0}, {0, 1}, {1, 1}, {1, 0}},
        {0, 1, 2, 0, 2, 3}
    };

    return geometry;
}

}
