#pragma once
#include "Test.h"

#include <memory>
#include <string>
#include "../Renderer.h"

#include "../IndexBuffer.h"
#include "../Shader.h"
#include "../Texture.h"
#include "../VertexBuffer.h"
#include "../VertexArray.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{
    class TestRotatingTexture :
        public Test
    {
    private:
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<VertexBuffer> m_VBuffer;
        std::unique_ptr<VertexArray> m_VArray;
        std::unique_ptr<IndexBuffer> m_IBuffer;

        Texture m_Texture;
        float m_RotationAngle;
        glm::vec3 m_Rotation;

        glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);

        glm::mat4 mvp = glm::mat4(1.0f);
    public:
        TestRotatingTexture();
        ~TestRotatingTexture();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    };
}

