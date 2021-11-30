#include "Camera.h"
#include "Utils.h"
#include "Projection.h"
#include "Manipulator.h"

#include <QtMath>

namespace custom_scene
{

Camera::Camera(const Parameters& parameters,
               const std::vector<std::shared_ptr<Projection>>& projections,
               std::shared_ptr<Manipulator> manipulator) :
    mYaw(parameters.yaw),
    mPitch(parameters.pitch),
    mSpeed(parameters.speed),
    mSensitivity(parameters.sensitivity),
    mZoom(parameters.zoom),
    mScale(parameters.scale),
    mPosition(parameters.position),
    mWorldUp(parameters.up),
    mDefaultParameters(parameters),
    mProjections(projections),
    mManipulator(manipulator)
{
    mManipulator->setCamera(this);
    setProjectionIndex(0);
    update();
}

void Camera::reset()
{
    mYaw = mDefaultParameters.yaw;
    mPitch = mDefaultParameters.pitch;
    mSpeed = mDefaultParameters.speed;
    mSensitivity = mDefaultParameters.sensitivity;
    mZoom = mDefaultParameters.zoom;
    mScale = mDefaultParameters.scale;
    mPosition = mDefaultParameters.position;
    mWorldUp = mDefaultParameters.up;

    setProjectionIndex(0);
    update();
}

void Camera::move(MovementDirection direction, float delta)
{
    auto k = mCurrentProjection->getProjectionKoef(mPosition.z(),
                                                   mViewPortSize.first);
    switch (direction)
    {
        case MovementDirection::kXYForward:
            moveXYFront(delta * k);
            break;
        case MovementDirection::kForward:
            moveFront(mSpeed * delta);
            break;
        case MovementDirection::kRight:
            moveRight(delta * k);
            break;
        case MovementDirection::kUp:
            moveUp(delta * k);
            break;
    }
}

void Camera::moveFront(float delta)
{
    move(mFront * delta);
}

void Camera::moveXYFront(float delta)
{
    move(mXYFront * delta);
}

void Camera::moveUp(float delta)
{
    move(mUp * delta);
}

void Camera::moveRight(float delta)
{
    move(mRight * delta);
}

void Camera::move(const Vec3& delta)
{
    mPosition += delta;
    update(false, true);
}

void Camera::rotateAround(int deltaX, int deltaY)
{
    rotateAround(static_cast<float>(deltaX) * mSensitivity,
                 static_cast<float>(deltaY) * mSensitivity);
}

void Camera::rotateAround(float deltaYaw, float deltaPitch)
{
    auto oldFront = mFront;
    rotate(deltaYaw, deltaPitch);
    mPosition -= (mFront - oldFront) * mLook.length();

    update(false);
}

void Camera::rotate(int deltaX, int deltaY)
{
    rotate(static_cast<float>(deltaX) * mSensitivity,
           static_cast<float>(deltaY) * mSensitivity);
}

void Camera::rotate(float deltaYaw, float deltaPitch)
{
    mYaw += deltaYaw;
    mPitch += deltaPitch;

    update();
}

void Camera::zoom(float delta)
{
    mZoom -= delta * mSensitivity;

    update(false);
}

void Camera::scale(const Vec3& deltaScale)
{
    mScale += deltaScale * mSensitivity;

    update(false, false);
}

void Camera::focus(const Vec3& point)
{
    auto delta = point - mLookPoint;
    delta.setZ(0.0f);
    mPosition += delta;

    update(false, true);
}

void Camera::switchProjection()
{
    mCurrentProjectionIndex++;
    setProjectionIndex(mCurrentProjectionIndex);
}

Vec3 Camera::toWorldXYCoordinates(const Point2i& point, float worldZ)
{
    return utils::toWorldXYCoordinates(point,
                                       mViewPortSize,
                                       mViewMatrix,
                                       mCurrentProjection->get(),
                                       worldZ);
}

std::vector<Point3f> Camera::toWorldXYCoordinates(
        const std::vector<Point2i>& screenPoints,
        float worldZ)
{
    return utils::toWorldXYCoordinates(screenPoints,
                                       mViewPortSize,
                                       mViewMatrix,
                                       mCurrentProjection->get(),
                                       worldZ);
}

Vec3 Camera::toWorldCoordinates(const Point2i& screenPoint, float distance)
{
    return utils::toWorldCoordinates(screenPoint,
                                     mViewPortSize,
                                     mViewMatrix,
                                     mCurrentProjection->get(),
                                     distance);
}

std::vector<Point3f> Camera::toWorldCoordinates(
        const std::vector<Point2i>& screenPoints,
        float distance)
{
    return utils::toWorldCoordinates(screenPoints,
                                     mViewPortSize,
                                     mViewMatrix,
                                     mCurrentProjection->get(),
                                     distance);
}

void Camera::update(bool use_angles, bool update_look)
{
    mManipulator->checkRangeLimits();

    if (use_angles)
    {
        float yaw = qDegreesToRadians(mYaw);
        float pitch = qDegreesToRadians(mPitch);

        mFront.setX(cosf(yaw) * cosf(pitch));
        mFront.setY(sinf(yaw) * cosf(pitch));
        mFront.setZ(sinf(pitch));
        mFront.normalize();

        mRight = QVector3D::crossProduct(mFront, mWorldUp);
        mRight.normalize();

        mUp = QVector3D::crossProduct(mRight, mFront);
        mUp.normalize();

        auto xyMove = mFront + mUp;
        mXYFront = {xyMove.x(), xyMove.y(), 0.0f};
    }

    calculateViewMatrix();

    if (update_look)
    {
        mLookPoint = toWorldXYCoordinates({mViewPortSize.first / 2,
                                           mViewPortSize.second / 2});
        mLook = getPosition() - mLookPoint;
    }

    if (auto projection
            = std::dynamic_pointer_cast<ProjectionOrtho>(mCurrentProjection))
    {
        projection->setScale(mScale);
    }
}

void Camera::calculateViewMatrix()
{
    mViewMatrix.setToIdentity();
    mViewMatrix.lookAt(mPosition, mPosition + mFront, mUp);
    mViewMatrix.scale(mZoom);
}

std::shared_ptr<Manipulator> Camera::getManipulator() const
{
    return mManipulator;
}

void Camera::setPitch(float pitch)
{
    mPitch = pitch;
    update();
}

void Camera::setYaw(float yaw)
{
    mYaw = yaw;
    update();
}

void Camera::setZoom(float zoom)
{
    mZoom = zoom;
    update();
}

void Camera::setScale(const Vec3& scale)
{
    mScale = scale;
    update(false, false);
}

void Camera::setPosition(const Vec3& position)
{
    mPosition = position;
    update(false, true);
}

void Camera::setSeneitivity(float sensetivity)
{
    mSensitivity = sensetivity;
}

void Camera::setSpeed(float speed)
{
    mSpeed = speed;
}

void Camera::setProjectionIndex(int index)
{
    if (mProjections.empty())
    {
        return;
    }

    mCurrentProjectionIndex = index;

    if (mCurrentProjectionIndex >= mProjections.size())
    {
        mCurrentProjectionIndex = 0;
    }

    mCurrentProjection = mProjections[mCurrentProjectionIndex];
}
void Camera::setViewPort(int veiw_width, int view_height)
{
    float ratio =
            static_cast<float>(veiw_width) /
            static_cast<float>(view_height);

    mViewPortSize = {veiw_width, view_height};

    for (auto& projection : mProjections)
    {
        projection->setRatio(ratio);
    }

    update(false, true);
}

bool Camera::isProjectionPerspective() const
{
    return dynamic_cast<ProjectionPerspective*>(mCurrentProjection.get());
}

Mat4 Camera::getTransformation() const
{
    return getProjection() * getView();
}

const Mat4 &Camera::getView() const
{
    return mViewMatrix;
}

const Mat4 &Camera::getProjection() const
{
    return mCurrentProjection->get();
}

float Camera::getYaw() const
{
    return mYaw;
}

float Camera::getPitch() const
{
    return mPitch;
}

float Camera::getZoom() const
{
    return mZoom;
}

const Vec3 &Camera::getScale() const
{
    return mScale;
}

const Vec3 &Camera::getPosition() const
{
    return mPosition;
}

const Vec3 &Camera::getRight() const
{
    return mRight;
}

const Vec3 &Camera::getUp() const
{
    return mUp;
}

const Vec3 &Camera::getFront() const
{
    return mFront;
}

const Vec3 &Camera::getLook() const
{
    return mLook;
}

const Vec3 &Camera::getLookPoint() const
{
    return mLookPoint;
}

const std::pair<int, int> &Camera::getViewPortSize() const
{
    return mViewPortSize;
}

}
