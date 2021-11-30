#include "View.h"
#include "Scene.h"
#include "ScenePipe.h"
#include "Item.h"
#include "Manipulator.h"

#include <QMouseEvent>

namespace custom_scene
{

View::View(QWidget* parent) :
    QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSamples(16);
    setFormat(format);
}

void View::setScene(std::shared_ptr<Scene> scene)
{
    mScene = scene;

    connect(mScene.get(), &Scene::changed,
            this, &View::updateScene);
}

void View::setCamera(std::shared_ptr<Camera> camera)
{
    mCamera = camera;
}

void View::updateScene()
{
    for(const auto& pipe : mScene->getPipes())
    {
        if (!pipe->isInitialized())
        {
            pipe->initialize();
        }

        if (!pipe->isAllocated())
        {
            pipe->realocate();
        }
    }
    update();
}

void View::initializeGL()
{
    initializeOpenGLFunctions();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    connect(context(), &QOpenGLContext::aboutToBeDestroyed,
            this, &View::cleanup);
    
    updateScene();
}

void View::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    mCamera->setViewPort(w, h);
}

void View::paintGL()
{
#ifdef SHOW_DEBUG
    using namespace std::chrono;
    auto t1 = system_clock::now().time_since_epoch();
#endif

    clear();
    for(const auto& pipe : mScene->getPipes())
    {
        pipe->render(mCamera,
                     mScene->getLights(),
                     mScene->getTextures());
    }

#ifdef SHOW_DEBUG
    auto t2 = system_clock::now().time_since_epoch();
    qDebug() << "GLSceneView::paintGL duration:" << duration_cast<microseconds>(t2 - t1).count() << "mks";
#endif
}

void View::clear()
{
    glClearColor(mBackgroundColor.redF(),
                 mBackgroundColor.greenF(),
                 mBackgroundColor.blueF(),
                 mBackgroundColor.alphaF());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void View::mousePressEvent(QMouseEvent* event)
{
    mCamera->getManipulator()->mousePressEvent(event);

    mCurX = mPressedX = event->x();
    mCurY = mPressedY = event->y();
}

void View::mouseReleaseEvent(QMouseEvent* event)
{
    if (mCamera->getManipulator()->isRectSelectionMode())
    {
        //pickItems(mPressedX, mPressedY, mCurX, mCurY);
    }
    else
    {
        if ((mPressedX == mCurX) && (mPressedY == mCurY))
        {
            //pickItems(mCurX, mCurY, mCurX, mCurY);
        }
    }

    mCamera->getManipulator()->mouseReleaseEvent(event);
    updateCursorShape();
}

void View::mouseMoveEvent(QMouseEvent* event)
{
    mCurX = event->x();
    mCurY = event->y();

    //pickItems(mCurX, mCurY, mCurX, mCurY, 1, false);

    mCamera->getManipulator()->mouseMoveEvent(event);
    updateCursorShape();
}

void View::keyPressEvent(QKeyEvent *event)
{
    mCamera->getManipulator()->keyPressEvent(event);
    updateCursorShape();
}

void View::wheelEvent(QWheelEvent *event)
{
    mCamera->getManipulator()->wheelEvent(event);
}

void View::cleanup()
{
    makeCurrent();
    doneCurrent();
}

void View::updateCursorShape()
{
    auto manipulator = mCamera->getManipulator();

    if (manipulator->isDragMode())
    {
        this->setCursor(Qt::CursorShape::OpenHandCursor);
    }
    else if (manipulator->isRectSelectionMode() ||
             manipulator->isRectZoomMode())
    {
        this->setCursor(Qt::CursorShape::CrossCursor);
    }
    else
    {
        this->setCursor(Qt::CursorShape::ArrowCursor);
    }
}

}
