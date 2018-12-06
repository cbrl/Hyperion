#pragma once

#include "directx/d3d11.h"
#include "resource/model/material/material.h"


//----------------------------------------------------------------------------------
// ModelOutput
//----------------------------------------------------------------------------------
//
// A struct returned by the model loader, and used in the creation of a ModelBlueprint
//
//----------------------------------------------------------------------------------
struct ModelOutput {

	//----------------------------------------------------------------------------------
	// Member Structs
	//----------------------------------------------------------------------------------

	struct Node {
		std::string name;
		std::vector<u32> mesh_indices;  //Array of indices used to index mesh array
		std::vector<Node> child_nodes;
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


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// The file that this data was loaded from (optional)
	fs::path file;

	// The name of this output
	std::string name;

	// The meshes contained in the output
	std::vector<MeshData> meshes;

	// The materials contained in the output
	std::vector<Material> materials;

	// The root node of the model hierarchy
	Node root;
};