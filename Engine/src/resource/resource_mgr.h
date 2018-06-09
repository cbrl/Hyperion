#pragma once

#include "datatypes/datatypes.h"
#include "resource/resource.h"
#include "resource/mesh/mesh.h"
#include "resource/model/model_blueprint.h"
#include "resource/model/model_output.h"
#include "resource/texture/texture.h"
#include "resource/font/font.h"


class ModelBlueprint;


class IResourceMap {
public:
	IResourceMap() = default;
	virtual ~IResourceMap() = default;
};


template<typename KeyT, typename ValueT>
class ResourceMap final : public IResourceMap {
public:
	ResourceMap() = default;
	~ResourceMap() = default;

	template<typename... ArgsT>
	shared_ptr<ValueT> getOrCreateResource(const KeyT& key, ArgsT&&... args);

	shared_ptr<ValueT> getResource(const KeyT& key);


private:
	unordered_map<KeyT, weak_ptr<ValueT>> resource_map;
};


template<typename ResMapKeyT>
class ResourceMapFactory final {
public:
	ResourceMapFactory() = default;

	~ResourceMapFactory() {
		for (auto& [key, val] : resource_maps) {
			delete val;
			val = nullptr;
		}
	}

	template<typename ResourceT>
	ResourceMap<ResMapKeyT, ResourceT>* getOrCreateMap() {

		using map_t = ResourceMap<ResMapKeyT, ResourceT>;

		const auto it = resource_maps.find(ResourceT::type_id);

		if (it == resource_maps.end()) {
			resource_maps[ResourceT::type_id] = new map_t();
			return static_cast<map_t*>(resource_maps[ResourceT::type_id]);
		}

		return static_cast<map_t*>(it->second);
	}

	template<typename ResourceT>
	ResourceMap<ResMapKeyT, ResourceT>* getMap() {

		using map_t = ResourceMap<ResMapKeyT, ResourceT>;

		const auto it = resource_maps.find(ResourceT::type_id);

		if (it == resource_maps.end()) {
			return nullptr;
		}

		return static_cast<map_t*>(it->second);
	}


private:
	unordered_map<type_index, IResourceMap*> resource_maps;
};


class ResourceMgr final {
public:
	ResourceMgr(ID3D11Device& device, ID3D11DeviceContext& device_context)
		: device(device)
		, device_context(device_context) {
	}

	~ResourceMgr() = default;


	// ModelBlueprint
	template<typename ResourceT>
	enable_if_t<is_same_v<ModelBlueprint, ResourceT>, shared_ptr<ModelBlueprint>> getOrCreate(const wstring& filename);

	template<typename ResourceT, typename VertexT>
	enable_if_t<is_same_v<ModelBlueprint, ResourceT>, shared_ptr<ModelBlueprint>> getOrCreate(
		const wstring& name,
		const ModelOutput<VertexT>& model_data);


	// Texture
	template<typename ResourceT>
	enable_if_t<is_same_v<Texture, ResourceT>, shared_ptr<Texture>> getOrCreate(const wstring& filename);

	template<typename ResourceT>
	enable_if_t<is_same_v<Texture, ResourceT>, shared_ptr<Texture>> getOrCreate(const float4& color);


	// SpriteFont
	template<typename ResourceT>
	enable_if_t<is_same_v<Font, ResourceT>, shared_ptr<Font>> getOrCreate(const wstring& filename);


private:
	// Device and device context
	reference_wrapper<ID3D11Device> device;
	reference_wrapper<ID3D11DeviceContext> device_context;

	// Resources
	ResourceMap<wstring, ModelBlueprint> models;
	ResourceMap<wstring, Texture> textures;
	ResourceMap<wstring, Font> fonts;
};


#include "resource_mgr.tpp"
