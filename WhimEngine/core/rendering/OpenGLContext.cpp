#include "OpenGLContext.hpp"
#include "../logger/Logger.hpp"

whim::OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
{
	this->m_windowHandle = windowHandle;
	whim::Logger::log("Created OpenGL context");
}
