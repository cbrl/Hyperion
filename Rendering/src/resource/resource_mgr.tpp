
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

template<typename ResourceT>
auto ResourceMgr::getResourceMap() const
	-> std::enable_if_t<std::is_same_v<ModelBlueprint, ResourceT>, const resource_map<std::wstring, ModelBlueprint>&> {
	return models;
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

template<typename ResourceT>
auto ResourceMgr::getResourceMap() const
	-> std::enable_if_t<std::is_same_v<Texture, ResourceT>, const resource_map<std::wstring, Texture>&> {
	return textures;
}



//----------------------------------------------------------------------------------	
// Font
//----------------------------------------------------------------------------------

template<typename ResourceT>
auto ResourceMgr::acquire(const std::wstring& filename)
    -> std::enable_if_t<std::is_same_v<Font, ResourceT>, std::shared_ptr<Font>> {

	return fonts.acquire(filename, device, filename.c_str());
}

template<typename ResourceT>
auto ResourceMgr::getResourceMap() const
	-> std::enable_if_t<std::is_same_v<Font, ResourceT>, const resource_map<std::wstring, Font>&> {
	return fonts;
}


//----------------------------------------------------------------------------------
// Compute Shader
//----------------------------------------------------------------------------------

template<typename ResourceT>
auto ResourceMgr::acquire(const std::wstring& guid, const ShaderBytecode& bytecode)
    -> std::enable_if_t<std::is_same_v<ComputeShader, ResourceT>, std::shared_ptr<ComputeShader>> {
	
	return compute_shaders.acquire(guid, guid, device, bytecode);
}

template<typename ResourceT>
auto ResourceMgr::getResourceMap() const
    -> std::enable_if_t<std::is_same_v<ComputeShader, ResourceT>, const shader_resource_map<std::wstring, ComputeShader>&> {
	return compute_shaders;
}


//----------------------------------------------------------------------------------
// Domain Shader
//----------------------------------------------------------------------------------

template<typename ResourceT>
auto ResourceMgr::acquire(const std::wstring& guid, const ShaderBytecode& bytecode)
    -> std::enable_if_t<std::is_same_v<DomainShader, ResourceT>, std::shared_ptr<DomainShader>> {
	
	return domain_shaders.acquire(guid, guid, device, bytecode);
}

template<typename ResourceT>
auto ResourceMgr::getResourceMap() const
    -> std::enable_if_t<std::is_same_v<DomainShader, ResourceT>, const shader_resource_map<std::wstring, DomainShader>&> {
	return domain_shaders;
}


//----------------------------------------------------------------------------------
// Geometry Shader
//----------------------------------------------------------------------------------

template<typename ResourceT>
auto ResourceMgr::acquire(const std::wstring& guid, const ShaderBytecode& bytecode)
    -> std::enable_if_t<std::is_same_v<GeometryShader, ResourceT>, std::shared_ptr<GeometryShader>> {
	
	return geometry_shaders.acquire(guid, guid, device, bytecode);
}

template<typename ResourceT>
auto ResourceMgr::getResourceMap() const
    -> std::enable_if_t<std::is_same_v<GeometryShader, ResourceT>, const shader_resource_map<std::wstring, GeometryShader>&> {
	return geometry_shaders;
}


//----------------------------------------------------------------------------------
// Hull Shader
//----------------------------------------------------------------------------------

template<typename ResourceT>
auto ResourceMgr::acquire(const std::wstring& guid, const ShaderBytecode& bytecode)
    -> std::enable_if_t<std::is_same_v<HullShader, ResourceT>, std::shared_ptr<HullShader>> {
	
	return hull_shaders.acquire(guid, guid, device, bytecode);
}

template<typename ResourceT>
auto ResourceMgr::getResourceMap() const
    -> std::enable_if_t<std::is_same_v<HullShader, ResourceT>, const shader_resource_map<std::wstring, HullShader>&> {
	return hull_shaders;
}


//----------------------------------------------------------------------------------
// Pixel Shader
//----------------------------------------------------------------------------------

template<typename ResourceT>
auto ResourceMgr::acquire(const std::wstring& guid, const ShaderBytecode& bytecode)
    -> std::enable_if_t<std::is_same_v<PixelShader, ResourceT>, std::shared_ptr<PixelShader>> {
	
	return pixel_shaders.acquire(guid, guid, device, bytecode);
}

template<typename ResourceT>
auto ResourceMgr::getResourceMap() const
    -> std::enable_if_t<std::is_same_v<PixelShader, ResourceT>, const shader_resource_map<std::wstring, PixelShader>&> {
	return pixel_shaders;
}


//----------------------------------------------------------------------------------
// Vertex Shader
//----------------------------------------------------------------------------------

template<typename ResourceT>
auto ResourceMgr::acquire(const std::wstring& guid,
	                      const ShaderBytecode& bytecode,
	                      gsl::span<const D3D11_INPUT_ELEMENT_DESC> input_element_descs)
    -> std::enable_if_t<std::is_same_v<VertexShader, ResourceT>, std::shared_ptr<VertexShader>> {
	
	return vertex_shaders.acquire(guid, guid, device, bytecode, input_element_descs);
}

template<typename ResourceT>
auto ResourceMgr::getResourceMap() const
    -> std::enable_if_t<std::is_same_v<VertexShader, ResourceT>, const shader_resource_map<std::wstring, VertexShader>&> {
	return vertex_shaders;
}