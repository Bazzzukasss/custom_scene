#pragma once

#include "Common.h"

namespace custom_scene
{

struct Material
{
    virtual ~Material() = default;

    float shininess;
};

struct StandartMaterial : Material {
    Vec3 ambient;
    Vec3 diffuse;
    Vec3 specular;
};

struct TexturedMaterial : Material {
    Texture diffuse;
    Texture specular;
};

struct BumpedMaterial : Material {
    Texture diffuse;
    Texture normal;
    Texture displacement;
};

}
