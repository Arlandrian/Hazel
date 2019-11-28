#pragma once
#include "Hazel.h"
class Sandbox2D : public Hazel::Layer {
public:

	Sandbox2D();
	virtual ~Sandbox2D() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Hazel::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Hazel::Event& event) override;
private:
	Hazel::OrthographicCameraController m_CameraController;

	// Temporary
	std::shared_ptr<Hazel::Shader> m_FlatColorShader;
	glm::vec4 m_SquareColor = { 0.8f, 0.2f, 0.3f,1.0f };
	std::shared_ptr<Hazel::VertexArray> m_SquareVA;
	Hazel::Ref<Hazel::Texture2D> m_CheckerboardTexture;

};