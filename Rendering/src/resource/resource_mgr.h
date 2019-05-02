#pragma once

#include "datatypes/datatypes.h"
#include "memory/managed_resource_map.h"
#include "resource/resource.h"
#include "shader/shader.h"
#include "resource/mesh/mesh.h"
#include "resource/model/model_blueprint.h"
#include "resource/model/model_output.h"
#include "resource/texture/texture.h"
#include "resource/font/font.h"


namespace render {

class ModelBlueprint;

class ResourceMgr final {
	template<typename KeyT, typename ValueT>
	using resource_map = ConcurrentWeakResourceMap<KeyT, ValueT>;

	template<typename KeyT, typename ValueT>
	using shader_resource_map = ConcurrentSharedResourceMap<KeyT, ValueT>;

public:

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	ResourceMgr(ID3D11Device& device, ID3D11DeviceContext& device_context)
		: device(device)
		, device_context(device_context) {
	}

	ResourceMgr(const ResourceMgr& mgr) = delete;
	ResourceMgr(ResourceMgr&& mgr) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~ResourceMgr() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	ResourceMgr& operator=(const ResourceMgr& mgr) = delete;
	ResourceMgr& operator=(ResourceMgr&& mgr) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - ModelBlueprint
	//----------------------------------------------------------------------------------

	template<typename ResourceT, typename VertexT>
	[[nodiscard]]
	auto getOrCreate(const std::wstring& filename,
	                 const ModelConfig<VertexT>& config)
	    -> std::enable_if_t<std::is_same_v<ModelBlueprint, ResourceT>, std::shared_ptr<ModelBlueprint>>;

	template<typename ResourceT, typename VertexT>
	[[nodiscard]]
	auto getOrCreate(const std::wstring& name,
	                 const ModelOutput& model_data,
	                 const ModelConfig<VertexT>& config)
	    -> std::enable_if_t<std::is_same_v<ModelBlueprint, ResourceT>, std::shared_ptr<ModelBlueprint>>;

	template<typename ResourceT, typename VertexT>
	[[nodiscard]]
	auto createOrReplace(const std::wstring& filename,
	                     const ModelConfig<VertexT>& config)
	    -> std::enable_if_t<std::is_same_v<ModelBlueprint, ResourceT>, std::shared_ptr<ModelBlueprint>>;

	template<typename ResourceT, typename VertexT>
	[[nodiscard]]
	auto createOrReplace(const std::wstring& name,
	                     const ModelOutput& model_data,
	                     const ModelConfig<VertexT>& config)
	    -> std::enable_if_t<std::is_same_v<ModelBlueprint, ResourceT>, std::shared_ptr<ModelBlueprint>>;

	template<typename ResourceT>
	[[nodiscard]]
	auto getResourceMap() const
		-> std::enable_if_t<std::is_same_v<ModelBlueprint, ResourceT>, const resource_map<std::wstring, ModelBlueprint>&>;
		


	//----------------------------------------------------------------------------------
	// Member Functions - Texture
	//----------------------------------------------------------------------------------

	template<typename ResourceT>
	[[nodiscard]]
	auto getOrCreate(const std::wstring& filename)
	    -> std::enable_if_t<std::is_same_v<Texture, ResourceT>, std::shared_ptr<Texture>>;

	template<typename ResourceT>
	[[nodiscard]]
	auto getOrCreate(const std::wstring& name,
	                 const D3D11_TEXTURE2D_DESC& desc,
	                 const D3D11_SUBRESOURCE_DATA& init_data)
	    -> std::enable_if_t<std::is_same_v<Texture, ResourceT>, std::shared_ptr<Texture>>;

	template<typename ResourceT>
	[[nodiscard]]
	auto createOrReplace(const std::wstring& filename)
	    -> std::enable_if_t<std::is_same_v<Texture, ResourceT>, std::shared_ptr<Texture>>;

	template<typename ResourceT>
	[[nodiscard]]
	auto createOrReplace(const std::wstring& name,
	                     const D3D11_TEXTURE2D_DESC& desc,
	                     const D3D11_SUBRESOURCE_DATA& init_data)
	    -> std::enable_if_t<std::is_same_v<Texture, ResourceT>, std::shared_ptr<Texture>>;

	template<typename ResourceT>
	[[nodiscard]]
	auto getResourceMap() const
		-> std::enable_if_t<std::is_same_v<Texture, ResourceT>, const resource_map<std::wstring, Texture>&>;


	//----------------------------------------------------------------------------------
	// Member Functions - Font
	//----------------------------------------------------------------------------------

	template<typename ResourceT>
	[[nodiscard]]
	auto getOrCreate(const std::wstring& filename)
	    -> std::enable_if_t<std::is_same_v<Font, ResourceT>, std::shared_ptr<Font>>;

	template<typename ResourceT>
	[[nodiscard]]
	auto createOrReplace(const std::wstring& filename)
	    -> std::enable_if_t<std::is_same_v<Font, ResourceT>, std::shared_ptr<Font>>;

	template<typename ResourceT>
	[[nodiscard]]
	auto getResourceMap() const
		-> std::enable_if_t<std::is_same_v<Font, ResourceT>, const resource_map<std::wstring, Font>&>;


	//----------------------------------------------------------------------------------
	// Member Functions - Compute Shader
	//----------------------------------------------------------------------------------

	template<typename ResourceT>
	[[nodiscard]]
	auto getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode)
	    -> std::enable_if_t<std::is_same_v<ComputeShader, ResourceT>, std::shared_ptr<ComputeShader>>;

	template<typename ResourceT>
	[[nodiscard]]
	auto createOrReplace(const std::wstring& guid, const ShaderBytecode& bytecode)
	    -> std::enable_if_t<std::is_same_v<ComputeShader, ResourceT>, std::shared_ptr<ComputeShader>>;

