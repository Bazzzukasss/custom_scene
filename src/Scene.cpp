#include "Scene.h"
#include "Item.h"
#include "Pipe.h"

namespace custom_scene
{

Scene::Scene(const Pipes& pipes, QObject* parent) :
    QObject(parent)
{
    addPipes(pipes);
}

void Scene::addPipe(std::shared_ptr<ScenePipe> pipe, bool isNotify)
{
    mPipes.push_back(pipe);
    if (isNotify)
    {
        emit changed();
    }
}

void Scene::addPipes(const Scene::Pipes& pipes, bool isNotify)
{
    for (auto& pipe : pipes)
    {
        mPipes.push_back(pipe);
    }

    if (isNotify)
    {
        emit changed();
    }
}

void Scene::removePipe(std::shared_ptr<ScenePipe> pipe, bool isNotify)
{
    mPipes.remove(pipe);

    if (isNotify)
    {
        emit changed();
    }
}

void Scene::removePipes(bool isNotify)
{
    mPipes.clear();

    if (isNotify)
    {
        emit changed();
    }
}

void Scene::addLight(std::shared_ptr<Light> light, bool isNotify)
{
    mLights.push_back(light);

    if (isNotify)
    {
        emit changed();
    }
}

void Scene::addLights(const Scene::Lights& lights, bool isNotify)
{
    for (auto& light : lights)
    {
        mLights.push_back(light);
    }

    if (isNotify)
    {
        emit changed();
    }
}

void Scene::removeLight(std::shared_ptr<Light> light, bool isNotify)
{
    mLights.remove(light);

    if (isNotify)
    {
        emit changed();
    }
}

void Scene::removeLights(bool isNotify)
{
    mLights.clear();

    if (isNotify)
    {
        emit changed();
    }
}

void Scene::addTexture(std::shared_ptr<Texture> texture, bool isNotify)
{
    mTextures.push_back(texture);

    if (isNotify)
    {
        emit changed();
    }
}

void Scene::removeTexture(std::shared_ptr<Texture> texture, bool isNotify)
{
    mTextures.remove(texture);

    if (isNotify)
    {
        emit changed();
    }
}

void Scene::removeTextures(bool isNotify)
{
    mTextures.clear();

    if (isNotify)
    {
        emit changed();
    }
}

void Scene::clear(bool isNotify)
{
    removePipes(false);
    removeLights(false);
    removeTextures(false);

    if (isNotify)
    {
        emit changed();
    }
}

void Scene::update()
{
    emit changed();
}

const Scene::Pipes& Scene::getPipes() const
{
    return mPipes;
}

const Scene::Lights &Scene::getLights() const
{
    return mLights;
}

const Scene::Textures& Scene::getTextures() const
{
    return mTextures;
}

}
