#include <Windows.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <memory>
#include <cassert>

#include "ModuleFactoryGL.h"
#include "NativeWindow.h"
#include "Renderer.h"

namespace
{
#define STRINGIFY(x) #x

	const char* vert_shader_source = STRINGIFY(

	in vec3 position;
	
	void main()
	{
		gl_Position = vec4(position, 1.0);
	}

	);

	const char* pixel_shader_source = STRINGIFY(

	void main()
	{
		gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	}

	);

	const float vertices[] = { 
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
	};

	GLuint vertex_shader, fragment_shader, program, position;

	GLuint vao, vbo;
}

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

void Init()
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

	position = glGetAttribLocation(program, "position");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), reinterpret_cast<const void*>(vertices), GL_STATIC_DRAW);

	glEnableVertexAttribArray(position);
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
}

void Render()
{
	glUseProgram(program);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	std::shared_ptr<hw4::ModuleFactory> factory = std::make_shared<hw4::ModuleFactoryGL>();

	auto win = factory->CreateNativeWindow(800, 600, "HW4");
	//auto win = factory->CreateNativeWindow(1920, 1080, "HW4", true);
	auto renderer = factory->CreateRenderer(win);

	Init();

	while (win->UpdateEvent())
	{
		renderer->ClearRenderTarget(0.3f, 0.3f, 0.3f, 1.0f);
		renderer->ClearDepthStencil(0.0f, 0);

		Render();

		win->Present();
	}

	return 0;
}
