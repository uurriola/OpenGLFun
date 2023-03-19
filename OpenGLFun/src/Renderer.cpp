#include "Renderer.h"

#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}


bool GLLogCall(const char* function, const char* file, int line)
{
    while (unsigned int error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ": " << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& vArray, const IndexBuffer& iBuffer, const Shader& shader) const
{
    shader.Bind();
    vArray.Bind();
    iBuffer.Bind();

    // Will draw the bound buffer
    GLCall(glDrawElements(GL_TRIANGLES, iBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));
}
