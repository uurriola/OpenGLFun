#include "TestBlendTexture.h"

#include "../VertexBufferLayout.h"
#include "imgui/imgui.h"

namespace test
{
	TestBlendTexture::TestBlendTexture() : m_Texture("res/textures/wood.png"), m_ColorIncrement(0.05f), m_Color(0.1f), m_Alpha(0.5f), m_timeDirection(1.0f), m_Time(5.0f)
	{
		float positions[16]{
			-1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 1.0f
		};
		unsigned int indices[6]{
			0, 1, 2,
			2, 3, 0
		};
		m_VBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
		m_IBuffer = std::make_unique<IndexBuffer>(indices, 6);

		std::string shaderFilePath = "res/shaders/WaveTextureBlend.shader";
		m_Shader = std::make_unique<Shader>(shaderFilePath);
		m_Texture.Bind(0);

		VertexBufferLayout vLayout;
		vLayout.Push<float>(2);
		vLayout.Push<float>(2);
		m_VArray = std::make_unique<VertexArray>();
		m_VArray->AddBuffer(*m_VBuffer, vLayout);

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f));
		m_Shader->SetUniform1i("u_Texture", 0);
	}

	TestBlendTexture::~TestBlendTexture()
	{
	}

	void TestBlendTexture::OnUpdate(float deltaTime)
	{
		if (m_Time > 5.5 || m_Time < 4.5)
			m_timeDirection = -m_timeDirection;
		m_Time += m_timeDirection * deltaTime;

		m_Color += m_ColorIncrement;
		if (m_Color > 1.0f || m_Color < 0.0f)
			m_ColorIncrement = -m_ColorIncrement;
		
		m_Shader->Bind();
		m_Shader->SetUniform1f("u_Color", m_Color);
		m_Shader->SetUniform1f("u_time", m_Time);
		m_Shader->SetUniform1f("u_alpha", m_Alpha);
	}

	void TestBlendTexture::OnRender()
	{
		Renderer renderer;
		renderer.Draw(*m_VArray, *m_IBuffer, *m_Shader);
	}

	void TestBlendTexture::OnImGuiRender()
	{
		ImGui::SliderFloat("Blend", &m_Alpha, 0.0f, 1.0f);
	}
}
