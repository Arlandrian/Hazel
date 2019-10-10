#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{

}

void Sandbox2D::OnAttach() {


	m_SquareVA = Hazel::VertexArray::Create();

	float squareVertices[3 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	Hazel::Ref<Hazel::VertexBuffer> squareVB;
	squareVB = Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

	Hazel::BufferLayout sqrBufferLayut = {
		{ Hazel::ShaderDataType::Float3, "a_Position" }
	};
	squareVB->SetLayout(sqrBufferLayut);
	m_SquareVA->AddVertexBuffer(squareVB);

	// Index Buffer
	uint32_t squareIndices[3 * 2] = {
		0,1,2,2,3,0
	};
	Hazel::Ref<Hazel::IndexBuffer> squareIB;
	squareIB = Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	m_SquareVA->SetIndexBuffer(squareIB);

	m_FlatColorShader = Hazel::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnUpdate(Hazel::Timestep ts) {
	// Update
	m_CameraController.OnUpdate(ts);

	Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Hazel::RenderCommand::Clear();

	Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	auto shader = std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader);
	shader->Bind();
	shader->UploadUniformFloat4("u_Color", m_SquareColor);


	for (int y = -10; y < 10; y++) {

		for (int x = -10; x < 10; x++) {
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);

			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

			Hazel::Renderer::Submit(shader, m_SquareVA, transform);
		}
	}


	Hazel::Renderer::EndScene();

}

void Sandbox2D::OnImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& event) {
	m_CameraController.OnEvent(event);

}
