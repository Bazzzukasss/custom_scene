#pragma once

#include "Camera.h"

class QMouseEvent;
class QKeyEvent;
class QWheelEvent;
class QtMath;

namespace custom_scene
{

class Manipulator
{
    const int DefaultZoomSensitivity{120};

public:
    struct Range
    {
        bool isEnabled;
        float min;
        float max;
    };

    struct RangeLimits
    {
        Range rangeX;
        Range rangeY;
        Range rangeZ;
        Range rangeZoom;
        Range rangeYaw;
        Range rangePitch;
        Range rangeScale;
    };

    Manipulator(const RangeLimits& rangeLimits);

    /** setters */
    void setCamera(Camera *camera);
    void setZoomSensitivity(int sensitivity);
    void setRectZoomMode(bool isZoomMode);
    void setRectSelectionMode(bool isSelectionMode);

    /** getters */
    int getZoomSensitivity() const;
    bool isRectZoomMode() const;
    bool isRectSelectionMode() const;
    bool isDragMode() const;

    /** methods for camera manipulation */
    void reset();
    void checkRangeLimits();
    void checkRangeLimit(float& value, const Range& range);

    virtual void focus(const Vec3& point);
    virtual void shift(int dx, int dy) = 0;
    virtual void move(int dx, int dy) = 0;
    virtual void rotate(int dx, int dy) = 0;
    virtual void zoom(int delta) = 0;
    virtual void zoom(const QRect& rect) = 0;

    virtual void mousePressEvent(QMouseEvent*);
    virtual void mouseReleaseEvent(QMouseEvent*);
    virtual void mouseMoveEvent(QMouseEvent*);
    virtual void wheelEvent(QWheelEvent*);
    virtual void keyPressEvent(QKeyEvent*);

protected:
    int mCurX{0};
    int mCurY{0};
    int mPressedX{0};
    int mPressedY{0};
    int mZoomSensitivity{DefaultZoomSensitivity};
    bool mIsRectSelectionMode{false};
    bool mIsRectZoomMode{false};
    bool mIsDragMode{false};
    Camera* mCamera;
    RangeLimits mRangeLimits;
};

class StandartManipulator : public Manipulator
{
public:
    StandartManipulator(const RangeLimits& rangeLimits);

    void focus(const Vec3& point) override;
    void move(int dx, int dy) override;
    void shift(int dx, int dy) override;
    void rotate(int dx, int dy) override;
    void zoom(int delta) override;
    void zoom(const QRect& rect) override;
};

class FixedManipulator : public Manipulator
{
public:
    FixedManipulator(const RangeLimits& rangeLimits);

    void focus(const Vec3&) override;
    void move(int, int) override;
    void shift(int, int) override;
    void rotate(int, int) override;
    void zoom(int) override;
    void zoom(const QRect&) override;
};

}
