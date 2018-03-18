#pragma once


//----------------------------------------------------------------------------------
// ResourceMap
//----------------------------------------------------------------------------------

template<typename KeyT, typename ValueT>
template<typename... ArgsT>
shared_ptr<ValueT> ResourceMap<KeyT, ValueT>::Create(const KeyT& key, ArgsT&&... args) {

	if (resource_map.find(key) != resource_map.end()) {
		return resource_map.at(key).lock();
	}
	else {
		auto resource = shared_ptr<ValueT>(new ValueT(std::forward<ArgsT>(args)...));
		resource_map[key] = resource;
		return resource;
	}
}


template<typename KeyT, typename ValueT>
shared_ptr<ValueT> ResourceMap<KeyT, ValueT>::Get(const KeyT& key) {
	return resource_map.at(key).lock();
}



//----------------------------------------------------------------------------------
// ResourceMgr
//----------------------------------------------------------------------------------

template<typename ResourceT>
enable_if_t<is_same_v<ModelBlueprint, ResourceT>, shared_ptr<ModelBlueprint>> ResourceMgr::Create(const wstring& folder, const wstring& filename) {
	return models.Create(folder + filename, device.Get(), *this, folder, filename);
}


template<typename ResourceT>
enable_if_t<is_same_v<Texture, ResourceT>, shared_ptr<Texture>> ResourceMgr::Create(const wstring& filename) {
	return textures.Create(filename, device.Get(), device_context.Get(), filename);
}


template<typename ResourceT>
enable_if_t<is_same_v<Texture, ResourceT>, shared_ptr<Texture>> ResourceMgr::Create(const vector<wstring>& filenames) {
	return texture_arrays.Create(filename, device.Get(), device_context.Get(), filenames);
}


template<typename ResourceT>
enable_if_t<is_same_v<Texture, ResourceT>, shared_ptr<Texture>> ResourceMgr::Create(const float4& color) {

	// Convert the float4 into a single hex color value
	u32 texColor = (u32)(color.x * 0xff) + ((u32)(color.y * 0xff) << 8) + ((u32)(color.z * 0xff) << 16) + ((u32)(color.w * 0xff) << 24);

	return color_textures.Create(texColor, device.Get(), texColor);
}


template<typename ResourceT>
enable_if_t<is_same_v<Text, ResourceT>, shared_ptr<Text>> ResourceMgr::Create(const wstring& label, const wstring& font) {
	return texts.Create(label, device.Get(), device_context.Get(), font);
}