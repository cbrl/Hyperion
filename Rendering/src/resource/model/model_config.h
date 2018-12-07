#pragma once


template <typename VertexT>
struct ModelConfig {
	// The vertex type to use for the model
	using vertex_t = VertexT;

	// Flips the vertex winding order if true
	bool flip_winding = false;

	// Flips UV coordinates if true
	bool flip_uv = false;
};