	template<typename ResourceT>
	[[nodiscard]]
	auto getResourceMap() const
		-> std::enable_if_t<std::is_same_v<ComputeShader, ResourceT>, const shader_resource_map<std::wstring, ComputeShader>&>;


	//----------------------------------------------------------------------------------
	// Member Functions - Domain Shader
	//----------------------------------------------------------------------------------

	template<typename ResourceT>
	[[nodiscard]]
	auto getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode)
	    -> std::enable_if_t<std::is_same_v<DomainShader, ResourceT>, std::shared_ptr<DomainShader>>;

	template<typename ResourceT>
	[[nodiscard]]
	auto createOrReplace(const std::wstring& guid, const ShaderBytecode& bytecode)
	    -> std::enable_if_t<std::is_same_v<DomainShader, ResourceT>, std::shared_ptr<DomainShader>>;

	template<typename ResourceT>
	[[nodiscard]]
	auto getResourceMap() const
	    -> std::enable_if_t<std::is_same_v<DomainShader, ResourceT>, const shader_resource_map<std::wstring, DomainShader>&>;


	//----------------------------------------------------------------------------------
	// Member Functions - Geometry Shader
	//----------------------------------------------------------------------------------

	template<typename ResourceT>
	[[nodiscard]]
	auto getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode)
	    -> std::enable_if_t<std::is_same_v<GeometryShader, ResourceT>, std::shared_ptr<GeometryShader>>;

	template<typename ResourceT>
	[[nodiscard]]
	auto createOrReplace(const std::wstring& guid, const ShaderBytecode& bytecode)
	    -> std::enable_if_t<std::is_same_v<GeometryShader, ResourceT>, std::shared_ptr<GeometryShader>>;

	template<typename ResourceT>
	[[nodiscard]]
	auto getResourceMap() const
	    -> std::enable_if_t<std::is_same_v<GeometryShader, ResourceT>, const shader_resource_map<std::wstring, GeometryShader>&>;


	//----------------------------------------------------------------------------------
	// Member Functions - Hull Shader
	//----------------------------------------------------------------------------------

	template<typename ResourceT>
	[[nodiscard]]
	auto getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode)
	    -> std::enable_if_t<std::is_same_v<HullShader, ResourceT>, std::shared_ptr<HullShader>>;

	template<typename ResourceT>
	[[nodiscard]]
	auto createOrReplace(const std::wstring& guid, const ShaderBytecode& bytecode)
	    -> std::enable_if_t<std::is_same_v<HullShader, ResourceT>, std::shared_ptr<HullShader>>;

	template<typename ResourceT>
	[[nodiscard]]
	auto getResourceMap() const
	    -> std::enable_if_t<std::is_same_v<HullShader, ResourceT>, const shader_resource_map<std::wstring, HullShader>&>;


	//----------------------------------------------------------------------------------
	// Member Functions - Pixel Shader
	//----------------------------------------------------------------------------------

	template<typename ResourceT>
	[[nodiscard]]
	auto getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode)
	    -> std::enable_if_t<std::is_same_v<PixelShader, ResourceT>, std::shared_ptr<PixelShader>>;

	template<typename ResourceT>
	[[nodiscard]]
	auto createOrReplace(const std::wstring& guid, const ShaderBytecode& bytecode)
	    -> std::enable_if_t<std::is_same_v<PixelShader, ResourceT>, std::shared_ptr<PixelShader>>;

	template<typename ResourceT>
	[[nodiscard]]
	auto getResourceMap() const
	    -> std::enable_if_t<std::is_same_v<PixelShader, ResourceT>, const shader_resource_map<std::wstring, PixelShader>&>;


	//----------------------------------------------------------------------------------
	// Member Functions - Vertex Shader
	//----------------------------------------------------------------------------------

	template<typename ResourceT>
	[[nodiscard]]
	auto getOrCreate(const std::wstring& guid,
	             const ShaderBytecode& bytecode,
	             gsl::span<const D3D11_INPUT_ELEMENT_DESC> input_element_descs)
	    -> std::enable_if_t<std::is_same_v<VertexShader, ResourceT>, std::shared_ptr<VertexShader>>;

	template<typename ResourceT>
	[[nodiscard]]
	auto createOrReplace(const std::wstring& guid,
	                     const ShaderBytecode& bytecode,
	                     gsl::span<const D3D11_INPUT_ELEMENT_DESC> input_element_descs)
	    -> std::enable_if_t<std::is_same_v<VertexShader, ResourceT>, std::shared_ptr<VertexShader>>;

	template<typename ResourceT>
	[[nodiscard]]
	auto getResourceMap() const
	    -> std::enable_if_t<std::is_same_v<VertexShader, ResourceT>, const shader_resource_map<std::wstring, VertexShader>&>;

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Device and device context
	std::reference_wrapper<ID3D11Device>        device;
	std::reference_wrapper<ID3D11DeviceContext> device_context;

	// Resources
	resource_map<std::wstring, ModelBlueprint> models;
	resource_map<std::wstring, Texture>        textures;
	resource_map<std::wstring, Font>           fonts;

	shader_resource_map<std::wstring, ComputeShader>  compute_shaders;
	shader_resource_map<std::wstring, DomainShader>   domain_shaders;
	shader_resource_map<std::wstring, GeometryShader> geometry_shaders;
	shader_resource_map<std::wstring, HullShader>     hull_shaders;
	shader_resource_map<std::wstring, PixelShader>    pixel_shaders;
	shader_resource_map<std::wstring, VertexShader>   vertex_shaders;
};

} //namespace render

#include "resource_mgr.tpp"