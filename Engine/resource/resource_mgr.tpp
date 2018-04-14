#pragma once

#include "util\math\math.h"


//----------------------------------------------------------------------------------
// ResourceMap
//----------------------------------------------------------------------------------

template<typename KeyT, typename ValueT>
template<typename... ArgsT>
shared_ptr<ValueT> ResourceMap<KeyT, ValueT>::GetOrCreate(const KeyT& key, ArgsT&&... args) {

	// Find the resource
	const auto n = resource_map.find(key);

	// Check if the resource exists
	if (n != resource_map.end()) {
		
		// Return the resource if it hasn't expired
		if (!n->second.expired()) {
			return n->second.lock();
		}
		// If the resource has expired then remove the ptr from the map
		else {
			resource_map.erase(n);
		}
	}

	// Create the resource if it doesn't exist or expired
	const auto resource = shared_ptr<ValueT>(new ValueT(std::forward<ArgsT>(args)...));
	resource_map[key] = resource;

	return resource;
}


template<typename KeyT, typename ValueT>
shared_ptr<ValueT> ResourceMap<KeyT, ValueT>::Get(const KeyT& key) {

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


// Texture2DArray
template<typename ResourceT>
enable_if_t<is_same_v<Texture, ResourceT>, shared_ptr<Texture>> ResourceMgr::Create(const vector<wstring>& filenames) {

	wstring key;
	for (const wstring& fn : filenames) {
		key += fn;
	}

	return textures.GetOrCreate(key, device.Get(), device_context.Get(), filenames);
}


// SpriteFont
template<typename ResourceT>
enable_if_t<is_same_v<SpriteFont, ResourceT>, shared_ptr<SpriteFont>> ResourceMgr::Create(const wstring& filename) {

	return fonts.GetOrCreate(filename, device.Get(), filename.c_str());
}