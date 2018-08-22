#pragma once

#include "directx/d3d11.h"
#include "resource/model/material/material.h"
#include "geometry/bounding_volume/bounding_volume.h"


// Groups are defined by the loader/factory, and
// transformed into complete model parts by the
// ModelOuptut constructor.
struct Group {
	std::string name;
	u32 index_start;
	u32 material_index;
};


struct ModelPart {
	std::string name;
	u32 index_start;
	u32 index_count;
	u32 material_index;
	AABB aabb;
	BoundingSphere sphere;
};


// A ModelOuput object is returned by a model loader and
// used in the construction of a ModelBlueprint.
template<typename VertexT>
struct ModelOutput final {
public:
	ModelOutput(const std::string& name,
	            const std::vector<VertexT>& vertices,
	            const std::vector<u32>& indices,
	            const std::vector<Material>& materials,
	            const std::vector<Group>& groups);


public:
	std::string name;
	std::vector<VertexT> vertices;
	std::vector<u32> indices;
	std::vector<Material> materials;
	std::vector<ModelPart> model_parts;
};


#include "model_output.tpp"
