#pragma once

#include "CommonTypes.h"

namespace hw4
{
	class Renderer
	{
		static Renderer* _instance;
	public:
		inline static Renderer* instance() { return _instance; }

		Renderer();

		virtual ~Renderer();

	public:

		virtual void ClearRenderTarget(float r, float g, float b, float a) = 0;

		virtual void ClearDepth(float depth) = 0;

		virtual void ClearDepthStencil(float depth, uint8 stencil) = 0;
	};
}
