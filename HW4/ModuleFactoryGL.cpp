#include "ModuleFactoryGL.h"

#include "NativeWindowGLFW.h"
#include "RendererGL.h"
#include "InputGLFW.h"

#include <GLFW\glfw3.h>
#include <cassert>

namespace hw4
{

	ModuleFactoryGL::ModuleFactoryGL()
	{
		assert(glfwInit() == GLFW_TRUE);
	}

	ModuleFactoryGL::~ModuleFactoryGL()
	{
		glfwTerminate();
	}

	std::shared_ptr<NativeWindow> ModuleFactoryGL::CreateNativeWindow(int width, int height, const char * title, bool fullscreen)
	{
		return std::make_shared<NativeWindowGLFW>(width, height, title, fullscreen);
	}

	std::shared_ptr<Renderer> ModuleFactoryGL::CreateRenderer(std::weak_ptr<NativeWindow> window)
	{
		return std::make_shared<RendererGL>();
	}

	std::shared_ptr<Input> ModuleFactoryGL::CreateInput(std::weak_ptr<NativeWindow> window)
	{
		std::shared_ptr<NativeWindow> win = window.lock();
		NativeWindowGLFW* ptr = dynamic_cast<NativeWindowGLFW*>(win.get());
		assert(nullptr != ptr);
		return std::make_shared<InputGLFW>(ptr);
	}

}

