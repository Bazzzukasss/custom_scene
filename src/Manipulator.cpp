#include "Manipulator.h"
#include "Projection.h"
#include "Utils.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QtMath>

namespace custom_scene
{

Manipulator::Manipulator(const RangeLimits& rangeLimits) :
    mRangeLimits(rangeLimits)
{}

void Manipulator::setZoomSensitivity(int sensitivity)
{
    mZoomSensitivity = sensitivity;
}

void Manipulator::setRectZoomMode(bool isZoomMode)
{
    mIsRectZoomMode = isZoomMode;
}

void Manipulator::setRectSelectionMode(bool isSelectionMode)
{
    mIsRectSelectionMode = isSelectionMode;
}

int Manipulator::getZoomSensitivity() const
{
    return mZoomSensitivity;
}

bool Manipulator::isRectZoomMode() const
{
    return mIsRectZoomMode;
}

bool Manipulator::isRectSelectionMode() const
{
    return mIsRectSelectionMode;
}

bool Manipulator::isDragMode() const
{
    return mIsDragMode;
}

void Manipulator::reset()
{
    mCamera->reset();
}

void Manipulator::focus(const Vec3&)
{}

void Manipulator::checkRangeLimits()
{
    auto position = mCamera->getPosition();
    auto scale = mCamera->getScale();
    auto pitch = mCamera->getPitch();
    auto yaw = mCamera->getYaw();
    auto zoom = mCamera->getZoom();

    float positionX{position.x()};
    float positionY{position.y()};
    float positionZ{position.z()};
    float scaleX{scale.x()};
    float scaleY{scale.y()};
    float scaleZ{scale.z()};

    checkRangeLimit(pitch, mRangeLimits.rangePitch);
    checkRangeLimit(yaw, mRangeLimits.rangeYaw);
    checkRangeLimit(zoom, mRangeLimits.rangeZoom);
    checkRangeLimit(positionX, mRangeLimits.rangeX);
    checkRangeLimit(positionY, mRangeLimits.rangeY);
    checkRangeLimit(positionZ, mRangeLimits.rangeZ);
    checkRangeLimit(scaleX, mRangeLimits.rangeScale);
    checkRangeLimit(scaleY, mRangeLimits.rangeScale);
    checkRangeLimit(scaleZ, mRangeLimits.rangeScale);

    mCamera->setPosition({positionX, positionY, positionZ});
    mCamera->setScale({scaleX, scaleY, scaleZ});
    mCamera->setPitch(pitch);
    mCamera->setYaw(yaw);
    mCamera->setZoom(zoom);
}

void Manipulator::checkRangeLimit(float& value, const Range& range)
{
    if (range.isEnabled)
    {
        if (value > range.max)
        {
            value = range.max;
        }
        if (value < range.min)
        {
            value = range.min;
        }
    }
}

void Manipulator::mousePressEvent(QMouseEvent* event)
{
    mPressedX = event->x();
    mPressedY = event->y();
    mIsRectZoomMode =
            (event->modifiers() == Qt::CTRL || mIsRectZoomMode) &&
            event->buttons() == Qt::LeftButton;
    mIsRectSelectionMode =
            (event->modifiers() == Qt::SHIFT || mIsRectSelectionMode) &&
            event->buttons() == Qt::LeftButton;
}

void Manipulator::mouseReleaseEvent(QMouseEvent*)
{
    if (mIsRectZoomMode)
    {
        auto x = std::min(mPressedX, mCurX);
        auto y = std::min(mPressedY, mCurY);
        zoom(QRect{x, y, abs(mCurX - mPressedX), abs(mCurY - mPressedY)});
    }

    mIsDragMode = false;
    mIsRectZoomMode = false;
    mIsRectSelectionMode = false;
}

void Manipulator::mouseMoveEvent(QMouseEvent* event)
{
    auto x  = event->x();
    auto y  = event->y();
    auto dx = x - mCurX;
    auto dy = y - mCurY;
    mCurX = x;
    mCurY = y;

    if (!mIsRectSelectionMode && !mIsRectZoomMode)
    {
        if (event->buttons() == Qt::LeftButton)
        {
            move(dx, dy);
            mIsDragMode = true;
        }

        if (event->buttons() == Qt::RightButton)
        {
            rotate(dx, dy);
            mIsDragMode = true;
        }

        if (event->buttons() == Qt::MiddleButton)
        {
            shift(dx, dy);
            mIsDragMode = true;
        }
    }
}

void Manipulator::wheelEvent(QWheelEvent* event)
{
    zoom(event->angleDelta().y());
}

void Manipulator::keyPressEvent(QKeyEvent *)
{}

void Manipulator::setCamera(Camera* camera)
{
    mCamera = camera;
}

StandartManipulator::StandartManipulator(const RangeLimits& rangeLimits) :
    Manipulator(rangeLimits)
{}

void StandartManipulator::focus(const Vec3& point)
{
    mCamera->focus(point);
}

void StandartManipulator::move(int dx, int dy)
{
    mCamera->move(Camera::MovementDirection::kRight, -dx);
    mCamera->move(Camera::MovementDirection::kXYForward, dy);
}

void StandartManipulator::shift(int dx, int dy)
{
    mCamera->move(Camera::MovementDirection::kRight, -dx);
    mCamera->move(Camera::MovementDirection::kUp, dy);
}

void StandartManipulator::rotate(int dx, int dy)
{
    mCamera->rotateAround(-dx, -dy);
}

void StandartManipulator::zoom(int delta)
{
    if (mCamera->isProjectionPerspective())
    {
        float dy = delta / mZoomSensitivity;
        if ((mCamera->getPosition().z() <= mRangeLimits.rangeZ.min &&
             dy < 0) ||
            mCamera->getPosition().z() > mRangeLimits.rangeZ.min)
        {
            mCamera->move(Camera::MovementDirection::kForward, dy);
        }
    }
    else
    {
        float dy = -delta / (static_cast<float>(mZoomSensitivity) * 10.0f);
        mCamera->scale({dy, dy, 0.0f});
    }
}

void StandartManipulator::zoom(const QRect& rect)
{
    auto screenH = mCamera->getViewPortSize().second;
    auto screenW = mCamera->getViewPortSize().first;

    auto rectCenter =
            mCamera->toWorldXYCoordinates({rect.center().x(), rect.center().y()});
    auto deltaPosition = rectCenter - mCamera->getLookPoint();

    if (mCamera->isProjectionPerspective())
    {
        auto rectLeft =
                mCamera->toWorldXYCoordinates({rect.left(), rect.center().y()});
        auto rectRight =
                mCamera->toWorldXYCoordinates({rect.right(), rect.center().y()});
        auto rectTop =
                mCamera->toWorldXYCoordinates({rect.center().x(), rect.top()});
        auto rectBottom =
                mCamera->toWorldXYCoordinates({rect.center().x(), rect.bottom()});
        auto rectVSize = (rectTop - rectBottom).length();
        auto rectHSize = (rectLeft - rectRight).length();

        auto screenLeft   = mCamera->toWorldXYCoordinates({0, screenH / 2});
        auto screenRight  = mCamera->toWorldXYCoordinates({screenW, screenH / 2});
        auto screenTop    = mCamera->toWorldXYCoordinates({screenW / 2, 0});
        auto screenBottom = mCamera->toWorldXYCoordinates({screenW / 2, screenH});
        auto screenHSize  = (screenLeft - screenRight).length();
        auto screenVSize  = (screenTop - screenBottom).length();

        float rectWorldSize;
        float screenWorldSize;
        if (rect.width() > rect.height())
        {
            rectWorldSize   = rectHSize;
            screenWorldSize = screenHSize;
        }
        else
        {
            rectWorldSize   = rectVSize;
            screenWorldSize = screenVSize;
        }

        float distance =
                rectWorldSize * mCamera->getLook().length() / screenWorldSize;
        float deltaDistance = mCamera->getLook().length() - distance;

        mCamera->moveFront(deltaDistance);
    }
    else
    {
        float k = rect.width() > rect.height()
                ? static_cast<float>(rect.width()) / static_cast<float>(screenW)
                : static_cast<float>(rect.height()) / static_cast<float>(screenH);
        auto scale = mCamera->getScale();
        mCamera->setScale({scale.x() * k, scale.y() * k, scale.z()});
    }

    mCamera->move(deltaPosition);
}

FixedManipulator::FixedManipulator(const RangeLimits& rangeLimits) :
    Manipulator(rangeLimits)
{}

void FixedManipulator::focus(const Vec3&)
{}

void FixedManipulator::move(int, int)
{}

void FixedManipulator::shift(int, int)
{}

void FixedManipulator::rotate(int, int)
{}

void FixedManipulator::zoom(int)
{}

void FixedManipulator::zoom(const QRect&)
{}

}
