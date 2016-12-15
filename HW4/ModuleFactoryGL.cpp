#include "ModuleFactoryGL.h"

#include "NativeWindowGL.h"
#include "RendererGL.h"

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
		return std::make_shared<NativeWindowGL>(width, height, title, fullscreen);
	}

	std::shared_ptr<Renderer> ModuleFactoryGL::CreateRenderer(std::weak_ptr<NativeWindow> window)
	{
		return std::make_shared<RendererGL>();
	}

}

