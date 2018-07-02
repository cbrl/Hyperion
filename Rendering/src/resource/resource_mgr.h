#pragma once

#include "datatypes/datatypes.h"
#include "memory/managed_resource_map.h"
#include "resource/resource.h"
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
	// Member Functions
	//----------------------------------------------------------------------------------

	// ModelBlueprint
	template<typename ResourceT>
	enable_if_t<is_same_v<ModelBlueprint, ResourceT>, shared_ptr<ModelBlueprint>> getOrCreate(const wstring& filename);

	template<typename ResourceT, typename VertexT>
	enable_if_t<is_same_v<ModelBlueprint, ResourceT>, shared_ptr<ModelBlueprint>> getOrCreate(const wstring& name,
	                                                                                          const ModelOutput<VertexT>& model_data);


	// Texture
	template<typename ResourceT>
	enable_if_t<is_same_v<Texture, ResourceT>, shared_ptr<Texture>> getOrCreate(const wstring& filename);

	template<typename ResourceT>
	enable_if_t<is_same_v<Texture, ResourceT>, shared_ptr<Texture>> getOrCreate(const vec4_f32& color);


	// SpriteFont
	template<typename ResourceT>
	enable_if_t<is_same_v<Font, ResourceT>, shared_ptr<Font>> getOrCreate(const wstring& filename);


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
};


#include "resource_mgr.tpp"
