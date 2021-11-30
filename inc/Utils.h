#pragma once

#include "Common.h"

namespace custom_scene
{

namespace utils
{

extern Point3f toPoint3(const Vec3& vec);

extern Vec3 toVec3(const Color& color);

/**
 * @brief Projects the screen's point to the OXY plane
 * @param screen_x - the screen's point x coordinate
 * @param screen_y - the screen's point y coordinate
 * @param world_z - the z coordinate of the OXY plane
 * @return Point in world coordinates
 */
extern Vec3 toWorldXYCoordinates(const Point2i& point,
                                 std::pair<int, int> viewPortSize,
                                 const Mat4& view,
                                 const Mat4& projection,
                                 float worldZ = 0.0f);

/**
 * @brief Projects the screen's points to the OXY plane
 * @param screen_points - the container with screen's points
 * @param world_z - the z coordinate of the OXY plane
 * @return Point's in  world coordinates
 */
extern std::vector<Point3f> toWorldXYCoordinates(
        const std::vector<Point2i>& screenPoints,
        std::pair<int, int> viewPortSize,
        const Mat4& view,
        const Mat4& projection,
        float worldZ = 0.0f);

/**
 * @brief Projects the screen's point to the camera plane
 * @param screen_x - the screen's point x coordinate
 * @param screen_y - the screen's point y coordinate
 * @param distance - the relative value of camera plane (0 coresponds to near plane, 1 coresponds to far plane)
 * @return Point in world coordinates
 */
extern Vec3 toWorldCoordinates(const Point2i& screenPoint,
                               std::pair<int, int> viewPortSize,
                               const Mat4& view,
                               const Mat4& projection,
                               float distance);

/**
 * @brief Projects the screen's point to the camera plane
 * @param screen_points - the container with screen's points
 * @param distance - the relative value of camera plane (0 coresponds to near plane, 1 coresponds to far plane)
 * @return Points in world coordinates
 */
extern std::vector<Point3f> toWorldCoordinates(
        const std::vector<Point2i>& screenPoints,
        std::pair<int, int> viewPortSize,
        const Mat4& view,
        const Mat4& projection,
        float distance);

/**
 * @brief Projects the world's point to the screen
 * @param world_point - the world's point coordinates
 * @return Point's screen coordinates
 */
extern Vec2 toScreenCoordinates(const Vec3& worldPoint,
                                const Mat4& transformation);

Vec3 calculateNormal(const Point3f& v1, const Point3f& v2, const Point3f& v3);

}
}
