#pragma once

#include <d3d11.h>
#include "datatypes/datatypes.h"
#include "resource/model/material/material.h"
#include "geometry/bounding_volume/bounding_volume.h"


// Groups are defined by the loader/factory, and
// transformed into complete model parts by the
// ModelOuptut constructor.
struct Group {
	string name;
	u32 index_start;
	u32 material_index;
};


struct ModelPart {
	string name;
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
	ModelOutput() = default;
	~ModelOutput() = default;

	ModelOutput(const string& name,
	            const vector<VertexT>& vertices,
	            const vector<u32>& indices,
	            const vector<Material>& materials,
	            const vector<Group>& groups);


public:
	string name;
	vector<VertexT> vertices;
	vector<u32> indices;
	vector<Material> materials;
	vector<ModelPart> model_parts;
	AABB aabb;
	BoundingSphere sphere;
};


#include "model_output.tpp"
