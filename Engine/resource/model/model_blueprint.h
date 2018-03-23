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
	wstring name;
	u32     index_start;
	u32     index_count;
	u32     material_index;
	AABB    aabb;
	BoundingSphere sphere;
};


class ModelBlueprint {
	public:
		ModelBlueprint() = default;

		ModelBlueprint(ID3D11Device* device,
					   ResourceMgr& resource_mgr,
					   const wstring& folder,
					   const wstring& filename);

		~ModelBlueprint() = default;


	public:
		wstring name;

		AABB           aabb;
		BoundingSphere sphere;

		shared_ptr<Mesh> mesh;

		vector<Material>  materials;
		vector<ModelPart> model_parts;
};


//#include "model_blueprint.tpp"