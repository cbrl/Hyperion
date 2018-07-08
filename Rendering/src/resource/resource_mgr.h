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
	enable_if_t<is_same_v<ModelBlueprint, ResourceT>,
		shared_ptr<ModelBlueprint>> getOrCreate(const wstring& filename);

	template<typename ResourceT, typename VertexT>
	enable_if_t<is_same_v<ModelBlueprint, ResourceT>,
		shared_ptr<ModelBlueprint>> getOrCreate(const wstring& name,
		                                        const ModelOutput<VertexT>& model_data);


	//----------------------------------------------------------------------------------
	// Member Functions - Texture
	//----------------------------------------------------------------------------------

	template<typename ResourceT>
	enable_if_t<is_same_v<Texture, ResourceT>,
		shared_ptr<Texture>> getOrCreate(const wstring& filename);

	template<typename ResourceT>
	enable_if_t<is_same_v<Texture, ResourceT>,
		shared_ptr<Texture>> getOrCreate(const vec4_f32& color);


	//----------------------------------------------------------------------------------
	// Member Functions - Font
	//----------------------------------------------------------------------------------

	template<typename ResourceT>
	enable_if_t<is_same_v<Font, ResourceT>,
		shared_ptr<Font>> getOrCreate(const wstring& filename);


	//----------------------------------------------------------------------------------
	// Member Functions - Shader
	//----------------------------------------------------------------------------------

	template<typename ResourceT>
	enable_if_t<is_same_v<ComputeShader, ResourceT>,
		shared_ptr<ComputeShader>> getOrCreate(const wstring& guid, const ShaderBytecode& bytecode);

	template<typename ResourceT>
	enable_if_t<is_same_v<DomainShader, ResourceT>,
		shared_ptr<DomainShader>> getOrCreate(const wstring& guid, const ShaderBytecode& bytecode);

	template<typename ResourceT>
	enable_if_t<is_same_v<GeometryShader, ResourceT>,
		shared_ptr<GeometryShader>> getOrCreate(const wstring& guid, const ShaderBytecode& bytecode);

	template<typename ResourceT>
	enable_if_t<is_same_v<HullShader, ResourceT>,
		shared_ptr<HullShader>> getOrCreate(const wstring& guid, const ShaderBytecode& bytecode);

	template<typename ResourceT>
	enable_if_t<is_same_v<PixelShader, ResourceT>,
		shared_ptr<PixelShader>> getOrCreate(const wstring& guid, const ShaderBytecode& bytecode);

	template<typename ResourceT>
	enable_if_t<is_same_v<VertexShader, ResourceT>,
		shared_ptr<VertexShader>> getOrCreate(const wstring& guid,
		                                      const ShaderBytecode& bytecode,
		                                      const D3D11_INPUT_ELEMENT_DESC* input_element_descs,
		                                      u32 input_element_count);


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Device and device context
	reference_wrapper<ID3D11Device> device;
	reference_wrapper<ID3D11DeviceContext> device_context;

	// Resources
	ManagedResourceMap<wstring, ModelBlueprint> models;
	ManagedResourceMap<wstring, Texture> textures;
	ManagedResourceMap<wstring, Font> fonts;

	ManagedResourceMap<wstring, ComputeShader>  compute_shaders;
	ManagedResourceMap<wstring, DomainShader>   domain_shaders;
	ManagedResourceMap<wstring, GeometryShader> geometry_shaders;
	ManagedResourceMap<wstring, HullShader>     hull_shaders;
	ManagedResourceMap<wstring, PixelShader>    pixel_shaders;
	ManagedResourceMap<wstring, VertexShader>   vertex_shaders;
};


#include "resource_mgr.tpp"
