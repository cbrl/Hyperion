#pragma once

#include "util\datatypes\datatypes.h"
#include "resource\mesh\mesh.h"
#include "resource\model\model_blueprint.h"
#include "resource\model\model_output.h"
#include "resource\texture\texture.h"
#include <SpriteFont.h>


class ModelBlueprint;


template<typename KeyT, typename ValueT, template<typename...> typename MapT>
class IResourceMap final {
	public:
		IResourceMap() = default;
		~IResourceMap() = default;

		template<typename... ArgsT>
		shared_ptr<ValueT> GetOrCreate(const KeyT& key, ArgsT&&... args);

		shared_ptr<ValueT> Get(const KeyT& key);


	private:
		MapT<KeyT, weak_ptr<ValueT>> resource_map;
};


template<typename KeyT, typename ValueT>
using ResourceMap = IResourceMap<KeyT, ValueT, std::map>;

template<typename KeyT, typename ValueT>
using UnorderedResourceMap = IResourceMap<KeyT, ValueT, std::unordered_map>;



class ResourceMgr final {
	public:
		ResourceMgr(ID3D11Device* device, ID3D11DeviceContext* device_context)
			: device(device)
			, device_context(device_context)
		{}

		~ResourceMgr() = default;


		// ModelBlueprint
		template<typename ResourceT>
		enable_if_t<is_same_v<ModelBlueprint, ResourceT>, shared_ptr<ModelBlueprint>> Create(const wstring& filename);


		// Texture
		template<typename ResourceT>
		enable_if_t<is_same_v<Texture, ResourceT>, shared_ptr<Texture>> Create(const wstring& filename);

		template<typename ResourceT>
		enable_if_t<is_same_v<Texture, ResourceT>, shared_ptr<Texture>> Create(const float4& color);


		// SpriteFont
		template<typename ResourceT>
		enable_if_t<is_same_v<SpriteFont, ResourceT>, shared_ptr<SpriteFont>> Create(const wstring& filename);


	private:
		// Device and device context
		ComPtr<ID3D11Device>        device;
		ComPtr<ID3D11DeviceContext> device_context;

		// Models
		ResourceMap<wstring, ModelBlueprint> models;

		// Textures
		ResourceMap<wstring, Texture> textures;

		// SpriteFonts
		ResourceMap<wstring, SpriteFont> fonts;
};


#include "resource_mgr.tpp"