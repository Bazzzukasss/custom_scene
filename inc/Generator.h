#pragma once

#include "Geometry.h"
#include "Figures.h"

#define GENERATE custom_scene::Generator::generate


namespace custom_scene
{

/**
 * The Generator Class
 * @brief The class provides functionality for generation of various figures' geometry
 */
class Generator
{
 public:
    /**
     * @brief The set of functions for geometry generation
     * @param figure - the data structure containing the figure's geometry
     * @return the container with points
     */
    static Geometry generate(const figures::Cube& figure);
    static Geometry generate(const figures::Quad& figure);
};

}  // namespace gl_scene

