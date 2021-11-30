#include "Item.h"
#include "Pipe.h"

namespace custom_scene
{

Item::Item(const std::shared_ptr<Mesh> mesh,
           std::shared_ptr<RenderParameters> renderParameters,
           std::shared_ptr<Material> material,
           std::shared_ptr<Texture> texture) :
    mMesh(mesh),
    mRenderParameters(renderParameters),
    mMaterial(material),
    mTexture(texture)
{
    mTransformation.isIdentity();
}

void Item::setMesh(const std::shared_ptr<Mesh> mesh)
{
    mMesh = mesh;
}

void Item::updateIndices(uint startIndex)
{
    mElementsStartIndex = startIndex;
    mElementsCount = mMesh->getElementsCount();
}

uint Item::getElementsStartIndex() const
{
    return mElementsStartIndex;
}

Material* Item::getMaterial() const
{
    return mMaterial.get();
}

Texture* Item::getTexture() const
{
    return mTexture.get();
}

uint Item::getElementsCount() const
{
    return mElementsCount;
}

Item::RenderParameters* Item::getRenderParameters() const
{
    return mRenderParameters.get();
}

const Mat4& Item::getTransformation() const
{
    return mTransformation;
}

const std::shared_ptr<Mesh> Item::getMesh() const
{
    return mMesh;
}

bool Item::isVisible() const
{
    return mIsVisible;
}

void Item::setIsVisible(bool isVisible)
{
    mIsVisible = isVisible;
}

void Item::setTransformation(const Mat4& transformation)
{
    mTransformation = transformation;
}

}
