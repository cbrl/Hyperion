
//----------------------------------------------------------------------------------
// ModelBlueprint
//----------------------------------------------------------------------------------

template<typename ResourceT, typename VertexT>
auto ResourceMgr::acquire(const std::wstring& filename, const ModelConfig<VertexT>& config)
    -> std::enable_if_t<std::is_same_v<ModelBlueprint, ResourceT>, std::shared_ptr<ModelBlueprint>> {

	return models.acquire(filename, device, *this, filename, config);
}

template<typename ResourceT, typename VertexT>
auto ResourceMgr::acquire(const std::wstring& name,
	                          const ModelOutput& model_data,
	                          const ModelConfig<VertexT>& config)
    -> std::enable_if_t<std::is_same_v<ModelBlueprint, ResourceT>, std::shared_ptr<ModelBlueprint>> {

	return models.acquire(name, device, model_data, config);
}



//----------------------------------------------------------------------------------
// Texture
//----------------------------------------------------------------------------------

template<typename ResourceT>
auto ResourceMgr::acquire(const std::wstring& filename)
    -> std::enable_if_t<std::is_same_v<Texture, ResourceT>, std::shared_ptr<Texture>> {

	return textures.acquire(filename, device, device_context, filename);
}

template<typename ResourceT>
auto ResourceMgr::acquire(const std::wstring& name,
	                          const D3D11_TEXTURE2D_DESC& desc, 
	                          const D3D11_SUBRESOURCE_DATA& init_data)
    -> std::enable_if_t<std::is_same_v<Texture, ResourceT>, std::shared_ptr<Texture>> {

	return textures.acquire(name, name, device, desc, init_data);
}



//----------------------------------------------------------------------------------	
// Font
//----------------------------------------------------------------------------------

template<typename ResourceT>
auto ResourceMgr::acquire(const std::wstring& filename)
    -> std::enable_if_t<std::is_same_v<Font, ResourceT>, std::shared_ptr<Font>> {

	return fonts.acquire(filename, device, filename.c_str());
}



//----------------------------------------------------------------------------------
// Shader
//----------------------------------------------------------------------------------

template<typename ResourceT>
auto ResourceMgr::acquire(const std::wstring& guid, const ShaderBytecode& bytecode)
    -> std::enable_if_t<std::is_same_v<ComputeShader, ResourceT>, std::shared_ptr<ComputeShader>> {
	
	return compute_shaders.acquire(guid, guid, device, bytecode);
}

template<typename ResourceT>
auto ResourceMgr::acquire(const std::wstring& guid, const ShaderBytecode& bytecode)
    -> std::enable_if_t<std::is_same_v<DomainShader, ResourceT>, std::shared_ptr<DomainShader>> {
	
	return domain_shaders.acquire(guid, guid, device, bytecode);
}

template<typename ResourceT>
auto ResourceMgr::acquire(const std::wstring& guid, const ShaderBytecode& bytecode)
    -> std::enable_if_t<std::is_same_v<GeometryShader, ResourceT>, std::shared_ptr<GeometryShader>> {
	
	return geometry_shaders.acquire(guid, guid, device, bytecode);
}

template<typename ResourceT>
auto ResourceMgr::acquire(const std::wstring& guid, const ShaderBytecode& bytecode)
    -> std::enable_if_t<std::is_same_v<HullShader, ResourceT>, std::shared_ptr<HullShader>> {
	
	return hull_shaders.acquire(guid, guid, device, bytecode);
}

template<typename ResourceT>
auto ResourceMgr::acquire(const std::wstring& guid, const ShaderBytecode& bytecode)
    -> std::enable_if_t<std::is_same_v<PixelShader, ResourceT>, std::shared_ptr<PixelShader>> {
	
	return pixel_shaders.acquire(guid, guid, device, bytecode);
}

template<typename ResourceT>
auto ResourceMgr::acquire(const std::wstring& guid,
	                          const ShaderBytecode& bytecode,
	                          gsl::span<const D3D11_INPUT_ELEMENT_DESC> input_element_descs)
    -> std::enable_if_t<std::is_same_v<VertexShader, ResourceT>, std::shared_ptr<VertexShader>> {
	
	return vertex_shaders.acquire(guid, guid, device, bytecode, input_element_descs);
}