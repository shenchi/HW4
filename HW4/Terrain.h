#pragma once

#include "CommonTypes.h"

#include <functional>
#include <cmath>

namespace hw4
{
	class Terrain
	{
	public:
		Terrain(float gridSize, int blockSize);
		~Terrain();

		void SetPlayerPosition(float x, float y, float z);

		inline void SetSamplingFunction(std::function<float(float, float)> sampler) {
			this->sampler = sampler;
		}

		inline const uint32* IndexBuffer() const { return indices; }
		inline size_t IndexBufferSize() const { return indices_count * sizeof(uint32); }
		inline size_t IndicesCount() const { return indices_count; }

		inline const float* VertexBuffer(int idx) const { return data[idx]; }
		inline size_t VertexBufferSize() const { return block_data_size * sizeof(float); }

		bool IsUpdateFlagSet(int idx) const;
		void ClearUpdateFlag(int idx);

	private:

		inline int to_block_coord(float x) const { 
			return static_cast<int>(floor(x / block_width + 0.5f)); 
		}

		inline int block_coord_to_index(int x, int y) const { return (y + 1) * 3 + x + 1; }

		void WorkingThread();

		void GenerateBlock(float* buffer, int blockX, int blockY);

	private:
		float	grid_size;
		int		block_size;

		float	block_width;

		uint32*	indices;
		size_t	indices_count;

		float*	data[9];
		size_t	block_data_size;

		int		buffer_id[9];

		int		current_block_x;
		int		current_block_y;
		bool	initialized;

		std::function<float(float, float)> sampler;

		struct ThreadData;
		ThreadData*	thread_data;
	};
}
