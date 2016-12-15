#include "RendererGL.h"

#include <GLFW\glfw3.h>

namespace hw4
{

	RendererGL::RendererGL()
	{
	}

	RendererGL::~RendererGL()
	{
	}

	void RendererGL::ClearRenderTarget(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void RendererGL::ClearDepth(float depth)
	{
		glClearDepth(depth);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void RendererGL::ClearDepthStencil(float depth, uint8 stencil)
	{
		glClearDepth(depth);
		glClearStencil(stencil);
		glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

}
