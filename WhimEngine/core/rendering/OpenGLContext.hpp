#pragma once

struct GLFWwindow;

namespace whim {
	class OpenGLContext {
	public:
		OpenGLContext(GLFWwindow* windowHandle);
	private:
		GLFWwindow* m_windowHandle;
	};

}