#include <Windows.h>
#include <memory>
#include <cassert>

#include <chrono>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "ModuleFactoryGL.h"
#include "NativeWindow.h"
#include "Renderer.h"
#include "Input.h"

namespace
{
	typedef std::chrono::high_resolution_clock timer;

	const float vertices[] = { 
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
	};

	float *dynamic_vertices = nullptr;

	hw4::VertexBufferHandle buffer;
}


void Init()
{
	dynamic_vertices = new float[9];
	for (int i = 0; i < 9; i++)
		dynamic_vertices[i] = vertices[i];
	
	buffer = hw4::Renderer::instance()->CreateVertexBuffer(sizeof(vertices), reinterpret_cast<const void*>(dynamic_vertices));
}

void Update(float delta_time)
{
	static float t = 0.0f;
	t += delta_time;
	dynamic_vertices[0] = sin(t) * 0.5f;

	hw4::Renderer::instance()->UpdateVertexBuffer(buffer, sizeof(vertices), reinterpret_cast<const void*>(dynamic_vertices));

	//glm::mat4 model_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));
	//hw4::Renderer::instance()->SetModelMatrix(reinterpret_cast<float*>(&model_mat));
}

void Render()
{
	hw4::Renderer::instance()->SetVertexBuffer(buffer);
	hw4::Renderer::instance()->Draw(0, 3);
}

void Release()
{
	delete[] dynamic_vertices;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	std::shared_ptr<hw4::ModuleFactory> factory = std::make_shared<hw4::ModuleFactoryGL>();

	auto win = factory->CreateNativeWindow(800, 600, "HW4");
	//auto win = factory->CreateNativeWindow(1920, 1080, "HW4", true);
	auto renderer = factory->CreateRenderer(win);
	auto input = factory->CreateInput(win);

	Init();

	float interval = 0.016f;
	float elapsed = 0.0f;
	auto lastTime = timer::now();

	while (win->UpdateEvent())
	{
		auto now = timer::now();
		float deltaTime = std::chrono::duration<float>(now - lastTime).count();


		if (elapsed + deltaTime < interval)
		{
			continue;
		}

		lastTime = now;
		elapsed = deltaTime + elapsed - interval;

		if (input->IsButtonPressed(hw4::KEY_ESCAPE))
			break;

		Update(deltaTime);

		renderer->ClearRenderTarget(0.0f, 0.0f, 0.0f, 1.0f);
		renderer->ClearDepthStencil(0.0f, 0);

		Render();

		win->Present();
	}

	Release();

	return 0;
}
