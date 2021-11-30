#pragma once

#include "Common.h"

#include <map>
#include <vector>
#include <memory>
#include <QObject>

namespace custom_scene
{

class Item;
class ScenePipe;
class Light;

class Scene  : public QObject
{
    Q_OBJECT

    using Pipes = std::list<std::shared_ptr<ScenePipe>>;
    using Lights = std::list<std::shared_ptr<Light>>;
    using Textures = std::list<std::shared_ptr<Texture>>;

public:
    Scene(const Pipes& pipes, QObject* parent = nullptr);

    void addPipe(std::shared_ptr<ScenePipe> pipe, bool isNotify = true);
    void addPipes(const Pipes& pipes, bool isNotify = true);
    void removePipe(std::shared_ptr<ScenePipe> pipe, bool isNotify = true);
    void removePipes(bool isNotify = true);

    void addLight(std::shared_ptr<Light> light, bool isNotify = true);
    void addLights(const Lights& lights, bool isNotify = true);
    void removeLight(std::shared_ptr<Light> light, bool isNotify = true);
    void removeLights(bool isNotify = true);

    void addTexture(std::shared_ptr<Texture> texture, bool isNotify = true);
    void addTextures(const Textures& texture, bool isNotify = true);
    void removeTexture(std::shared_ptr<Texture> texture, bool isNotify = true);
    void removeTextures(bool isNotify = true);

    void clear(bool isNotify = true);
    void update();

    const Pipes& getPipes() const;
    const Lights& getLights() const;
    const Textures& getTextures() const;

signals:
    void changed();

private:
    Pipes mPipes;
    Lights mLights;
    Textures mTextures;
};

}
