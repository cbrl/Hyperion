#pragma once

#include "math/math.h"
#include "resource_mgr.h"


//----------------------------------------------------------------------------------
// ResourceMap
//----------------------------------------------------------------------------------

template<typename KeyT, typename ValueT>
template<typename... ArgsT>
shared_ptr<ValueT> ResourceMap<KeyT, ValueT>::GetOrCreateResource(const KeyT& key, ArgsT&&... args) {

	// Find the resource
	const auto it = resource_map.find(key);

	// Check if the resource exists
	if (it != resource_map.end()) {

		// Return the resource if it hasn't expired
		if (!it->second.expired()) {
			return it->second.lock();
		}
		// If the resource has expired then remove the ptr from the map
		resource_map.erase(it);
	}

	// Create the resource if it doesn't exist or expired
	const auto resource = std::make_shared<ValueT>(std::forward<ArgsT>(args)...);
	resource_map[key] = resource;

	return resource;
}


template<typename KeyT, typename ValueT>
shared_ptr<ValueT> ResourceMap<KeyT, ValueT>::GetResource(const KeyT& key) {

	const auto n = resource_map.find(key);

	if (n != resource_map.end()) {

		if (!n->second.expired()) {
			return n->second.lock();
		}
		resource_map.erase(n);
	}

	return nullptr;
}


//----------------------------------------------------------------------------------
// ResourceMgr
//----------------------------------------------------------------------------------

// ModelBlueprint
template<typename ResourceT>
enable_if_t<is_same_v<ModelBlueprint, ResourceT>, shared_ptr<ModelBlueprint>> ResourceMgr::GetOrCreate(
	const wstring& filename) {

	return models.GetOrCreateResource(filename, device, *this, filename);
}

// ModelBlueprint
template<typename ResourceT, typename VertexT>
enable_if_t<is_same_v<ModelBlueprint, ResourceT>, shared_ptr<ModelBlueprint>> ResourceMgr::GetOrCreate(
	const wstring& name,
	const ModelOutput<VertexT>& model_data) {

	return models.GetOrCreateResource(name, device, model_data);
}


// Texture
template<typename ResourceT>
enable_if_t<is_same_v<Texture, ResourceT>, shared_ptr<Texture>> ResourceMgr::GetOrCreate(const wstring& filename) {

	return textures.GetOrCreateResource(filename, device, device_context, filename);
}

template<typename ResourceT>
enable_if_t<is_same_v<Texture, ResourceT>, shared_ptr<Texture>> ResourceMgr::GetOrCreate(const float4& color) {

	u32 texColor = Float4ColorToU32(color);

	return textures.GetOrCreateResource(to_wstring(texColor), device, texColor);
}


// Font
template<typename ResourceT>
enable_if_t<is_same_v<Font, ResourceT>, shared_ptr<Font>> ResourceMgr::GetOrCreate(const wstring& filename) {

	return fonts.GetOrCreateResource(filename, device, filename.c_str());
}
