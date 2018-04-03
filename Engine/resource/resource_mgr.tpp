#pragma once

#include "util\math\math.h"


//----------------------------------------------------------------------------------
// ResourceMap
//----------------------------------------------------------------------------------

template<typename KeyT, typename ValueT>
template<typename... ArgsT>
shared_ptr<ValueT> ResourceMap<KeyT, ValueT>::Create(const KeyT& key, ArgsT&&... args) {

	const auto n = resource_map.find(key);

	if (n != resource_map.end()) {
		
		if (!n->second.expired()) {
			return n->second.lock();
		}
		else {
			resource_map.erase(n);
		}
	}

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
enable_if_t<is_same_v<ModelBlueprint, ResourceT>, shared_ptr<ModelBlueprint>> ResourceMgr::Create(const wstring& folder, const wstring& filename) {

	return models.Create(folder + filename, device.Get(), *this, folder, filename);
}


// Texture
template<typename ResourceT>
enable_if_t<is_same_v<Texture, ResourceT>, shared_ptr<Texture>> ResourceMgr::Create(const wstring& filename) {

	return textures.Create(filename, device.Get(), device_context.Get(), filename);
}


// Texture2DArray
template<typename ResourceT>
enable_if_t<is_same_v<Texture, ResourceT>, shared_ptr<Texture>> ResourceMgr::Create(const vector<wstring>& filenames) {

	return texture_arrays.Create(filename, device.Get(), device_context.Get(), filenames);
}


// Texture
template<typename ResourceT>
enable_if_t<is_same_v<Texture, ResourceT>, shared_ptr<Texture>> ResourceMgr::Create(const float4& color) {

	u32 texColor = Float4ColorToU32(color);

	return color_textures.Create(texColor, device.Get(), texColor);
}


// SpriteFont
template<typename ResourceT>
enable_if_t<is_same_v<SpriteFont, ResourceT>, shared_ptr<SpriteFont>> ResourceMgr::Create(const wstring& filename) {

	return fonts.Create(filename, device.Get(), filename.c_str());
}