#pragma once

#include "util\datatypes\datatypes.h"
#include "resource\mesh\mesh.h"
#include "resource\model\model_blueprint.h"
#include "resource\texture\texture.h"
#include "resource\text\text.h"


class ModelBlueprint;


template<typename KeyT, typename ValueT>
class ResourceMap {
	public:
	ResourceMap() = default;
	~ResourceMap() = default;

	template<typename... ArgsT>
	shared_ptr<ValueT> Create(const KeyT& key, ArgsT&&... args);

	shared_ptr<ValueT> Get(const KeyT& key);


	private:
	map<KeyT, weak_ptr<ValueT>> resource_map;
};


class ResourceMgr {
	public:
		ResourceMgr() = delete;

		ResourceMgr(ID3D11Device* device, ID3D11DeviceContext* device_context)
			: device(device)
			, device_context(device_context)
		{}

		~ResourceMgr() = default;

		template<typename ResourceT>
		enable_if_t<is_same_v<ModelBlueprint, ResourceT>, shared_ptr<ModelBlueprint>> Create(const wstring& folder, const wstring& filename);

		template<typename ResourceT>
		enable_if_t<is_same_v<Texture, ResourceT>, shared_ptr<Texture>> Create(const wstring& filename);

		template<typename ResourceT>
		enable_if_t<is_same_v<Texture, ResourceT>, shared_ptr<Texture>> Create(const vector<wstring>& filenames);

		template<typename ResourceT>
		enable_if_t<is_same_v<Texture, ResourceT>, shared_ptr<Texture>> Create(const float4& color);

		template<typename ResourceT>
		enable_if_t<is_same_v<Text, ResourceT>, shared_ptr<Text>> Create(const wstring& label, const wstring& font);


	private:
		// Device and device context
		ComPtr<ID3D11Device>        device;
		ComPtr<ID3D11DeviceContext> device_context;

		// Models
		ResourceMap<wstring, ModelBlueprint> models;

		// Textures
		ResourceMap<wstring, Texture>         textures;
		ResourceMap<u32, Texture>             color_textures;
		ResourceMap<vector<wstring>, Texture> texture_arrays;

		// Text
		ResourceMap<wstring, Text> texts;
};


#include "resource_mgr.tpp"