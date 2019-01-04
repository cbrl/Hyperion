#pragma once

#include "directx/d3d11.h"
#include "datatypes/datatypes.h"
#include "geometry/shapes/shapes.h"
#include "resource/resource_mgr.h"


namespace BlueprintFactory {

	template<typename VertexT>
	[[nodiscard]]
	static std::shared_ptr<ModelBlueprint> CreateCube(ResourceMgr& resource_mgr,
	                                                  const ModelConfig<VertexT>& config,
	                                                  f32 size = 1);

	template<typename VertexT>
	[[nodiscard]]
	static std::shared_ptr<ModelBlueprint> CreateBox(ResourceMgr& resource_mgr,
	                                                 const ModelConfig<VertexT>& config,
	                                                 const vec3_f32& size);

	template<typename VertexT>
	[[nodiscard]]
	static std::shared_ptr<ModelBlueprint> CreateSphere(ResourceMgr& resource_mgr,
	                                                    const ModelConfig<VertexT>& config,
	                                                    f32    diameter     = 1,
	                                                    size_t tessellation = 16);

	template<typename VertexT>
	[[nodiscard]]
	static std::shared_ptr<ModelBlueprint> CreateGeoSphere(ResourceMgr& resource_mgr,
	                                                       const ModelConfig<VertexT>& config,
	                                                       f32    diameter     = 1,
	                                                       size_t tessellation = 3);

	template<typename VertexT>
	[[nodiscard]]
	static std::shared_ptr<ModelBlueprint> CreateCylinder(ResourceMgr& resource_mgr,
	                                                      const ModelConfig<VertexT>& config,
	                                                      f32    diameter     = 1,
	                                                      f32    height       = 1,
	                                                      size_t tessellation = 32);

	template<typename VertexT>
	[[nodiscard]]
	static std::shared_ptr<ModelBlueprint> CreateCone(ResourceMgr& resource_mgr,
	                                                  const ModelConfig<VertexT>& config,
	                                                  f32    diameter     = 1,
	                                                  f32    height       = 1,
	                                                  size_t tessellation = 32);

	template<typename VertexT>
	[[nodiscard]]
	static std::shared_ptr<ModelBlueprint> CreateTorus(ResourceMgr& resource_mgr,
	                                                   const ModelConfig<VertexT>& config,
	                                                   f32    diameter     = 1,
	                                                   f32    thickness    = 0.333f,
	                                                   size_t tessellation = 32);

	template<typename VertexT>
	[[nodiscard]]
	static std::shared_ptr<ModelBlueprint> CreateTetrahedron(ResourceMgr& resource_mgr,
	                                                         const ModelConfig<VertexT>& config,
	                                                         f32 size = 1);

	template<typename VertexT>
	[[nodiscard]]
	static std::shared_ptr<ModelBlueprint> CreateOctahedron(ResourceMgr& resource_mgr,
	                                                        const ModelConfig<VertexT>& config,
	                                                        f32 size = 1);

	template<typename VertexT>
	[[nodiscard]]
	static std::shared_ptr<ModelBlueprint> CreateDodecahedron(ResourceMgr& resource_mgr,
	                                                          const ModelConfig<VertexT>& config,
	                                                          f32 size = 1);

	template<typename VertexT>
	[[nodiscard]]
	static std::shared_ptr<ModelBlueprint> CreateIcosahedron(ResourceMgr& resource_mgr,
	                                                         const ModelConfig<VertexT>& config,
	                                                         f32 size = 1);
};


#include "blueprint_factory.tpp"
