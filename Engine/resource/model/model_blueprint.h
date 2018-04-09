#pragma once

#include <d3d11.h>
#include "util\datatypes\datatypes.h"
#include "util\math\math.h"
#include "resource\resource_mgr.h"
#include "resource\mesh\mesh.h"
#include "resource\material\material.h"
#include "geometry\boundingvolume\bounding_volume.h"
#include "resource\model\model_output.h"


class ResourceMgr;


struct ModelPart {
	string  name;
	u32     index_start;
	u32     index_count;
	u32     material_index;
	AABB    aabb;
	BoundingSphere sphere;
};


class ModelBlueprint final {
	public:
		ModelBlueprint() = default;

		ModelBlueprint(ID3D11Device* device,
					   ResourceMgr& resource_mgr,
					   const wstring& filename);

		~ModelBlueprint() = default;


	public:
		string name;

		AABB           aabb;
		BoundingSphere sphere;

		shared_ptr<Mesh> mesh;

		vector<Material>  materials;
		vector<ModelPart> model_parts;
};
