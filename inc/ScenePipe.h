#pragma once

#include "Pipe.h"
#include "Common.h"

namespace custom_scene
{

class Item;
class Camera;
class Light;

class ScenePipe : public Pipe
{
    using Items = std::list<std::shared_ptr<Item>>;
    using Lights = std::list<std::shared_ptr<Light>>;
    using Textures = std::list<std::shared_ptr<Texture>>;

public:
    ScenePipe(std::shared_ptr<Program> program,
              const std::vector<Attribute>& attributes,
              const Items &items = {});

    void addItem(std::shared_ptr<Item> item);
    void addItems(const Items& items);
    void removeItem(std::shared_ptr<Item> item);
    void clear();
    void realocate();

    virtual void render(std::shared_ptr<Camera> camera,
                        const Lights& lights,
                        const Textures& textures);

    const Items& getItems() const;

private:
    Items mItems;
};

} // custom_scene
