#pragma warning(disable: 4996)
#include "Terrain.h"

#include <Windows.h>
#include <string>
#include <cstdlib>

#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <array>

#include <unordered_set>
#include <cassert>
#include <glm\glm.hpp>

namespace hw4
{
	struct Task
	{
		int x;
		int y;
		int bufferId;
	};

	struct Terrain::ThreadData 
	{
		std::queue<Task> task_queue;
		std::mutex task_queue_mutex;

		std::atomic_bool work_thread_running;
		std::thread work_thread;

		std::array<std::atomic_bool, 9> update_flags;
	};

	hw4::Terrain::Terrain(float gridSize, int blockSize)
		:
		grid_size(gridSize),
		block_size(blockSize),
		block_width(gridSize * blockSize),
		data{ nullptr },
		initialized(false),
		thread_data(new ThreadData())
	{
		sampler = [](float x, float y) { return 0.0f; };

		block_data_size = (block_size + 1) * (block_size + 1) * (3 + 3);
		for (int i = 0; i < 9; ++i)
		{
			data[i] = new float[block_data_size];
		}

		indices_count = block_size * block_size * 6;
		indices = new uint32[indices_count];

		for (int i = 0, y = 0; y < block_size; ++y)
		{
			for (int x = 0; x < block_size; ++x)
			{
				int start_index = y * (block_size + 1) + x;
				indices[i++] = start_index;
				indices[i++] = start_index + (block_size + 1);
				indices[i++] = start_index + 1;
				indices[i++] = start_index + 1;
				indices[i++] = start_index + (block_size + 1);
				indices[i++] = start_index + (block_size + 2);
			}
		}

		thread_data->work_thread_running = true;
		thread_data->work_thread = std::thread(&Terrain::WorkingThread, this);

		for (int i = 0; i < 9; i++)
		{
			buffer_id[i] = -1;
			thread_data->update_flags[i] = false;
		}
	}

	Terrain::~Terrain()
	{
		thread_data->work_thread_running = false;
		thread_data->work_thread.join();

		for (int i = 0; i < 9; ++i)
		{
			if (nullptr != data[9])
			{
				delete[] data[i];
				data[i] = nullptr;
			}
		}

		delete thread_data;
	}

	void Terrain::SetPlayerPosition(float x, float y, float z)
	{
		int blockX = to_block_coord(x);
		int blockY = to_block_coord(z);

		if (blockX == current_block_x && blockY == current_block_y && initialized)
			return;

		{
			std::string str = "coord: ";
			char buf[20];
			itoa(blockX, buf, 10);
			str += buf;
			str += ", ";
			itoa(blockY, buf, 10);
			str += buf;
			str += '\n';
			OutputDebugStringA(str.c_str());
		}

		initialized = true;

		std::unordered_set<int> bufferIds;
		int new_buffer_id[9];
		for (int i = 0; i < 9; i++)
			new_buffer_id[i] = -1;

		for (int dy = -1; dy <= 1; ++dy)
		{
			for (int dx = -1; dx <= 1; ++dx)
			{
				int nx = blockX + dx;
				int ny = blockY + dy;
				int new_idx = block_coord_to_index(dx, dy);

				if (nx >= current_block_x - 1 && nx <= current_block_x + 1 &&
					ny >= current_block_y - 1 && ny <= current_block_y + 1)
				{
					int odx = nx - current_block_x;
					int ody = ny - current_block_y;
					int old_idx = block_coord_to_index(odx, ody);
					int bufferId = new_buffer_id[new_idx] = buffer_id[old_idx];
					if (bufferId != -1)
					{
						bufferIds.insert(bufferId);
					}
				}
			}
		}

		for (int dy = -1; dy <= 1; ++dy)
		{
			for (int dx = -1; dx <= 1; ++dx)
			{
				int new_idx = block_coord_to_index(dx, dy);

				if (new_buffer_id[new_idx] != -1)
					continue;

				for (int i = 0; i < 9; i++)
				{
					if (bufferIds.find(i) == bufferIds.end())
					{
						bufferIds.insert(i);
						new_buffer_id[new_idx] = i;
						break;
					}
				}

				assert(new_buffer_id[new_idx] != -1);

				{
					std::lock_guard<std::mutex> guard(thread_data->task_queue_mutex);
					thread_data->task_queue.push(Task{ blockX + dx, blockY + dy, new_buffer_id[new_idx] });
					std::string str = "new: ";
					char buf[20]; 
					itoa(blockX + dx, buf, 10);
					str += buf;
					str += ", ";
					itoa(blockY + dy, buf, 10);
					str += buf;
					str += '\n';
					OutputDebugStringA(str.c_str());
				}
			}
		}

		for (int i = 0; i < 9; i++)
			buffer_id[i] = new_buffer_id[i];
		current_block_x = blockX;
		current_block_y = blockY;
	}

	bool Terrain::IsUpdateFlagSet(int idx) const
	{
		return thread_data->update_flags[idx];
	}

	void Terrain::ClearUpdateFlag(int idx)
	{
		thread_data->update_flags[idx] = false;
	}

	void Terrain::WorkingThread()
	{
		bool empty = false;
		Task task;

		while (thread_data->work_thread_running)
		{
			empty = false;
			{
				std::lock_guard<std::mutex> guard(thread_data->task_queue_mutex);
				empty = thread_data->task_queue.empty();
				if (!empty)
				{
					task = thread_data->task_queue.front();
					thread_data->task_queue.pop();
				}
			}

			std::this_thread::yield();

			if (!empty)
			{
				GenerateBlock(data[task.bufferId], task.x, task.y);
				thread_data->update_flags[task.bufferId] = true;
			}
		}
	}

	void Terrain::GenerateBlock(float* buffer, int blockX, int blockY)
	{
		float start_x = (blockX - 0.5f) * block_width;
		float start_y = (blockY - 0.5f) * block_width;

		for (int i = 0, iy = 0; iy <= block_size; ++iy)
		{
			for (int ix = 0; ix <= block_size; ++ix)
			{
				float x = start_x + ix * grid_size;
				float z = start_y + iy * grid_size;
				float y = sampler(x, z);

				const float normal_step = grid_size * 0.5f;

				glm::vec3 sx(2.0f * normal_step, 0.0f, sampler(x + normal_step, z) - sampler(x - normal_step, z));
				glm::vec3 sz(0.0f, 2.0f * normal_step, sampler(x, z + normal_step) - sampler(x, z - normal_step));

				glm::vec3 normal = glm::cross(glm::normalize(sx), glm::normalize(sz));

				//glm::vec3 normal(
				//	sampler(x - normal_step, z) - sampler(x + normal_step, z),
				//	2 * normal_step,
				//	sampler(x, z - normal_step) - sampler(x, z + normal_step)
				//);

				//normal = glm::normalize(normal);

				buffer[i++] = x;
				buffer[i++] = y;
				buffer[i++] = z;
				buffer[i++] = normal.x;
				buffer[i++] = normal.y;
				buffer[i++] = normal.z;
			}
		}
	}

}
