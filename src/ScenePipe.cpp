#include "ScenePipe.h"
#include "Item.h"
#include "Mesh.h"
#include "Program.h"
#include "Camera.h"
#include "Light.h"

namespace custom_scene
{

ScenePipe::ScenePipe(std::shared_ptr<Program> program,
                     const std::vector<Pipe::Attribute>& attributes,
                     const Items& items) :
    Pipe(program, attributes)
{
    addItems(items);
}

void ScenePipe::addItem(std::shared_ptr<Item> item)
{
    mItems.push_back(item);
    mIsAllocated = false;
}

void ScenePipe::addItems(const Items &items)
{
    for(auto& item : items)
    {
        mItems.push_back(item);
    }
    mIsAllocated = false;
}

void ScenePipe::removeItem(std::shared_ptr<Item> item)
{
    mItems.remove(item);
    mIsAllocated = false;
}

void ScenePipe::clear()
{
    mItems.clear();
    mIsAllocated = false;
}

void ScenePipe::realocate()
{
    Mesh mesh;
    for (auto& item : mItems)
    {
        item->updateIndices(mesh.getIndices().size());
        mesh += *item->getMesh();
        allocate(mesh);
    }
}

void ScenePipe::render(std::shared_ptr<Camera> camera,
                       const ScenePipe::Lights& lights,
                       const ScenePipe::Textures& textures)
{
    if (!mIsInitialized)
    {
        return;
    }

    for (auto item : mItems)
    {
        bind();
        mProgram->setView(camera->getPosition(),
                          camera->getProjection(),
                          camera->getView());
        mProgram->setLight(lights.front().get());
        mProgram->setMaterial(item->getMaterial());
        mProgram->setTransformation(item->getTransformation());

        const auto& renderParameters = item->getRenderParameters();

        glLineWidth(renderParameters->lineWidth);
        for (const auto& param : renderParameters->enableAttributes)
        {
            glEnable(param);
        }
        for (const auto& param : renderParameters->disableAttributes)
        {
            glDisable(param);
        }

        glDrawElements(renderParameters->renderMode,
                       item->getElementsCount(),
                       GL_UNSIGNED_INT,
                       (void*)item->getElementsStartIndex());
        /*
            ->pipe.program
            ->scene.camera
            ->scene.light
            ->item.material
            ->item.texture
            ->pipe.VAO
                ->VAP
                ->VBO
                ->EBO
            ->item.transformation
            ->item.renderParameters
            ->draw
        */
    }
}

const ScenePipe::Items& ScenePipe::getItems() const
{
    return mItems;
}


} //custom_scene
