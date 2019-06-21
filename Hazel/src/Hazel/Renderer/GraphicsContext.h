#pragma once
#include <GLFW/glfw3.h>



namespace Hazel {

	class GraphicsContext {
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

	};

}