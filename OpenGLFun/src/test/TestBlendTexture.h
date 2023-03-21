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
    class TestBlendTexture :
        public Test
    {
    private:
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<VertexBuffer> m_VBuffer;
        std::unique_ptr<VertexArray> m_VArray;
        std::unique_ptr<IndexBuffer> m_IBuffer;

        Texture m_Texture;

        float m_ColorIncrement, m_Color, m_Alpha, m_timeDirection, m_Time;
    public:
        TestBlendTexture();
        ~TestBlendTexture();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    };
}

