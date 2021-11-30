#pragma once

#include "Common.h"

#include <map>
#include <QOpenGLShaderProgram>

namespace custom_scene
{

struct Material;
struct Light;

using ShaderSources = std::map<QOpenGLShader::ShaderType, QString>;

class Program : public QOpenGLShaderProgram
{
    Q_OBJECT

public:
    Program(const ShaderSources& sources, QObject* parent = nullptr);

    void initialize();

    void setView(const Vec3& position, const Mat4& projection, const Mat4& view);
    void setTransformation(const Mat4& transformation);
    void setLight(Light* light);
    void setMaterial(Material* material);
    void setAlfa(float alfa);

private:
    const ShaderSources& mSources;
    bool mIsInitialized{false};
};

}
