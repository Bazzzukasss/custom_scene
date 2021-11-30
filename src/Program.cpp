#include "Program.h"
#include "Utils.h"
#include "Material.h"
#include "Light.h"

#include <QFileInfo>

namespace custom_scene {

Program::Program(const ShaderSources& sources, QObject* parent) :
    QOpenGLShaderProgram(parent),
    mSources(sources)
{
}

void Program::initialize()
{
    if (mIsInitialized)
    {
        return;
    }

    auto isCreated = create();

    if (isCreated)
    {
        for (const auto& [type, source] : mSources)
        {
            if (QFileInfo(source).isFile())
            {
                addShaderFromSourceFile(type, source);
            }
            else
            {
                addShaderFromSourceCode(type, source);
            }
        }

        auto isLinked = link();
        mIsInitialized = isCreated && isLinked;
    }
}

void Program::setView(const Vec3& position,
                      const Mat4& projection,
                      const Mat4& view)
{
    setUniformValue("viewPos", position);
    setUniformValue("projection", projection);
    setUniformValue("view", view);
}

void Program::setTransformation(const Mat4& transformation)
{
    setUniformValue("normal", transformation.normalMatrix());
    setUniformValue("model", transformation);
}

void Program::setLight(Light* light)
{
    setUniformValue("light.direction", light->direction);
    setUniformValue("light.ambient", utils::toVec3(light->ambient));
    setUniformValue("light.diffuse", utils::toVec3(light->diffuse));
    setUniformValue("light.specular", utils::toVec3(light->specular));
}

void Program::setMaterial(Material* material)
{
    setUniformValue("material.shininess", material->shininess);

    if (auto standartMaterial = dynamic_cast<StandartMaterial*>(material))
    {
        setUniformValue("material.ambient", standartMaterial->ambient);
        setUniformValue("material.diffuse", standartMaterial->diffuse);
        setUniformValue("material.specular", standartMaterial->specular);
    }
}

void Program::setAlfa(float alfa)
{
    setUniformValue("alfa", alfa);
}

}
