#include "hzpch.h"
#include "Application.h"

//#include "GLFW/glfw3.h"
#include <glad/glad.h>

#include "Hazel/Input.h"
#include "glm/glm.hpp"

namespace Hazel {

// Note: this will return a function like this => x(this); 
#define BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application() { 
		HZ_CORE_ASSERT(!s_Instance, "Application already exist!");
		s_Instance = this;
	
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

	}


	Application::~Application() { }

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		
		//HZ_CORE_TRACE("{0}",e);

		for( auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*--it)->OnEvent(e);
			if( e.Handled )
				break;
		}

		
	}

	void Application::Run() {

		while( m_Running ) {
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for( Layer* layer : m_LayerStack ) {
				layer->OnUpdate();
			}
			auto[x, y] = Input::GetMousePosition();
			//HZ_CORE_TRACE("{0}, {1}", x, y);

			//Input::IsKeyPressed();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& event) {
		m_Running = false;
		return true;
	}
}

