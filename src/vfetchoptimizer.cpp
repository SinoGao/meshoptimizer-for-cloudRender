// This file is part of meshoptimizer library; see meshoptimizer.hpp for version/license details
#include "meshoptimizer.hpp"

#include <cassert>
#include <cstring>
#include <vector>

namespace meshopt
{

template <typename T>
static void optimizeVertexFetchImpl(void* destination, const void* vertices, T* indices, size_t index_count, size_t vertex_count, size_t vertex_size)
{
	assert(destination != vertices);

	// build vertex remap table
	std::vector<unsigned int> vertex_remap(vertex_count, static_cast<unsigned int>(-1));

	unsigned int vertex = 0;

	for (size_t i = 0; i < index_count; ++i)
	{
		T index = indices[i];
		assert(index < vertex_count);

		unsigned int& remap = vertex_remap[index];

		if (remap == static_cast<unsigned int>(-1)) // vertex was not added to destination VB
		{
			// add vertex
			memcpy(static_cast<char*>(destination) + vertex * vertex_size, static_cast<const char*>(vertices) + index * vertex_size, vertex_size);

			remap = vertex++;
		}

		// modify indices in place
		indices[i] = static_cast<T>(remap);
	}
}

void optimizeVertexFetch(void* destination, const void* vertices, unsigned short* indices, size_t index_count, size_t vertex_count, size_t vertex_size)
{
	optimizeVertexFetchImpl(destination, vertices, indices, index_count, vertex_count, vertex_size);
}

void optimizeVertexFetch(void* destination, const void* vertices, unsigned int* indices, size_t index_count, size_t vertex_count, size_t vertex_size)
{
	optimizeVertexFetchImpl(destination, vertices, indices, index_count, vertex_count, vertex_size);
}

} // namespace meshopt