#include "directx/directx_math.h"


//----------------------------------------------------------------------------------
// ModelBlueprint
//----------------------------------------------------------------------------------

template<typename ResourceT>
enable_if_t<is_same_v<ModelBlueprint, ResourceT>,
	shared_ptr<ModelBlueprint>> ResourceMgr::getOrCreate(const std::wstring& filename) {

	return models.getOrCreateResource(filename, device, *this, filename);
}

template<typename ResourceT, typename VertexT>
enable_if_t<is_same_v<ModelBlueprint, ResourceT>,
	shared_ptr<ModelBlueprint>> ResourceMgr::getOrCreate(const std::wstring& name,
	                                                     const ModelOutput<VertexT>& model_data) {

	return models.getOrCreateResource(name, device, model_data);
}



//----------------------------------------------------------------------------------
// Texture
//----------------------------------------------------------------------------------

template<typename ResourceT>
enable_if_t<is_same_v<Texture, ResourceT>,
	shared_ptr<Texture>> ResourceMgr::getOrCreate(const std::wstring& filename) {

	return textures.getOrCreateResource(filename, device, device_context, filename);
}

template<typename ResourceT>
enable_if_t<is_same_v<Texture, ResourceT>,
	shared_ptr<Texture>> ResourceMgr::getOrCreate(const vec4_f32& color) {

	u32 texColor = Float4ColorToU32(color);

	return textures.getOrCreateResource(std::to_wstring(texColor), device, texColor);
}



//----------------------------------------------------------------------------------	
// Font
//----------------------------------------------------------------------------------

template<typename ResourceT>
enable_if_t<is_same_v<Font, ResourceT>,
	shared_ptr<Font>> ResourceMgr::getOrCreate(const std::wstring& filename) {

	return fonts.getOrCreateResource(filename, device, filename.c_str());
}



//----------------------------------------------------------------------------------
// Shader
//----------------------------------------------------------------------------------

template<typename ResourceT>
enable_if_t<is_same_v<ComputeShader, ResourceT>,
	shared_ptr<ComputeShader>> ResourceMgr::getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode) {
	
	return compute_shaders.getOrCreateResource(guid, guid, device, bytecode);
}

template<typename ResourceT>
enable_if_t<is_same_v<DomainShader, ResourceT>,
	shared_ptr<DomainShader>> ResourceMgr::getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode) {
	
	return domain_shaders.getOrCreateResource(guid, guid, device, bytecode);
}

template<typename ResourceT>
enable_if_t<is_same_v<GeometryShader, ResourceT>,
	shared_ptr<GeometryShader>> ResourceMgr::getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode) {
	
	return geometry_shaders.getOrCreateResource(guid, guid, device, bytecode);
}

template<typename ResourceT>
enable_if_t<is_same_v<HullShader, ResourceT>,
	shared_ptr<HullShader>> ResourceMgr::getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode) {
	
	return hull_shaders.getOrCreateResource(guid, guid, device, bytecode);
}

template<typename ResourceT>
enable_if_t<is_same_v<PixelShader, ResourceT>,
	shared_ptr<PixelShader>> ResourceMgr::getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode) {
	
	return pixel_shaders.getOrCreateResource(guid, guid, device, bytecode);
}

template<typename ResourceT>
enable_if_t<is_same_v<VertexShader, ResourceT>,
	shared_ptr<VertexShader>> ResourceMgr::getOrCreate(const std::wstring& guid,
										  const ShaderBytecode& bytecode,
										  const D3D11_INPUT_ELEMENT_DESC* input_element_descs,
										  u32 input_element_count) {
	
	return vertex_shaders.getOrCreateResource(guid, guid, device, bytecode, input_element_descs, input_element_count);
}