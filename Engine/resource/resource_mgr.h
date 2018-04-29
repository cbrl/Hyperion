#pragma once

#include "util\datatypes\datatypes.h"
#include "util\memory\resource_pool.h"
#include "resource\resource.h"
#include "resource\mesh\mesh.h"
#include "resource\model\model_blueprint.h"
#include "resource\model\model_output.h"
#include "resource\texture\texture.h"
#include <SpriteFont.h>


class ModelBlueprint;



//----------------------------------------------------------------------------------
// ResourcePtr
//----------------------------------------------------------------------------------
//
// Wraps the raw pointer to a resource, and a pointer to the
// resource manager that created it. Automatically frees the
// resource and its memory when the ResourcePtr is deleted.
//
//----------------------------------------------------------------------------------

template<typename T>
class ResourcePtr final {
	friend class ResourceMgr;

	public:
		ResourcePtr(ResourceMgr* const res_mgr, T* data_ptr)
			: data(data_ptr)
			, mgr(res_mgr) {
		}
		~ResourcePtr() {
			mgr->Destroy(data);
			data = nullptr;
		}

		T* operator->() {
			return data;
		}
		T& operator*() {
			return *data;
		}

	private:
		T* data;
		ResourceMgr* const mgr;
};





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







	public:
		template<typename ResourceT, typename... ArgsT>
		ResourcePtr<ResourceT> GetOrCreate(const wstring& key, ArgsT&&... args);

		template<typename ResourceT>
		void Destroy(ResourceT* resource);


	private:
		ResourcePoolMap resource_pools;
		unordered_map<type_index, unordered_map<wstring, IResource*>> resources;
};


#include "resource_mgr.tpp"