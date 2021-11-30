#pragma once

#include "Common.h"

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>

namespace custom_scene
{

class Scene;
class Camera;

/**
 * The GLSceneView Class
 * @brief The class is the widget for the scene visualisation
 */
class View : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

    const Color DefaultBackgroundColor{0, 0, 0};

 public:
    View(QWidget* parent = nullptr);
    ~View() override;

    /** setters */
    void setScene(std::shared_ptr<Scene> scene);
    void setCamera(std::shared_ptr<Camera> camera);    

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    void clear();
    void cleanup();
    void updateCursorShape();
    void updateScene();

private:
    int mCurX{0};
    int mCurY{0};
    int mPressedX{0};
    int mPressedY{0};
    Color mBackgroundColor{DefaultBackgroundColor};
    std::shared_ptr<Camera> mCamera;
    std::shared_ptr<Scene> mScene;
};

}
