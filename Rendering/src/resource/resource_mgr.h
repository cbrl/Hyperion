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


class ModelBlueprint;


class ResourceMgr final {
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
	auto getOrCreate(const std::wstring& filename, const ModelConfig<VertexT>& config)
	    -> std::enable_if_t<std::is_same_v<ModelBlueprint, ResourceT>, std::shared_ptr<ModelBlueprint>>;

	template<typename ResourceT, typename VertexT>
	auto getOrCreate(const std::wstring& name,
	                 const ModelOutput& model_data,
	                 const ModelConfig<VertexT>& config)
	    -> std::enable_if_t<std::is_same_v<ModelBlueprint, ResourceT>, std::shared_ptr<ModelBlueprint>>;


	//----------------------------------------------------------------------------------
	// Member Functions - Texture
	//----------------------------------------------------------------------------------

	template<typename ResourceT>
	auto getOrCreate(const std::wstring& filename)
	    -> std::enable_if_t<std::is_same_v<Texture, ResourceT>, std::shared_ptr<Texture>>;

	template<typename ResourceT>
	auto getOrCreate(const std::wstring& name,
	                 const D3D11_TEXTURE2D_DESC& desc,
	                 const D3D11_SUBRESOURCE_DATA& init_data)
	    -> std::enable_if_t<std::is_same_v<Texture, ResourceT>, std::shared_ptr<Texture>>;


	//----------------------------------------------------------------------------------
	// Member Functions - Font
	//----------------------------------------------------------------------------------

	template<typename ResourceT>
	auto getOrCreate(const std::wstring& filename)
	    -> std::enable_if_t<std::is_same_v<Font, ResourceT>, std::shared_ptr<Font>>;


	//----------------------------------------------------------------------------------
	// Member Functions - Shader
	//----------------------------------------------------------------------------------

	template<typename ResourceT>
	auto getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode)
	    -> std::enable_if_t<std::is_same_v<ComputeShader, ResourceT>, std::shared_ptr<ComputeShader>>;

	template<typename ResourceT>
	auto getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode)
	    -> std::enable_if_t<std::is_same_v<DomainShader, ResourceT>, std::shared_ptr<DomainShader>>;

	template<typename ResourceT>
	auto getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode)
	    -> std::enable_if_t<std::is_same_v<GeometryShader, ResourceT>, std::shared_ptr<GeometryShader>>;

	template<typename ResourceT>
	auto getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode)
	    -> std::enable_if_t<std::is_same_v<HullShader, ResourceT>, std::shared_ptr<HullShader>>;

	template<typename ResourceT>
	auto getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode)
	    -> std::enable_if_t<std::is_same_v<PixelShader, ResourceT>, std::shared_ptr<PixelShader>>;

	template<typename ResourceT>
	auto getOrCreate(const std::wstring& guid,
	                 const ShaderBytecode& bytecode,
	                 gsl::span<const D3D11_INPUT_ELEMENT_DESC> input_element_descs)
	    -> std::enable_if_t<std::is_same_v<VertexShader, ResourceT>, std::shared_ptr<VertexShader>>;


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Device and device context
	std::reference_wrapper<ID3D11Device> device;
	std::reference_wrapper<ID3D11DeviceContext> device_context;

	// Resources
	ManagedResourceMap<std::wstring, ModelBlueprint> models;
	ManagedResourceMap<std::wstring, Texture> textures;
	ManagedResourceMap<std::wstring, Font> fonts;

	ManagedResourceMap<std::wstring, ComputeShader>  compute_shaders;
	ManagedResourceMap<std::wstring, DomainShader>   domain_shaders;
	ManagedResourceMap<std::wstring, GeometryShader> geometry_shaders;
	ManagedResourceMap<std::wstring, HullShader>     hull_shaders;
	ManagedResourceMap<std::wstring, PixelShader>    pixel_shaders;
	ManagedResourceMap<std::wstring, VertexShader>   vertex_shaders;
};


#include "resource_mgr.tpp"
