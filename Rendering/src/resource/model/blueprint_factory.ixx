module;

#include "datatypes/types.h"
#include "directx/d3d11.h"

export module rendering.blueprint_factory;

export import rendering.model_config;
import rendering.resource_mgr;
import rendering.material_factory;
import rendering.importer.model_importer;

import math.geometry.shapes;


export namespace render::BlueprintFactory {

template<typename VertexT>
[[nodiscard]]
std::shared_ptr<ModelBlueprint> LoadModelFile(ID3D11Device& device,
                                              ResourceMgr& resource_mgr,
                                              const std::wstring& filename,
                                              const ModelConfig<VertexT>& config);

template<typename VertexT>
[[nodiscard]]
std::shared_ptr<ModelBlueprint> CreateCube(ResourceMgr& resource_mgr,
	                                       const ModelConfig<VertexT>& config,
	                                       f32 size = 1);

template<typename VertexT>
[[nodiscard]]
std::shared_ptr<ModelBlueprint> CreateBox(ResourceMgr& resource_mgr,
	                                      const ModelConfig<VertexT>& config,
	                                      const f32_3& size);

template<typename VertexT>
[[nodiscard]]
std::shared_ptr<ModelBlueprint> CreateSphere(ResourceMgr& resource_mgr,
	                                         const ModelConfig<VertexT>& config,
	                                         f32    diameter     = 1,
	                                         size_t tessellation = 16);

template<typename VertexT>
[[nodiscard]]
std::shared_ptr<ModelBlueprint> CreateGeoSphere(ResourceMgr& resource_mgr,
	                                            const ModelConfig<VertexT>& config,
	                                            f32    diameter     = 1,
	                                            size_t tessellation = 3);

template<typename VertexT>
[[nodiscard]]
std::shared_ptr<ModelBlueprint> CreateCylinder(ResourceMgr& resource_mgr,
	                                           const ModelConfig<VertexT>& config,
	                                           f32    diameter     = 1,
	                                           f32    height       = 1,
	                                           size_t tessellation = 32);

template<typename VertexT>
[[nodiscard]]
std::shared_ptr<ModelBlueprint> CreateCone(ResourceMgr& resource_mgr,
	                                       const ModelConfig<VertexT>& config,
	                                       f32    diameter     = 1,
	                                       f32    height       = 1,
	                                       size_t tessellation = 32);

template<typename VertexT>
[[nodiscard]]
std::shared_ptr<ModelBlueprint> CreateTorus(ResourceMgr& resource_mgr,
	                                        const ModelConfig<VertexT>& config,
	                                        f32    diameter     = 1,
	                                        f32    thickness    = 0.333f,
	                                        size_t tessellation = 32);

template<typename VertexT>
[[nodiscard]]
std::shared_ptr<ModelBlueprint> CreateTetrahedron(ResourceMgr& resource_mgr,
	                                              const ModelConfig<VertexT>& config,
	                                              f32 size = 1);

template<typename VertexT>
[[nodiscard]]
std::shared_ptr<ModelBlueprint> CreateOctahedron(ResourceMgr& resource_mgr,
	                                             const ModelConfig<VertexT>& config,
	                                             f32 size = 1);

template<typename VertexT>
[[nodiscard]]
std::shared_ptr<ModelBlueprint> CreateDodecahedron(ResourceMgr& resource_mgr,
	                                               const ModelConfig<VertexT>& config,
	                                               f32 size = 1);

template<typename VertexT>
[[nodiscard]]
std::shared_ptr<ModelBlueprint> CreateIcosahedron(ResourceMgr& resource_mgr,
	                                              const ModelConfig<VertexT>& config,
	                                              f32 size = 1);

} //namespace render::BlueprintFactory

#include "blueprint_factory.tpp"
