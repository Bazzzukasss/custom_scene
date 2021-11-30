#pragma once

#include "Common.h"

namespace custom_scene
{

class Projection;
class Manipulator;

/**
 * The Camera Class
 * @brief The class provides functionality to manipule the view port and the lookpoint
 */
class Camera
{
public:
    enum class MovementDirection
    {
        kForward,
        kRight,
        kUp,
        kXYForward
    };

    struct Parameters
    {
        Vec3 position;
        Vec3 up;
        float yaw;
        float pitch;
        float speed;
        float sensitivity;
        float zoom;
        Vec3 scale;
    };

    /**
     * @brief Constructor for Camera
     * @param position - the initial camera's position
     * @param up - the vector to the world's up
     * @param yaw - the rotation angle which is relative to the camera's vertical axis
     * @param pitch - the rotation angle which is relative to the camera's lateral axis
     * @param speed - the coefficient of the camera's movement
     * @param sensitivity - the coefficient of the camera's rotation
     * @param zoom - the value of the camera's zoom
     * @param projections - the vector of all cameras projections
     */
    Camera(const Parameters& parameters,
           const std::vector<std::shared_ptr<Projection>>& projections,
           std::shared_ptr<Manipulator> manipulator);

    /**
     * @brief Reset camera state to initial
     */
    void reset();

    /**
     * @brief Moves the camera according to the direction by the delta value
     * @param direction - the movement direction vector
     * @param delta - the movement value in screen coordinates
     */
    void move(MovementDirection direction, float delta = 1.0f);

    /**
     * @brief Moves the camera by the delta value
     * @param delta - the movement vector
     */
    void move(const Vec3& delta);

    /**
     * @brief Moves the camera by the delta value in front direction
     * @param delta - the movement value
     */
    void moveFront(float delta);

    /**
     * @brief Moves the camera by the delta value in front direction on XY plane
     * @param delta - the movement value
     */
    void moveXYFront(float delta);

    /**
     * @brief Moves the camera by the delta value in up direction
     * @param delta - the movement value
     */
    void moveUp(float delta);

    /**
     * @brief Moves the camera by the delta value in right direction
     * @param delta - the movement value
     */
    void moveRight(float delta);

    /**
     * @brief Rotate the camera around its mounting point
     * @param delta_x - the mouse cursor x coordinate delta
     * @param delta_y - the mouse cursor y coordinate delta
     * @todo possibly, either find better solution or remove it alltogether
     */
    void rotate(int deltaX, int deltaY);

    /**
     * @brief Rotate the camera around its mounting point
     * @param delta_yaw - yaw angle delta
     * @param delta_pitch - pitch angle delta
     * @todo possibly, either find better solution or remove it alltogether
     */
    void rotate(float deltaYaw, float deltaPitch);

    /**
     * @brief Rotate the camera around z axis
     * @param delta_x - the mouse cursor x coordinate delta
     * @param delta_y - the mouse cursor y coordinate delta
     * @todo possibly, either find better solution or remove it alltogether
     */
    void rotateAround(int deltaX, int deltaY);

    /**
     * @brief Rotate the camera around z axis
     * @param delta_yaw - yaw angle delta
     * @param delta_pitch - pitch angle delta
     * @todo possibly, either find better solution or remove it alltogether
     */
    void rotateAround(float deltaYaw, float deltaPitch);

    /**
     * @brief Zoom the camera's view
     * @param delta - the zoom delta value
     */
    void zoom(float delta);

    /**
     * @brief Scale the camera's orthographic projection
     * @param delta_scale - the delta scale by x, y and z axis delta value
     */
    void scale(const Vec3& deltaScale);

    /**
     * @brief Focus the camera's view
     * @param point - the point camra should be focused on
     */
    void focus(const Vec3& point);

    /**
     * @brief Switch the bitween cameras projections
     */
    void switchProjection();

    /**
     * @brief Projects the screen's point to the OXY plane
     * @param screen_x - the screen's point x coordinate
     * @param screen_y - the screen's point y coordinate
     * @param world_z - the z coordinate of the OXY plane
     * @return Point in world coordinates
     */
    Vec3 toWorldXYCoordinates(const Point2i& point, float worldZ = 0.0f);

    /**
     * @brief Projects the screen's points to the OXY plane
     * @param screen_points - the container with screen's points
     * @param world_z - the z coordinate of the OXY plane
     * @return Point's in  world coordinates
     */
    std::vector<Point3f> toWorldXYCoordinates(
            const std::vector<Point2i>& screenPoints,
            float worldZ = 0.0f);

    /**
     * @brief Projects the screen's point to the camera plane
     * @param screen_x - the screen's point x coordinate
     * @param screen_y - the screen's point y coordinate
     * @param distance - the relative value of camera plane (0 coresponds to near plane, 1 coresponds to far plane)
     * @return Point in world coordinates
     */
    Vec3 toWorldCoordinates(const Point2i& screenPoint, float distance);

    /**
     * @brief Projects the screen's point to the camera plane
     * @param screen_points - the container with screen's points
     * @param distance - the relative value of camera plane (0 coresponds to near plane, 1 coresponds to far plane)
     * @return Points in world coordinates
     */
    std::vector<Point3f> toWorldCoordinates(
            const std::vector<Point2i>& screenPoints,
            float distance);

    /** setters */
    void setPitch(float pitch);
    void setYaw(float yaw);
    void setZoom(float zoom);
    void setScale(const Vec3& scale);
    void setPosition(const Vec3& position);
    void setSeneitivity(float sensetivity);
    void setSpeed(float speed);
    void setProjectionIndex(int index);
    void setViewPort(int view_width, int view_height);
    bool isProjectionPerspective() const;

    /** getters */
    Mat4 getTransformation() const;
    const Mat4& getView() const;
    const Mat4& getProjection() const;
    float getYaw() const;
    float getPitch() const;
    float getZoom() const;
    const Vec3& getScale() const;
    const Vec3& getPosition() const;
    const Vec3& getRight() const;
    const Vec3& getUp() const;
    const Vec3& getFront() const;
    const Vec3& getLook() const;
    const Vec3& getLookPoint() const;
    const std::pair<int, int>& getViewPortSize() const;
    std::shared_ptr<Manipulator> getManipulator() const;

private:
    void update(bool use_angles = true, bool update_look = false);
    void calculateViewMatrix();

private:
    float mYaw;
    float mPitch;
    float mSpeed;
    float mSensitivity;
    float mZoom;
    Vec3 mScale;
    Vec3 mPosition;
    Vec3 mFront;
    Vec3 mXYFront;
    Vec3 mUp;
    Vec3 mRight;
    Vec3 mWorldUp;
    Vec3 mLookPoint;
    Vec3 mLook;
    Parameters mDefaultParameters;
    std::vector<std::shared_ptr<Projection>> mProjections;
    std::shared_ptr<Manipulator> mManipulator;
    std::shared_ptr<Projection> mCurrentProjection;
    std::pair<int, int> mViewPortSize;
    Mat4 mViewMatrix;
    uint mCurrentProjectionIndex;
};

}
