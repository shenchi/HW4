#include "RendererGL.h"

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <cassert>

namespace
{
#define STRINGIFY(x) #x

	const char* vert_shader_source = STRINGIFY(
	in vec3 position;
	
	uniform mat4 matModel;
	uniform mat4 matView;
	uniform mat4 matProj;

	void main()
	{
		gl_Position = matProj * matView * matModel * vec4(position, 1.0);
	}

	);

	const char* pixel_shader_source = STRINGIFY(

	void main()
	{
		gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	}

	);


	void ShowShaderLog(GLuint shader)
	{
		GLint len = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
		if (0 == len)
			return;

		GLchar* log = new GLchar[len];

		glGetShaderInfoLog(shader, len, nullptr, log);

		delete[] log;
	}
}

namespace hw4
{

	RendererGL::RendererGL()
	{
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vert_shader_source, nullptr);
		glCompileShader(vertex_shader);

		ShowShaderLog(vertex_shader);

		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &pixel_shader_source, nullptr);
		glCompileShader(fragment_shader);

		ShowShaderLog(fragment_shader);

		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		glValidateProgram(program);
		GLint status = GL_FALSE;
		glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
		assert(status == GL_TRUE);

		position_location = glGetAttribLocation(program, "position");

		mat_model_location = glGetUniformLocation(program, "matModel");
		mat_view_location = glGetUniformLocation(program, "matView");
		mat_proj_location = glGetUniformLocation(program, "matProj");

		glUseProgram(program);

		glm::mat4 identity(1.0f);
		glUniformMatrix4fv(mat_model_location, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&identity));
		glUniformMatrix4fv(mat_view_location, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&identity));
		glUniformMatrix4fv(mat_proj_location, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&identity));
	}

	RendererGL::~RendererGL()
	{
	}

	VertexBufferHandle RendererGL::CreateVertexBuffer(size_t size, const void* data)
	{
		GLuint vao, vbo;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(position_location);
		glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

		vertex_buffers.push_back(VertexBuffer{ true, vao, vbo });

		return VertexBufferHandle{ static_cast<uint32>(vertex_buffers.size() - 1) };
	}

	void RendererGL::UpdateVertexBuffer(VertexBufferHandle vertex_buffer, size_t size, const void * data)
	{
		if (!IsValid(vertex_buffer))
			return;

		glBindVertexArray(vertex_buffers[vertex_buffer].vao);
		glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_DRAW); // orphaning the buffer
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	void RendererGL::SetVertexBuffer(VertexBufferHandle vertex_buffer)
	{
		if (!IsValid(vertex_buffer))
			return;

		current_vertex_buffer = vertex_buffer;
	}

	IndexBufferHandle RendererGL::CreateIndexBuffer(size_t size, const uint32 * data)
	{
		return IndexBufferHandle();
	}

	void RendererGL::UpdateIndexBuffer(IndexBufferHandle index_buffer, size_t size, const uint32 * data)
	{
	}

	void RendererGL::SetIndexBuffer(IndexBufferHandle index_buffer)
	{
	}

	void RendererGL::SetModelMatrix(const float* matrix)
	{
		glUniformMatrix4fv(mat_model_location, 1, GL_FALSE, matrix);
	}

	void RendererGL::SetViewMatrix(const float* matrix)
	{
		glUniformMatrix4fv(mat_view_location, 1, GL_FALSE, matrix);
	}

	void RendererGL::SetProjectionMatrix(const float* matrix)
	{
		glUniformMatrix4fv(mat_proj_location, 1, GL_FALSE, matrix);
	}

	void RendererGL::Draw(uint32 start, uint32 count)
	{
		if (!IsValid(current_vertex_buffer))
			return;

		glBindVertexArray(vertex_buffers[current_vertex_buffer].vao);
		glDrawArrays(GL_TRIANGLES, start, count);
	}

	void RendererGL::DrawIndexed(uint32 start, uint32 count)
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

	bool RendererGL::IsValid(VertexBufferHandle handle)
	{
		return handle && handle.id < vertex_buffers.size() && vertex_buffers[handle.id].in_use;
	}

}
