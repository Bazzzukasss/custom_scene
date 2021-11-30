#include "Utils.h"

namespace custom_scene
{

namespace utils
{

Point3f toPoint3(const Vec3& vec)
{
    return {vec.x(), vec.y(), vec.z()};
}

Vec3 toVec3(const Color& color)
{
    return {static_cast<float>(color.redF()),
            static_cast<float>(color.greenF()),
            static_cast<float>(color.blueF())};
}

Vec3 toWorldXYCoordinates(const Point2i& point,
                          std::pair<int, int> viewPortSize,
                          const Mat4& view,
                          const Mat4& projection,
                          float worldZ)
{
    Vec3 worldNear = toWorldCoordinates({point[0], point[1]},
                                        viewPortSize,
                                        view,
                                        projection,
                                        0.0f);

    Vec3 worldFar = toWorldCoordinates({point[0], point[1]},
                                       viewPortSize,
                                       view,
                                       projection,
                                       1.0f);

    if (worldFar.z() > worldNear.z() - 0.001f)
    {
        worldFar.setZ(worldNear.z() - 0.001f);
    }

    auto worldDir = worldFar - worldNear;
    float res = !qFuzzyIsNull(worldDir.z())
            ? (worldZ - worldNear.z()) / worldDir.z()
            : worldZ - worldNear.z();

    auto worldX = !qFuzzyIsNull(worldDir.x())
            ? res * worldDir.x() + worldNear.x()
            : res + worldNear.x();
    auto worldY = !qFuzzyIsNull(worldDir.y())
            ? res * worldDir.y() + worldNear.y()
            : res + worldNear.y();

    Vec3 pos = {worldX, worldY, worldZ};

    return pos;
}

std::vector<Point3f> toWorldXYCoordinates(
        const std::vector<Point2i>& screenPoints,
        std::pair<int, int> viewPortSize,
        const Mat4 &view,
        const Mat4 &projection,
        float worldZ)
{
    std::vector<Point3f> points;

    for (const auto& point : screenPoints)
    {
        const auto& worldPoint = toWorldXYCoordinates(point,
                                                      viewPortSize,
                                                      view,
                                                      projection,
                                                      worldZ);
        points.push_back(toPoint3(worldPoint));
    }

    return points;
}

Vec3 toWorldCoordinates(const Point2i& screenPoint,
                        float distance,
                        std::pair<int, int> viewPortSize,
                        const Mat4& view,
                        const Mat4& projection)
{
    int screenW = viewPortSize.first;
    int screenH = viewPortSize.second;
    Vec3 screenNear(screenPoint[0], screenH - screenPoint[1], distance);

    return screenNear.unproject(view,
                                projection,
                                QRect(0, 0, screenW, screenH));
}

std::vector<Point3f> toWorldCoordinates(const std::vector<Point2i>& screenPoints,
                                       float distance,
                                       std::pair<int, int> viewPortSize,
                                       const Mat4& view,
                                       const Mat4& projection)
{
    std::vector<Point3f> points;
    for (const auto& point : screenPoints)
    {
        const auto& worldPoint = toWorldCoordinates(point,
                                                    viewPortSize,
                                                    view,
                                                    projection,
                                                    distance);
        points.push_back(toPoint3(worldPoint));
    }

    return points;
}

Vec2 toScreenCoordinates(const Vec3& worldPoint, const Mat4& transformation)
{
    const auto& position = transformation * QVector4D(worldPoint, 1.0);

    return {position.x(), position.y()};
}

Vec3 calculateNormal(const Point3f& p1, const Point3f& p2, const Point3f& p3)
{
    Vec3 edge1{p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2]};
    Vec3 edge2{p3[0] - p1[0], p3[1] - p1[1], p3[2] - p1[2]};

    return QVector3D::crossProduct(edge1, edge2);
}

}
}
