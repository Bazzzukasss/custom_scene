#pragma once

#include "Common.h"

namespace custom_scene
{

struct Light {
    bool isVisible{true};
    Vec3 position;
    Vec3 direction;
    Color ambient;
    Color diffuse;
    Color specular;
};

}
