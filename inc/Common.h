#pragma once

#include <array>
#include <vector>
#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>
#include <QColor>
#include <QOpenGLTexture>

namespace custom_scene
{

using Point3f = std::array<float, 3>;
using Point2f = std::array<float, 2>;
using Point2i = std::array<int, 2>;
using Vec3 = QVector3D;
using Vec2 = QVector2D;
using Color = QColor;
using Texture = QOpenGLTexture;
using Mat4 = QMatrix4x4;

}
