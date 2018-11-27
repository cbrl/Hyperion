#pragma once

#include "directx/d3d11.h"
#include "resource/model/material/material.h"


struct ModelOutput {
	struct Node {
		std::string name;
		std::vector<u32> mesh_indices;  //Meshes in this node
		std::vector<Node> child_nodes;  //Child nodes
	};
	struct MeshData {
		std::string name;
		std::vector<u32> indices;
		std::vector<vec3_f32> positions;
		std::vector<vec3_f32> normals;
		std::vector<vec3_f32> tangents;
		std::vector<vec3_f32> bitangents;
		std::vector<vec2_f32> texture_coords;
		std::vector<vec3_f32> colors;
		u32 material_index = 0;
	};

	std::string name;
	std::vector<MeshData> meshes;
	std::vector<Material> materials;
	Node root;
};