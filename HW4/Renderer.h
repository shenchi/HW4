#pragma once

#include "CommonTypes.h"
#include "Handle.h"

namespace hw4
{

	HANDLE_DECLARE(VertexBufferHandle);
	HANDLE_DECLARE(IndexBufferHandle);

	class Renderer
	{
	private:
		static Renderer* _instance;
	public:
		inline static Renderer* instance() { return _instance; }

		Renderer();

		virtual ~Renderer();

	public:

		virtual VertexBufferHandle CreateVertexBuffer(size_t size = 0, const void* data = nullptr) = 0;

		virtual void UpdateVertexBuffer(VertexBufferHandle vertex_buffer, size_t size, const void* data) = 0;

		virtual void SetVertexBuffer(VertexBufferHandle vertex_buffer) = 0;

		virtual IndexBufferHandle CreateIndexBuffer(size_t size = 0, const uint32* data = nullptr) = 0;

		virtual void UpdateIndexBuffer(IndexBufferHandle index_buffer, size_t size, const uint32* data) = 0;

		virtual void SetIndexBuffer(IndexBufferHandle index_buffer) = 0;

		virtual void SetModelMatrix(const float* matrix) = 0;

		virtual void SetViewMatrix(const float* matrix) = 0;

		virtual void SetProjectionMatrix(const float* matrix) = 0;

		virtual void Draw(uint32 start, uint32 count) = 0;

		virtual void DrawIndexed(uint32 start, uint32 count) = 0;

		virtual void ClearRenderTarget(float r, float g, float b, float a) = 0;

		virtual void ClearDepth(float depth) = 0;

		virtual void ClearDepthStencil(float depth, uint8 stencil) = 0;
	};
}
