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

	template<typename ResourceT>
	std::enable_if_t<std::is_same_v<ModelBlueprint, ResourceT>,
		shared_ptr<ModelBlueprint>> getOrCreate(const std::wstring& filename);

	template<typename ResourceT, typename VertexT>
	std::enable_if_t<std::is_same_v<ModelBlueprint, ResourceT>,
		shared_ptr<ModelBlueprint>> getOrCreate(const std::wstring& name,
		                                        const ModelOutput<VertexT>& model_data);


	//----------------------------------------------------------------------------------
	// Member Functions - Texture
	//----------------------------------------------------------------------------------

	template<typename ResourceT>
	std::enable_if_t<std::is_same_v<Texture, ResourceT>,
		shared_ptr<Texture>> getOrCreate(const std::wstring& filename);

	template<typename ResourceT>
	std::enable_if_t<std::is_same_v<Texture, ResourceT>,
		shared_ptr<Texture>> getOrCreate(const std::wstring& name,
		                                 const D3D11_TEXTURE2D_DESC& desc,
		                                 const D3D11_SUBRESOURCE_DATA& init_data);


	//----------------------------------------------------------------------------------
	// Member Functions - Font
	//----------------------------------------------------------------------------------

	template<typename ResourceT>
	std::enable_if_t<std::is_same_v<Font, ResourceT>,
		shared_ptr<Font>> getOrCreate(const std::wstring& filename);


	//----------------------------------------------------------------------------------
	// Member Functions - Shader
	//----------------------------------------------------------------------------------

	template<typename ResourceT>
	std::enable_if_t<std::is_same_v<ComputeShader, ResourceT>,
		shared_ptr<ComputeShader>> getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode);

	template<typename ResourceT>
	std::enable_if_t<std::is_same_v<DomainShader, ResourceT>,
		shared_ptr<DomainShader>> getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode);

	template<typename ResourceT>
	std::enable_if_t<std::is_same_v<GeometryShader, ResourceT>,
		shared_ptr<GeometryShader>> getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode);

	template<typename ResourceT>
	std::enable_if_t<std::is_same_v<HullShader, ResourceT>,
		shared_ptr<HullShader>> getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode);

	template<typename ResourceT>
	std::enable_if_t<std::is_same_v<PixelShader, ResourceT>,
		shared_ptr<PixelShader>> getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode);

	template<typename ResourceT>
	std::enable_if_t<std::is_same_v<VertexShader, ResourceT>,
		shared_ptr<VertexShader>> getOrCreate(const std::wstring& guid,
		                                      const ShaderBytecode& bytecode,
		                                      const D3D11_INPUT_ELEMENT_DESC* input_element_descs,
		                                      u32 input_element_count);


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
