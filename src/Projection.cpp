#include "Projection.h"
#include <QtMath>

namespace custom_scene
{

Projection::Projection(float ratio) :
    mRatio(ratio)
{}

float Projection::getRatio() const
{
    return mRatio;
}

const Mat4& Projection::get() const
{
    return mProjection;
}

ProjectionOrtho::ProjectionOrtho(float minX,
                                 float maxX,
                                 float minY,
                                 float maxY,
                                 float minZ,
                                 float maxZ,
                                 float ratio,
                                 const Vec3& scale) :
    Projection(ratio),
    mMinX(minX),
    mMaxX(maxX),
    mMinY(minY),
    mMaxY(maxY),
    mMinZ(minZ),
    mMaxZ(maxZ),
    mScale(scale)
{
    calculate();
}

void ProjectionOrtho::setRatio(float ratio)
{
    mRatio = ratio;
    calculate();
}

void ProjectionOrtho::setXRange(float min, float max)
{
    mMinX = min;
    mMaxX = max;
    calculate();
}

void ProjectionOrtho::setYRange(float min, float max)
{
    mMinY = min;
    mMaxY = max;
    calculate();
}

void ProjectionOrtho::setZRange(float min, float max)
{
    mMinZ = min;
    mMaxZ = max;
    calculate();
}

void ProjectionOrtho::setScale(const Vec3& scale)
{
    mScale = scale;
    calculate();
}

float ProjectionOrtho::getProjectionKoef(float, int view_width) const
{
    return (mMaxX - mMinX) * mRatio * mScale.x() / static_cast<float>(view_width);
}

float ProjectionOrtho::getProjectedX(float x, int viewWidth) const
{
    return (mMinX + (mMaxX - mMinX) * x / viewWidth) * mScale.x() * mRatio;
}

std::pair<float, float> ProjectionOrtho::getXRange() const
{
    return {mMinX, mMaxX};
}

std::pair<float, float> ProjectionOrtho::getYRange() const
{
    return {mMinY, mMaxY};
}

std::pair<float, float> ProjectionOrtho::getZRange() const
{
    return {mMinZ, mMaxZ};
}

void ProjectionOrtho::calculate()
{
    mProjection.setToIdentity();
    mProjection.ortho(mMinX * mRatio * mScale.x(),
                      mMaxX * mRatio * mScale.x(),
                      mMinY * mScale.y(),
                      mMaxY * mScale.y(),
                      mMinZ * mScale.z(),
                      mMaxZ * mScale.z());
}

ProjectionPerspective::ProjectionPerspective(
        float alfa,
        float nearD,
        float farD,
        float ratio) :
    Projection(ratio),
    mAlfa(alfa),
    mNearD(nearD),
    mFarD(farD)
{
    calculate();
}

float ProjectionPerspective::getProjectionKoef(float cameraPosition,
                                               int viewWidth) const
{
    return cameraPosition * tanf(qDegreesToRadians(mAlfa)) * mRatio /
            static_cast<float>(viewWidth);
}

void ProjectionPerspective::setRatio(float ratio)
{
    mRatio = ratio;
    calculate();
}

void ProjectionPerspective::setAlfa(float alfa)
{
    mAlfa = alfa;
    calculate();
}

void ProjectionPerspective::setNear(float minD)
{
    mNearD = minD;
    calculate();
}

void ProjectionPerspective::setFar(float maxD)
{
    mFarD = maxD;
    calculate();
}

void ProjectionPerspective::calculate()
{
    mProjection.setToIdentity();
    mProjection.perspective(mAlfa, mRatio, mNearD, mFarD);
}

}
