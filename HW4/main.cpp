#include <Windows.h>

#include <memory>

#include <cstdio>
#include <cassert>

#include <chrono>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\noise.hpp>

#include "ModuleFactoryGL.h"
#include "NativeWindow.h"
#include "Renderer.h"
#include "Input.h"
#include "Camera.h"
#include "Terrain.h"

namespace
{
	typedef std::chrono::high_resolution_clock timer;

	hw4::VertexBufferHandle vertex_buffer[9];
	hw4::IndexBufferHandle index_buffer;
	hw4::Camera camera;

	hw4::Terrain* pTerrain;
}


void Init()
{
	for (int i = 0; i < 9; i++)
	{
		vertex_buffer[i] = hw4::Renderer::instance()->CreateVertexBuffer(pTerrain->VertexBufferSize(), 0);
	}

	index_buffer = hw4::Renderer::instance()->CreateIndexBuffer(pTerrain->IndexBufferSize(), pTerrain->IndexBuffer());

	camera.SetPosition(glm::vec3(0.0f, 1.0f, 0.0f));

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

	for (int i = 0; i < 9; i++)
	{
		if (pTerrain->IsUpdateFlagSet(i))
		{
			const float* vertices = pTerrain->VertexBuffer(i);
			hw4::Renderer::instance()->UpdateVertexBuffer(vertex_buffer[i], 
				pTerrain->VertexBufferSize(), 
				reinterpret_cast<const void*>(vertices));
			pTerrain->ClearUpdateFlag(i);
			break;
		}
	}

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
		pTerrain->SetPlayerPosition(camera_pos.x, camera_pos.y, camera_pos.z);
	}


	glm::mat4 mat_view = camera.GetViewMatrix();
	hw4::Renderer::instance()->SetViewMatrix(reinterpret_cast<float*>(&mat_view));
}

void Render()
{
	hw4::Renderer::instance()->SetIndexBuffer(index_buffer);

	for (int i = 0; i < 9; i++)
	{
		hw4::Renderer::instance()->SetVertexBuffer(vertex_buffer[i]);
		hw4::Renderer::instance()->DrawIndexed(0, pTerrain->IndicesCount());
	}
}

void Release()
{
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	std::shared_ptr<hw4::ModuleFactory> factory = std::make_shared<hw4::ModuleFactoryGL>();

	auto win = factory->CreateNativeWindow(800, 600, "HW4");
	//auto win = factory->CreateNativeWindow(1920, 1080, "HW4", true);
	auto renderer = factory->CreateRenderer(win);
	auto input = factory->CreateInput(win);

	//auto terrain = std::make_shared<hw4::Terrain>(0.1f, 100);
	auto terrain = std::make_shared<hw4::Terrain>(0.1f, 100);
	pTerrain = terrain.get();
	pTerrain->SetSamplingFunction([](float x, float y) { 
		glm::vec2 pos(x, y);
		return glm::perlin(pos) * 0.7f + glm::perlin(pos * 10.0f) * 0.3f; 
	});
	pTerrain->SetPlayerPosition(0.0f, 1.0f, 0.0f);

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

		{
			char buf[64];
			auto pos = camera.GetPosition();
			sprintf(buf, "HW4 (%.6f, %.6f, %.6f)", pos.x, pos.y, pos.z);
			win->SetTitle(buf);
		}

		renderer->ClearRenderTarget(1.0f, 1.0f, 1.0f, 1.0f);
		renderer->ClearDepthStencil(1.0f, 0);

		Render();

		win->Present();
	}

	Release();

	return 0;
}
