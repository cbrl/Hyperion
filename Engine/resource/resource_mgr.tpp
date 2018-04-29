#pragma once

#include "util\math\math.h"
#include "resource_mgr.h"


//----------------------------------------------------------------------------------
// IResourceMap
//----------------------------------------------------------------------------------

template<typename KeyT, typename ValueT, template<typename ...> typename MapT>
template<typename... ArgsT>
shared_ptr<ValueT> IResourceMap<KeyT, ValueT, MapT>::GetOrCreate(const KeyT& key, ArgsT&&... args) {

	// Find the resource
	const auto it = resource_map.find(key);

	// Check if the resource exists
	if (it != resource_map.end()) {
		
		// Return the resource if it hasn't expired
		if (!it->second.expired()) {
			return it->second.lock();
		}
		// If the resource has expired then remove the ptr from the map
		else {
			resource_map.erase(it);
		}
	}

	// Create the resource if it doesn't exist or expired
	const auto resource = shared_ptr<ValueT>(new ValueT(std::forward<ArgsT>(args)...));
	resource_map[key] = resource;

	return resource;
}


template<typename KeyT, typename ValueT, template<typename ...> typename MapT>
shared_ptr<ValueT> IResourceMap<KeyT, ValueT, MapT>::Get(const KeyT& key) {

	const auto n = resource_map.find(key);

	if (n != resource_map.end()) {

		if (!n->second.expired()) {
			return n->second.lock();
		}
		else {
			resource_map.erase(n);
		}
	}
	
	return nullptr;
}



//----------------------------------------------------------------------------------
// ResourceMgr
//----------------------------------------------------------------------------------

// ModelBlueprint
template<typename ResourceT>
enable_if_t<is_same_v<ModelBlueprint, ResourceT>, shared_ptr<ModelBlueprint>> ResourceMgr::Create(const wstring& filename) {

	return models.GetOrCreate(filename, device.Get(), *this, filename);
}


// Texture
template<typename ResourceT>
enable_if_t<is_same_v<Texture, ResourceT>, shared_ptr<Texture>> ResourceMgr::Create(const wstring& filename) {

	return textures.GetOrCreate(filename, device.Get(), device_context.Get(), filename);
}

template<typename ResourceT>
enable_if_t<is_same_v<Texture, ResourceT>, shared_ptr<Texture>> ResourceMgr::Create(const float4& color) {

	u32 texColor = Float4ColorToU32(color);

	return textures.GetOrCreate(to_wstring(texColor), device.Get(), texColor);
}


// SpriteFont
template<typename ResourceT>
enable_if_t<is_same_v<SpriteFont, ResourceT>, shared_ptr<SpriteFont>> ResourceMgr::Create(const wstring& filename) {

	return fonts.GetOrCreate(filename, device.Get(), filename.c_str());
}






template<typename ResourceT, typename... ArgsT>
ResourcePtr<ResourceT> ResourceMgr::GetOrCreate(const wstring& key, ArgsT&&... args) {

	// Get the map for this resource type
	const auto map_it = resources.find(ResourceT::type_id);

	unordered_map<wstring, IResource*> map;

	if (map_it == resources.end()) {
		map = resources[ResourceT::type_id];
	}
	else {
		map = map_it->second;
	}


	// Locate the resource referred to by this key, if it exists
	auto resource_it = map.find(key);

	// Create the resource if it doesn't exist
	if (resource_it == map.end()) {
		auto pool = resource_pools.GetOrCreatePool<ResourceT>();

		auto memory = pool->CreateObject();

		auto resource = new(memory) ResourceT(std::forward<ArgsT>(args)...);

		map[key] = resource;

		return ResourcePtr<ResourceT>(this, resource);
	}

	// Return the resource if it does exist
	return ResourcePtr<ResourceT>(this, static_cast<ResourceT*>(resource_it->second));
}


template<typename ResourceT>
void ResourceMgr::Destroy(ResourceT* resource) {

	// Get the map for this resource type
	const auto map_it = resources.find(ResourceT::type_id);
	if (map_it == resources.end()) {
		// Log, throw, etc
		return;
	}


	// Get the resource referred to by the guid
	auto& map = map_it->second;
	auto resource_it = map.find(resource->GetGUID());

	if (resource_it == map.end()) {
		// Log, throw, etc
		return;
	}

	// Erase the entry
	map.erase(resource_it);


	// Get the resource pool
	auto pool = resource_pools.GetPool(ResourceT::type_id);
	if (pool == nullptr) {
		// Log, throw, etc
		return;
	}

	// Free the memory occupied by the resource
	pool->DestroyObject(reinterpret_cast<void*>(resource));
}