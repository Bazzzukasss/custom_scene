#pragma once

#include "Common.h"

namespace custom_scene
{
/**
 * The Projection Class
 * @brief The class for projections of the camera.
 * It is used for calculation of view during the scene rendering.
 */
class Projection
{
 public:
    /**
     * @brief The coonstructor for the Projection
     * @param type - the projection type
     * @param ratio - the screen's viewport ratio
     */
    Projection(float ratio);
    virtual ~Projection() = default;

    /** setters */
    virtual void setRatio(float ratio) = 0;

    /** getters */
    float getRatio() const;
    const Mat4& get() const;
    virtual float getProjectionKoef(float cameraPosition, int viewWidth) const = 0;

 protected:
    Mat4 mProjection;
    float mRatio;
};

/**
 * The ProjectionOrtho Class
 * @brief The class represents orthogonal view projection
 */
class ProjectionOrtho : public Projection
{
 public:
    /**
     * @brief Constructor for ProjectionOrtho
     * @param min_x - minimal value by x axis
     * @param max_x - maximal value by x axis
     * @param min_y - minimal value by y axis
     * @param max_y - maximal value by y axis
     * @param min_z - minimal value by z axis
     * @param max_z - maximal value by z axis
     * @param ratio - screen viewport ratio
     */
    ProjectionOrtho(float minX,
                    float maxX,
                    float minY,
                    float maxY,
                    float minZ,
                    float maxZ,
                    float ratio,
                    const Vec3& scale = {1.0f, 1.0f, 1.0f});

    /** setters */
    void setRatio(float ratio) override;
    void setXRange(float min, float max);
    void setYRange(float min, float max);
    void setZRange(float min, float max);
    void setScale(const Vec3& scale);

    /** getters */
    float getProjectionKoef(float cameraPosition, int viewWidth) const override;
    float getProjectedX(float x, int viewWidth) const;
    inline std::pair<float, float> getXRange() const;
    inline std::pair<float, float> getYRange() const;
    inline std::pair<float, float> getZRange() const;

 private:
    void calculate();

    float mMinX;
    float mMaxX;
    float mMinY;
    float mMaxY;
    float mMinZ;
    float mMaxZ;
    Vec3 mScale;
};

/**
 * The ProjectionPerspective Class
 * @brief The class represents perspective view projection
 */
class ProjectionPerspective : public Projection
{
 public:
    /**
     * @brief The constructor for the ProjectionPerspective
     * @param alfa - the angle of the projection
     * @param near_distance - the distance to the near projection plane
     * @param far_distance - the distance to the far projection plane
     * @param ratio - the screen's viewport ratio
     */
    ProjectionPerspective(float alfa,
                          float nearD,
                          float farD,
                          float ratio);

    /** setters */
    void setRatio(float ratio) override;
    void setAlfa(float alfa);
    void setNear(float nearD);
    void setFar(float farD);

    /** getters */
    float getProjectionKoef(float cameraPosition, int viewWidth) const override;

 private:
    void calculate();

    float mAlfa;
    float mNearD;
    float mFarD;
};

}  // namespace gl_scene
