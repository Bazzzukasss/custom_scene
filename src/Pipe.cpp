#include "Pipe.h"
#include "Mesh.h"
#include "Program.h"

namespace custom_scene
{

Pipe::Pipe(std::shared_ptr<Program> program,
           const std::vector<custom_scene::Pipe::Attribute>& attributes) :
    mProgram(program),
    mAttributes(attributes)
{
}

void Pipe::initialize()
{
    initializeOpenGLFunctions();
    mProgram->initialize();
    create();
    bind();
    initializeAttributes();
    release();

    mIsInitialized = true;
}

void Pipe::create()
{
    mVAO.create();
    mVBO.create();
    mEBO.create();
}

bool Pipe::isAllocated() const
{
    return mIsAllocated;
}

bool Pipe::isInitialized() const
{
    return mIsInitialized;
}

void Pipe::bind()
{
    mProgram->bind();
    mVAO.bind();
    mVBO.bind();
    mEBO.bind();
}

void Pipe::release()
{
    mProgram->release();
    mVAO.release();
    mVBO.release();
    mEBO.release();
}

void Pipe::allocate(const Mesh& mesh)
{
    const auto& vertices = mesh.getVertices();
    const auto& indices = mesh.getIndices();

    bind();
    mVBO.allocate(vertices.data(), vertices.size() * sizeof(Vertex));
    mEBO.allocate(indices.data(), indices.size() * sizeof(uint));
    release();
    mIsAllocated = true;
}

void Pipe::initializeAttributes()
{
    GLuint index{0};

    for (const auto& attribute : mAttributes)
    {
        auto strideSize = attribute.stride * sizeof(GLfloat);
        auto shiftSize = attribute.shift * sizeof(GLfloat);

        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
                    index,
                    attribute.size,
                    GL_FLOAT,
                    GL_FALSE,
                    strideSize,
                    reinterpret_cast<void*>(shiftSize));
        index++;
    }
}

}
