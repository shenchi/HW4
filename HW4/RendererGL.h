#pragma once

#include "Renderer.h"

#include <vector>

namespace hw4
{

	class RendererGL : public Renderer
	{
	public:
		RendererGL();
		~RendererGL();

		virtual VertexBufferHandle CreateVertexBuffer(size_t size = 0, const void* data = nullptr);

		virtual void UpdateVertexBuffer(VertexBufferHandle vertex_buffer, size_t size, const void* data);

		virtual void SetVertexBuffer(VertexBufferHandle vertex_buffer);

		virtual IndexBufferHandle CreateIndexBuffer(size_t size = 0, const uint32* data = nullptr);

		virtual void UpdateIndexBuffer(IndexBufferHandle index_buffer, size_t size, const uint32* data);

		virtual void SetIndexBuffer(IndexBufferHandle index_buffer);

		virtual void SetModelMatrix(const float* matrix);

		virtual void SetViewMatrix(const float* matrix);

		virtual void SetProjectionMatrix(const float* matrix);

		virtual void Draw(uint32 start, uint32 count);

		virtual void DrawIndexed(uint32 start, uint32 count);

		virtual void ClearRenderTarget(float r, float g, float b, float a);

		virtual void ClearDepth(float depth);

		virtual void ClearDepthStencil(float depth, uint8 stencil);

	private:
		bool IsValid(VertexBufferHandle handle);
		bool IsValid(IndexBufferHandle handle);

	private:

		uint32 vertex_shader, fragment_shader, program;
		uint32 position_location;
		uint32 normal_location;
		uint32 mat_model_location;
		uint32 mat_view_location;
		uint32 mat_proj_location;
		
		struct VertexBuffer
		{
			bool	in_use;
			uint32	vao, vbo;
		};

		std::vector<VertexBuffer> vertex_buffers;

		VertexBufferHandle current_vertex_buffer;

		struct IndexBuffer
		{
			bool	in_use;
			uint32	ibo;
		};

		std::vector<IndexBuffer> index_buffers;

		IndexBufferHandle current_index_buffer;
	};

}
