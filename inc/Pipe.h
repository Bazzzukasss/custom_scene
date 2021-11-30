#pragma once

#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <memory>

namespace custom_scene
{

class Program;
class Mesh;

class Pipe : public QOpenGLFunctions
{
public:
    struct Attribute
    {
        GLint size;
        GLsizei stride;
        GLint shift;
    };

    Pipe(std::shared_ptr<Program> program,
         const std::vector<Attribute>& attributes);
    virtual ~Pipe() = default;

    void initialize();
    void bind();
    void release();
    void allocate(const Mesh& mesh);

    bool isInitialized() const;
    bool isAllocated() const;

protected:
    std::shared_ptr<Program> mProgram;
    bool mIsAllocated{false};
    bool mIsInitialized{false};

private:
    void initializeAttributes();
    void create();

private:
    std::vector<Attribute> mAttributes;
    QOpenGLVertexArrayObject mVAO;
    QOpenGLBuffer mVBO;
    QOpenGLBuffer mEBO;
};

}
