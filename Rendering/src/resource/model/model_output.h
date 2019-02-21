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
		std::vector<f32_3> positions;
		std::vector<f32_3> normals;
		std::vector<f32_3> tangents;
		std::vector<f32_3> bitangents;
		std::vector<f32_2> texture_coords;
		std::vector<f32_3> colors;
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