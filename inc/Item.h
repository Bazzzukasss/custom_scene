#pragma once

#include "Common.h"
#include "Geometry.h"
#include "Mesh.h"

namespace custom_scene
{

struct Material;
class Pipe;

class Item
{
public:
    struct RenderParameters
    {
        GLenum renderMode;
        float alfa;
        float lineWidth;
        std::vector<GLenum> enableAttributes;
        std::vector<GLenum> disableAttributes;
    };

    Item(const std::shared_ptr<Mesh> mesh,
         std::shared_ptr<RenderParameters> renderParameters,
         std::shared_ptr<Material> material,
         std::shared_ptr<Texture> texture);

    void setMesh(const std::shared_ptr<Mesh> mesh);
    const std::shared_ptr<Mesh> getMesh() const;

    void updateIndices(uint startIndex);
    uint getElementsStartIndex() const;
    uint getElementsCount() const;

    void setTransformation(const Mat4& transformation);
    const Mat4& getTransformation() const;

    bool isVisible() const;
    void setIsVisible(bool isVisible);

    RenderParameters* getRenderParameters() const;
    Material* getMaterial() const;
    Texture* getTexture() const;

private:
    std::shared_ptr<Mesh> mMesh;
    std::shared_ptr<RenderParameters> mRenderParameters;
    std::shared_ptr<Material> mMaterial;
    std::shared_ptr<Texture> mTexture;    
    Mat4 mTransformation;
    bool mIsVisible{true};
    uint mElementsStartIndex{0};
    uint mElementsCount{0};
};

}
