#include <Hazel.h>
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include "Hazel/Renderer/Shader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtc/type_ptr.hpp>


//#include "Hazel/Renderer/Texture.h"

class ExampleLayer : public Hazel::Layer {
public:
	ExampleLayer() : Layer("Example") ,
		m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		
		// VertexArray
		m_VertexArray.reset(Hazel::VertexArray::Create());

		// Vertex Buffer

		float vertices[3 * 7] = {
			-0.5f,-0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f,-0.5f, 0.0f, 0.1f, 0.3f, 0.8f, 1.0f,
			 0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.3f, 1.0f
		};

		std::shared_ptr<Hazel::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));


		Hazel::BufferLayout layout = {
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);


		// Index Buffer
		uint32_t indices[3] = {
			0,1,2
		};
		std::shared_ptr<Hazel::IndexBuffer> indexBuffer;

		indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);
		/////////////////////////////////////////////////
		/////////////////////////////////////////////////
		/////////////////////////////////////////////////

		m_SqrVertexArray.reset(Hazel::VertexArray::Create());

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Hazel::Ref<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		Hazel::BufferLayout sqrBufferLayut = {
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float2, "a_TexCoord" }
		};
		squareVB->SetLayout(sqrBufferLayut);
		m_SqrVertexArray->AddVertexBuffer(squareVB);

		// Index Buffer
		uint32_t squareIndices[3 * 2] = {
			0,1,2,2,3,0
		};
		Hazel::Ref<Hazel::IndexBuffer> squareIB;
		squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SqrVertexArray->SetIndexBuffer(squareIB);


		// Shader
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location=0) in vec3 a_Position;
			layout(location=1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;
			void main(){
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
			
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location=0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;

			void main(){

				color = vec4(v_Position*0.7+0.5, 1.0);
				color = v_Color;

			}
			
		)";

		m_Shader = Hazel::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

		// Shader
		std::string flatShaderVertexSrc = R"(
			#version 330 core
			
			layout(location=0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			void main(){
				v_Position = a_Position;
				gl_Position = u_ViewProjection *u_Transform* vec4(a_Position, 1.0);
			}
			
		)";

		std::string flatShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location=0) out vec4 color;
			in vec3 v_Position;
			uniform vec4 u_Color;

			void main(){

				color = u_Color;

			}
			
		)";

		m_FlatColorShader = Hazel::Shader::Create("Flat Color Shader", flatShaderVertexSrc, flatShaderFragmentSrc);

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");


		m_Texture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");//ChernoLogo.png
		m_ChernoLogoTexture = Hazel::Texture2D::Create("assets/textures/ChernoLogo.png");
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Hazel::Timestep ts) override {
		//Renderer::BeginScene(camera,lights,environment);

		if( Hazel::Input::IsKeyPressed(HZ_KEY_UP) ) {
			m_CameraPosition.y += m_CameraSpeed * ts;
		}else if( Hazel::Input::IsKeyPressed(HZ_KEY_DOWN) ) {
			m_CameraPosition.y -= m_CameraSpeed * ts;

		}
		if( Hazel::Input::IsKeyPressed(HZ_KEY_LEFT) ) {
			m_CameraPosition.x -= m_CameraSpeed * ts;

		} else if( Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT) ) {
			m_CameraPosition.x += m_CameraSpeed * ts;

		}

		if( Hazel::Input::IsKeyPressed(HZ_KEY_A) ) {
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		} else if( Hazel::Input::IsKeyPressed(HZ_KEY_D) ) {
			m_CameraRotation += m_CameraRotationSpeed * ts;

		}

		Hazel::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
		Hazel::RenderCommand::Clear();

		m_Camera.setPosition(m_CameraPosition);

		m_Camera.setRotation(m_CameraRotation);


		Hazel::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		auto shader = std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader);
		shader->Bind();
		shader->UploadUniformFloat4("u_Color", m_SquareColor);

		//Hazel::MaterialRef material = new Hazel::Material(m_FlatColorShader);
		//Hazel::MaterialInstanceRef mi = new Hazel::MaterialInstance(material);
		
		//mi->Set("u_Color",redColor);
		//mi->SetTexture("u_AlbedoMap",texture);
		//squareMesh->SetMaterial(mi);

		for( int y = -10; y < 10; y++ ) {

			for( int x = -10; x < 10; x++ ) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);

				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

				Hazel::Renderer::Submit(shader, m_SqrVertexArray, transform);
			}
		}
		// Triangle
		//Hazel::Renderer::Submit(m_Shader, m_VertexArray);

		m_Texture->Bind();
		//std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);

		auto textureShader = m_ShaderLibrary.Get("Texture");

		Hazel::Renderer::Submit(textureShader, m_SqrVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));


		m_ChernoLogoTexture->Bind(0);
		//std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 1);


		Hazel::Renderer::Submit(textureShader, m_SqrVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));


		Hazel::Renderer::EndScene();

	}

	void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

		ImGui::End();


	}

	void OnEvent(Hazel::Event& event) override {
		Hazel::EventDispatcher dispatcher(event);

		dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));

	}

	bool OnKeyPressedEvent(Hazel::KeyPressedEvent event) {

		return false;
	}
private:
	Hazel::ShaderLibrary m_ShaderLibrary;
	std::shared_ptr<Hazel::Shader> m_Shader;
	std::shared_ptr<Hazel::VertexArray> m_VertexArray;
					
	std::shared_ptr<Hazel::Shader> m_FlatColorShader;
	std::shared_ptr<Hazel::VertexArray> m_SqrVertexArray;

	Hazel::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;

	glm::vec4 m_SquareColor = {0.8f, 0.2f, 0.3f,1.0f};

	float m_CameraSpeed = 5.0f;
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 45.0f;

	Hazel::Ref<Hazel::Texture2D> m_Texture,m_ChernoLogoTexture;
};

class Sandbox: public Hazel::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};


Hazel::Application* Hazel::CreateApplication() {
	return new Sandbox();
}