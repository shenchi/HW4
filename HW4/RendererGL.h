#pragma once

#include "Renderer.h"

namespace hw4
{

	class RendererGL : public Renderer
	{
	public:
		RendererGL();
		~RendererGL();

		virtual void ClearRenderTarget(float r, float g, float b, float a);

		virtual void ClearDepth(float depth);

		virtual void ClearDepthStencil(float depth, uint8 stencil);
	};

}
