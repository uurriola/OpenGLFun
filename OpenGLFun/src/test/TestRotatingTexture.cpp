#include "TestRotatingTexture.h"

#include "../VertexBufferLayout.h"
#include "imgui/imgui.h"

namespace test
{
	TestRotatingTexture::TestRotatingTexture() : m_RotationAngle(0.05f), m_Rotation(glm::vec3(0.0f, 1.0, 0.0f)), m_Texture("res/textures/wood.png")
	{
		float positions[16]{
			-0.5f, -0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f
		};
		unsigned int indices[6]{
			0, 1, 2,
			2, 3, 0
		};
		m_VBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
		m_IBuffer = std::make_unique<IndexBuffer>(indices, 6);

		std::string shaderFilePath = "res/shaders/Texture.shader";
		m_Shader = std::make_unique<Shader>(shaderFilePath);
		m_Texture.Bind(0);

		VertexBufferLayout vLayout;
		vLayout.Push<float>(2);
		vLayout.Push<float>(2);
		m_VArray = std::make_unique<VertexArray>();
		m_VArray->AddBuffer(*m_VBuffer, vLayout);

		m_Shader->Bind();
		m_Shader->SetUniform1i("u_Texture", 0);
	}

	TestRotatingTexture::~TestRotatingTexture()
	{
	}

	void TestRotatingTexture::OnUpdate(float deltaTime)
	{
		m_Shader->Bind();
		model = glm::rotate(model, m_RotationAngle, m_Rotation);
		mvp = proj * view * model;
		m_Shader->SetUniformMat4f("u_MVP", mvp);
	}

	void TestRotatingTexture::OnRender()
	{
		Renderer renderer;
		renderer.Draw(*m_VArray, *m_IBuffer, *m_Shader);
	}

	void TestRotatingTexture::OnImGuiRender()
	{
		ImGui::SliderFloat("Rotation speed", &m_RotationAngle, -0.1f, 0.1f);
		ImGui::SliderFloat3("Rotation", &m_Rotation.x, 0.0f, 1.0f);
	}
}
