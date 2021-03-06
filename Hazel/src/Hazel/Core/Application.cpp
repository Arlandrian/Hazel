#include "hzpch.h"
#include "Application.h"
#include "Hazel/Renderer/Renderer.h"

#include "GLFW/glfw3.h"
namespace Hazel {

// Note: this will return a function like this => x(this); 
#define BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application() { 
		HZ_CORE_ASSERT(!s_Instance, "Application already exist!");
		s_Instance = this;
	
		m_Window = Scope<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		//m_Window->SetVSync(false);

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

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
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));
		

		for( auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*--it)->OnEvent(e);
			if( e.Handled )
				break;
		}
		
	}

	void Application::Run() {

		while( m_Running ) {

			float time = (float)glfwGetTime(); //Platform::GetTime
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;
			
			if (!m_Minimized) {
				for (Layer* layer : m_LayerStack) {
					layer->OnUpdate(timestep);
				}
			}
			
			m_ImGuiLayer->Begin();
			for( Layer* layer : m_LayerStack ) {
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& event) {
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;

		Renderer::OnWindowResized(e.GetWidth(), e.GetHeight());
		return false;
	}
}

