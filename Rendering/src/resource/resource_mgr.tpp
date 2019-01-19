
//----------------------------------------------------------------------------------
// ModelBlueprint
//----------------------------------------------------------------------------------

template<typename ResourceT, typename VertexT>
auto ResourceMgr::getOrCreate(const std::wstring& filename, const ModelConfig<VertexT>& config)
    -> std::enable_if_t<std::is_same_v<ModelBlueprint, ResourceT>, std::shared_ptr<ModelBlueprint>> {

	return models.getOrCreateResource(filename, device, *this, filename, config);
}

template<typename ResourceT, typename VertexT>
auto ResourceMgr::getOrCreate(const std::wstring& name,
	                          const ModelOutput& model_data,
	                          const ModelConfig<VertexT>& config)
    -> std::enable_if_t<std::is_same_v<ModelBlueprint, ResourceT>, std::shared_ptr<ModelBlueprint>> {

	return models.getOrCreateResource(name, device, model_data, config);
}



//----------------------------------------------------------------------------------
// Texture
//----------------------------------------------------------------------------------

template<typename ResourceT>
auto ResourceMgr::getOrCreate(const std::wstring& filename)
    -> std::enable_if_t<std::is_same_v<Texture, ResourceT>, std::shared_ptr<Texture>> {

	return textures.getOrCreateResource(filename, device, device_context, filename);
}

template<typename ResourceT>
auto ResourceMgr::getOrCreate(const std::wstring& name,
	                          const D3D11_TEXTURE2D_DESC& desc, 
	                          const D3D11_SUBRESOURCE_DATA& init_data)
    -> std::enable_if_t<std::is_same_v<Texture, ResourceT>, std::shared_ptr<Texture>> {

	return textures.getOrCreateResource(name, name, device, desc, init_data);
}



//----------------------------------------------------------------------------------	
// Font
//----------------------------------------------------------------------------------

template<typename ResourceT>
auto ResourceMgr::getOrCreate(const std::wstring& filename)
    -> std::enable_if_t<std::is_same_v<Font, ResourceT>, std::shared_ptr<Font>> {

	return fonts.getOrCreateResource(filename, device, filename.c_str());
}



//----------------------------------------------------------------------------------
// Shader
//----------------------------------------------------------------------------------

template<typename ResourceT>
auto ResourceMgr::getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode)
    -> std::enable_if_t<std::is_same_v<ComputeShader, ResourceT>, std::shared_ptr<ComputeShader>> {
	
	return compute_shaders.getOrCreateResource(guid, guid, device, bytecode);
}

template<typename ResourceT>
auto ResourceMgr::getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode)
    -> std::enable_if_t<std::is_same_v<DomainShader, ResourceT>, std::shared_ptr<DomainShader>> {
	
	return domain_shaders.getOrCreateResource(guid, guid, device, bytecode);
}

template<typename ResourceT>
auto ResourceMgr::getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode)
    -> std::enable_if_t<std::is_same_v<GeometryShader, ResourceT>, std::shared_ptr<GeometryShader>> {
	
	return geometry_shaders.getOrCreateResource(guid, guid, device, bytecode);
}

template<typename ResourceT>
auto ResourceMgr::getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode)
    -> std::enable_if_t<std::is_same_v<HullShader, ResourceT>, std::shared_ptr<HullShader>> {
	
	return hull_shaders.getOrCreateResource(guid, guid, device, bytecode);
}

template<typename ResourceT>
auto ResourceMgr::getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode)
    -> std::enable_if_t<std::is_same_v<PixelShader, ResourceT>, std::shared_ptr<PixelShader>> {
	
	return pixel_shaders.getOrCreateResource(guid, guid, device, bytecode);
}

template<typename ResourceT>
auto ResourceMgr::getOrCreate(const std::wstring& guid,
	                          const ShaderBytecode& bytecode,
	                          gsl::span<const D3D11_INPUT_ELEMENT_DESC> input_element_descs)
    -> std::enable_if_t<std::is_same_v<VertexShader, ResourceT>, std::shared_ptr<VertexShader>> {
	
	return vertex_shaders.getOrCreateResource(guid, guid, device, bytecode, input_element_descs);
}