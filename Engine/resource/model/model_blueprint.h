#pragma once

#include <d3d11.h>
#include "util\datatypes\datatypes.h"
#include "util\math\math.h"
#include "resource\resource_mgr.h"
#include "resource\mesh\mesh.h"
#include "resource\model\model_output.h"
#include "resource\model\material\material.h"
#include "geometry\bounding_volume\bounding_volume.h"


class ResourceMgr;


class ModelBlueprint final {
	public:
		ModelBlueprint() = default;
		~ModelBlueprint() = default;

		ModelBlueprint(ID3D11Device* device,
					   ResourceMgr& resource_mgr,
					   const wstring& filename);

		template<typename VertexT>
		ModelBlueprint(ID3D11Device* device,
					   const ModelOutput<VertexT>& out)
		{
			ConstructBlueprint(device, out);
		}


	private:
		template<typename VertexT>
		void ConstructBlueprint(ID3D11Device* device, const ModelOutput<VertexT>& out);


	public:
		string name;

		AABB           aabb;
		BoundingSphere sphere;

		shared_ptr<Mesh> mesh;

		vector<Material> materials;

		vector<ModelPart> model_parts;
};


#include "model_blueprint.tpp"