#pragma once

#include <d3d11.h>
#include "util\datatypes\datatypes.h"
#include "geometry\shapes\shapes.h"
#include "resource\model\model_output.h"
#include "resource\model\model_blueprint.h"


namespace BlueprintFactory {
	template<typename VertexT>
	static shared_ptr<ModelBlueprint> CreateCube(ID3D11Device* device, float size = 1, bool rhcoords = false, bool invertn = false);

	template<typename VertexT>
	static shared_ptr<ModelBlueprint> CreateBox(ID3D11Device* device, const float3& size, bool rhcoords = false, bool invertn = false);

	template<typename VertexT>
	static shared_ptr<ModelBlueprint> CreateSphere(ID3D11Device* device, float diameter = 1, size_t tessellation = 16, bool rhcoords = false, bool invertn = false);

	template<typename VertexT>
	static shared_ptr<ModelBlueprint> CreateGeoSphere(ID3D11Device* device, float diameter = 1, size_t tessellation = 3, bool rhcoords = false);

	template<typename VertexT>
	static shared_ptr<ModelBlueprint> CreateCylinder(ID3D11Device* device, float height = 1, float diameter = 1, size_t tessellation = 32, bool rhcoords = false);

	template<typename VertexT>
	static shared_ptr<ModelBlueprint> CreateCone(ID3D11Device* device, float diameter = 1, float height = 1, size_t tessellation = 32, bool rhcoords = false);

	template<typename VertexT>
	static shared_ptr<ModelBlueprint> CreateTorus(ID3D11Device* device, float diameter = 1, float thickness = 0.333f, size_t tessellation = 32, bool rhcoords = false);

	template<typename VertexT>
	static shared_ptr<ModelBlueprint> CreateTetrahedron(ID3D11Device* device, float size = 1, bool rhcoords = false);

	template<typename VertexT>
	static shared_ptr<ModelBlueprint> CreateOctahedron(ID3D11Device* device, float size = 1, bool rhcoords = false);

	template<typename VertexT>
	static shared_ptr<ModelBlueprint> CreateDodecahedron(ID3D11Device* device, float size = 1, bool rhcoords = false);

	template<typename VertexT>
	static shared_ptr<ModelBlueprint> CreateIcosahedron(ID3D11Device* device, float size = 1, bool rhcoords = false);
};


#include "blueprint_factory.tpp"