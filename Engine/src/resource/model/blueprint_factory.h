#pragma once

#include <d3d11.h>
#include "datatypes/datatypes.h"
#include "geometry/shapes/shapes.h"
#include "resource/resource_mgr.h"


namespace BlueprintFactory {
	template<typename VertexT>
	static shared_ptr<ModelBlueprint> CreateCube(ResourceMgr& resource_mgr,
	                                             float size    = 1,
	                                             bool rhcoords = false,
	                                             bool invertn  = false);

	template<typename VertexT>
	static shared_ptr<ModelBlueprint> CreateBox(ResourceMgr& resource_mgr,
	                                            const float3& size,
	                                            bool rhcoords = false,
	                                            bool invertn  = false);

	template<typename VertexT>
	static shared_ptr<ModelBlueprint> CreateSphere(ResourceMgr& resource_mgr,
	                                               float diameter      = 1,
	                                               size_t tessellation = 16,
	                                               bool rhcoords       = false,
	                                               bool invertn        = false);

	template<typename VertexT>
	static shared_ptr<ModelBlueprint> CreateGeoSphere(ResourceMgr& resource_mgr,
	                                                  float diameter      = 1,
	                                                  size_t tessellation = 3,
	                                                  bool rhcoords       = false);

	template<typename VertexT>
	static shared_ptr<ModelBlueprint> CreateCylinder(ResourceMgr& resource_mgr,
	                                                 float diameter      = 1,
	                                                 float height        = 1,
	                                                 size_t tessellation = 32,
	                                                 bool rhcoords       = false);

	template<typename VertexT>
	static shared_ptr<ModelBlueprint> CreateCone(ResourceMgr& resource_mgr,
	                                             float diameter      = 1,
	                                             float height        = 1,
	                                             size_t tessellation = 32,
	                                             bool rhcoords       = false);

	template<typename VertexT>
	static shared_ptr<ModelBlueprint> CreateTorus(ResourceMgr& resource_mgr,
	                                              float diameter      = 1,
	                                              float thickness     = 0.333f,
	                                              size_t tessellation = 32,
	                                              bool rhcoords       = false);

	template<typename VertexT>
	static shared_ptr<ModelBlueprint> CreateTetrahedron(ResourceMgr& resource_mgr,
	                                                    float size    = 1,
	                                                    bool rhcoords = false);

	template<typename VertexT>
	static shared_ptr<ModelBlueprint> CreateOctahedron(ResourceMgr& resource_mgr,
	                                                   float size    = 1,
	                                                   bool rhcoords = false);

	template<typename VertexT>
	static shared_ptr<ModelBlueprint> CreateDodecahedron(ResourceMgr& resource_mgr,
	                                                     float size    = 1,
	                                                     bool rhcoords = false);

	template<typename VertexT>
	static shared_ptr<ModelBlueprint> CreateIcosahedron(ResourceMgr& resource_mgr,
	                                                    float size    = 1,
	                                                    bool rhcoords = false);
};


#include "blueprint_factory.tpp"
