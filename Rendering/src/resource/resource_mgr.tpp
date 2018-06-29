#include "directx/directx_math.h"


// ModelBlueprint
template<typename ResourceT>
enable_if_t<is_same_v<ModelBlueprint, ResourceT>,
shared_ptr<ModelBlueprint>> ResourceMgr::getOrCreate(const wstring& filename) {

	return models.getOrCreateResource(filename, device, *this, filename);
}

// ModelBlueprint
template<typename ResourceT, typename VertexT>
enable_if_t<is_same_v<ModelBlueprint, ResourceT>,
shared_ptr<ModelBlueprint>> ResourceMgr::getOrCreate(const wstring& name, const ModelOutput<VertexT>& model_data) {

	return models.getOrCreateResource(name, device, model_data);
}


// Texture
template<typename ResourceT>
enable_if_t<is_same_v<Texture, ResourceT>,
shared_ptr<Texture>> ResourceMgr::getOrCreate(const wstring& filename) {

	return textures.getOrCreateResource(filename, device, device_context, filename);
}

template<typename ResourceT>
enable_if_t<is_same_v<Texture, ResourceT>,
shared_ptr<Texture>> ResourceMgr::getOrCreate(const vec4_f32& color) {

	u32 texColor = Float4ColorToU32(color);

	return textures.getOrCreateResource(to_wstring(texColor), device, texColor);
}


// Font
template<typename ResourceT>
enable_if_t<is_same_v<Font, ResourceT>,
shared_ptr<Font>> ResourceMgr::getOrCreate(const wstring& filename) {

	return fonts.getOrCreateResource(filename, device, filename.c_str());
}
