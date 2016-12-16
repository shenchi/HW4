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
#include "Camera.h"

namespace
{
	typedef std::chrono::high_resolution_clock timer;

	/*const float vertices[] = {
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
	};*/

	const float vertices[] = 
	{
		-0.5f, 0.0f, 0.5f,
		-0.5f, 0.0f, -0.5f,
		0.5f, 0.0f, 0.5f,

		0.5f, 0.0f, 0.5f,
		-0.5f, 0.0f, -0.5f,
		0.5f, 0.0f, -0.5f,
	};

	float *dynamic_vertices = nullptr;

	hw4::VertexBufferHandle buffer;
	hw4::Camera camera;
}


void Init()
{
	int float_count = sizeof(vertices) / sizeof(float);
	dynamic_vertices = new float[float_count];
	for (int i = 0; i < float_count; i++)
		dynamic_vertices[i] = vertices[i];

	buffer = hw4::Renderer::instance()->CreateVertexBuffer(sizeof(vertices), reinterpret_cast<const void*>(dynamic_vertices));

	camera.SetPosition(glm::vec3(0.0f, 1.0f, 1.0f));

}

void ResetProjection(int width, int height)
{
	float aspect = height == 0 ? 1.0f : (float)width / height;
	camera.SetPerspectiveProjection(3.1415926f / 3, aspect, 0.01f, 100.0f);
	hw4::Renderer::instance()->SetProjectionMatrix(reinterpret_cast<const float*>(&(camera.GetProjectionMatrix())));
}

void Update(float delta_time)
{
	static float t = 0.0f;
	t += delta_time;

	//dynamic_vertices[0] = sin(t) * 0.5f;
	//hw4::Renderer::instance()->UpdateVertexBuffer(buffer, sizeof(vertices), reinterpret_cast<const void*>(dynamic_vertices));

	{
		hw4::Input* input = hw4::Input::instance();

		glm::vec3 camera_rot = camera.GetRotation();

		static double lastMouseX = input->GetMousePositionX();
		static double lastMouseY = input->GetMousePositionY();

		double curMouseX = input->GetMousePositionX();
		double curMouseY = input->GetMousePositionY();

		camera_rot.y += static_cast<float>((lastMouseX - curMouseX) * 0.001);
		camera_rot.x += static_cast<float>((lastMouseY - curMouseY) * 0.001);

		camera_rot.x = glm::clamp(camera_rot.x, -3.1415926f / 2, 3.1415926f / 2);

		lastMouseX = curMouseX;
		lastMouseY = curMouseY;

		camera.SetRotation(camera_rot);

		glm::vec3 camera_pos = camera.GetPosition();

		glm::mat3 rot = camera.GetRotationMatrix();
		glm::vec3 forward = rot * glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 right = rot * glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 up = rot * glm::vec3(0.0f, 1.0f, 0.0f);

		if (input->IsButtonPressed(hw4::KEY_W))
		{
			camera_pos += forward * delta_time;
		}
		else if (input->IsButtonPressed(hw4::KEY_S))
		{
			camera_pos -= forward * delta_time;
		}

		if (input->IsButtonPressed(hw4::KEY_A))
		{
			camera_pos -= right * delta_time;
		}
		else if (input->IsButtonPressed(hw4::KEY_D))
		{
			camera_pos += right * delta_time;
		}

		if (input->IsButtonPressed(hw4::KEY_SPACE))
		{
			camera_pos += up * delta_time;
		}
		else if (input->IsButtonPressed(hw4::KEY_LEFT_SHIFT))
		{
			camera_pos -= up * delta_time;
		}

		camera.SetPosition(camera_pos);
	}


	glm::mat4 mat_view = camera.GetViewMatrix();
	hw4::Renderer::instance()->SetViewMatrix(reinterpret_cast<float*>(&mat_view));
}

void Render()
{
	hw4::Renderer::instance()->SetVertexBuffer(buffer);
	hw4::Renderer::instance()->Draw(0, 6);
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

	ResetProjection(win->Width(), win->Height());
	win->SetResizeCallback(ResetProjection);

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
