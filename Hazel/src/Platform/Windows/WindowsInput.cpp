#include "hzpch.h"
#include "WindowsInput.h"
#include <GLFW/glfw3.h>
#include "Hazel/Core/Application.h"

namespace Hazel {
	Scope<Input> Input::s_Instance = CreateScope<WindowsInput>();

	bool WindowsInput::IsKeyPressedImpl(int keycode) {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button) {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	float WindowsInput::GetMouseXImpl() {
		auto[x, y] = GetMousePositionImpl();
		return (float)x;
	}

	float WindowsInput::GetMouseYImpl() {
		auto[x, y] = GetMousePositionImpl();
		return (float)y;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl() {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return std::pair<float, float>((float)xpos,(float)ypos);
	}
}