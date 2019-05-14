#include "hzpch.h"
#include "WindowsWindow.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"

#include "glad/glad.h"


namespace Hazel {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* desc) {
		HZ_CORE_ERROR("GLFW ERROR ({0}): {1}", error, desc);
	}

	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) {
		Init(props);
	}

	WindowsWindow::~WindowsWindow() {
		Shutdown();
	}
	
	void WindowsWindow::Init(const WindowProps& props) {
		m_Data.Title = props.Title;
		m_Data.Height = props.Height;
		m_Data.Width = props.Width;

		HZ_CORE_INFO("Creating window {0} {1} {2}", props.Title, props.Width, props.Height);

		if( !s_GLFWInitialized ) {
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			HZ_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status,"Failed to initialized Glad!");

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		//Set GLFW Callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window,int width,int height) {
			//Above we set window user pointer, now we can get it from window itself
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width,height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});


		/**@param[in] window The window that received the event.
		*  @param[in] key The[keyboard key](@ref keys) that was pressed or released.
		*  @param[in] scancode The system - specific scancode of the key.
		*  @param[in] action `GLFW_PRESS`, `GLFW_RELEASE` or `GLFW_REPEAT`.
		*  @param[in] mods Bit field describing which[modifier keys](@ref mods) were
		*  held down.*/
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window,int keycode,int scancode,int action,int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch( action ) {
				case GLFW_PRESS:
				{
					KeyPressedEvent event(keycode,0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(keycode);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(keycode, 1);
					data.EventCallback(event);
					break;
				}
			}
		});
		
		/**@param[in] window The window that received the event.
		*  @param[in] button The[mouse button](@ref buttons) that was pressed or
		*   released.
		*  @param[in] action One of `GLFW_PRESS` or `GLFW_RELEASE`.
		*  @param[in] mods Bit field describing which[modifier keys](@ref mods) were
		*   held down.*/
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch( action ) {
				case GLFW_PRESS:
				{ 
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{ 
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window,double offsetX,double offsetY) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)offsetX,(float)offsetY);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xpos, (float)ypos);
			data.EventCallback(event);
		});
	}

	void WindowsWindow::Shutdown() {
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled) {
		if( enabled ) {
			glfwSwapInterval(1);
		} else {
			glfwSwapInterval(0);
		}

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const {
		return m_Data.VSync;
	}
}