
namespace render {

//----------------------------------------------------------------------------------
// ModelBlueprint
//----------------------------------------------------------------------------------

template<typename ResourceT, typename VertexT>
requires std::same_as<ModelBlueprint, ResourceT>
std::shared_ptr<ModelBlueprint> ResourceMgr::getOrCreate(const std::wstring& name,
	                                                     const ModelOutput& model_data,
	                                                     const ModelConfig<VertexT>& config) {

	return models.getOrCreate(name, device, model_data, config);
}

template<typename ResourceT, typename VertexT>
requires std::same_as<ModelBlueprint, ResourceT>
std::shared_ptr<ModelBlueprint> ResourceMgr::createOrReplace(const std::wstring& name,
                                                             const ModelOutput& model_data,
                                                             const ModelConfig<VertexT>& config) {

	return models.createOrReplace(name, device, model_data, config);
}

template<typename ResourceT>
requires std::same_as<ModelBlueprint, ResourceT>
const ResourceMgr::resource_map<std::wstring, ModelBlueprint>& ResourceMgr::getResourceMap() const {
	return models;
}



//----------------------------------------------------------------------------------
// Texture
//----------------------------------------------------------------------------------

template<typename ResourceT>
requires std::same_as<Texture, ResourceT>
std::shared_ptr<Texture> ResourceMgr::getOrCreate(const std::wstring& filename) {

	return textures.getOrCreate(filename, device, device_context, filename);
}

template<typename ResourceT>
requires std::same_as<Texture, ResourceT>
std::shared_ptr<Texture> ResourceMgr::getOrCreate(const std::wstring& name,
                                                  const D3D11_TEXTURE2D_DESC& desc, 
                                                  const D3D11_SUBRESOURCE_DATA& init_data) {

	return textures.getOrCreate(name, name, device, desc, init_data);
}

template<typename ResourceT>
requires std::same_as<Texture, ResourceT>
std::shared_ptr<Texture> ResourceMgr::createOrReplace(const std::wstring& filename) {

	return textures.createOrReplace(filename, device, device_context, filename);
}

template<typename ResourceT>
requires std::same_as<Texture, ResourceT>
std::shared_ptr<Texture> ResourceMgr::createOrReplace(const std::wstring& name,
                                                      const D3D11_TEXTURE2D_DESC& desc,
                                                      const D3D11_SUBRESOURCE_DATA& init_data) {

	return textures.createOrReplace(name, name, device, desc, init_data);
}

template<typename ResourceT>
requires std::same_as<Texture, ResourceT>
const ResourceMgr::resource_map<std::wstring, Texture>& ResourceMgr::getResourceMap() const {
	return textures;
}



//----------------------------------------------------------------------------------	
// Font
//----------------------------------------------------------------------------------

template<typename ResourceT>
requires std::same_as<Font, ResourceT>
std::shared_ptr<Font> ResourceMgr::getOrCreate(const std::wstring& filename) {

	return fonts.getOrCreate(filename, device, filename.c_str());
}

template<typename ResourceT>
requires std::same_as<Font, ResourceT>
std::shared_ptr<Font> ResourceMgr::createOrReplace(const std::wstring& filename) {

	return fonts.createOrReplace(filename, device, filename.c_str());
}

template<typename ResourceT>
requires std::same_as<Font, ResourceT>
const ResourceMgr::resource_map<std::wstring, Font>& ResourceMgr::getResourceMap() const {
	return fonts;
}


//----------------------------------------------------------------------------------
// Compute Shader
//----------------------------------------------------------------------------------

template<typename ResourceT>
requires std::same_as<ComputeShader, ResourceT>
std::shared_ptr<ComputeShader> ResourceMgr::getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode) {
	
	return compute_shaders.getOrCreate(guid, guid, device, bytecode);
}

template<typename ResourceT>
requires std::same_as<ComputeShader, ResourceT>
std::shared_ptr<ComputeShader> ResourceMgr::createOrReplace(const std::wstring& guid, const ShaderBytecode& bytecode) {

	return compute_shaders.createOrReplace(guid, guid, device, bytecode);
}

template<typename ResourceT>
requires std::same_as<ComputeShader, ResourceT>
const ResourceMgr::shader_resource_map<std::wstring, ComputeShader>& ResourceMgr::getResourceMap() const {
	return compute_shaders;
}


//----------------------------------------------------------------------------------
// Domain Shader
//----------------------------------------------------------------------------------

template<typename ResourceT>
requires std::same_as<DomainShader, ResourceT>
std::shared_ptr<DomainShader> ResourceMgr::getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode) {
	
	return domain_shaders.getOrCreate(guid, guid, device, bytecode);
}

template<typename ResourceT>
requires std::same_as<DomainShader, ResourceT>
std::shared_ptr<DomainShader> ResourceMgr::createOrReplace(const std::wstring& guid, const ShaderBytecode& bytecode) {

	return domain_shaders.createOrReplace(guid, guid, device, bytecode);
}

template<typename ResourceT>
requires std::same_as<DomainShader, ResourceT>
const ResourceMgr::shader_resource_map<std::wstring, DomainShader>& ResourceMgr::getResourceMap() const {
	return domain_shaders;
}


//----------------------------------------------------------------------------------
// Geometry Shader
//----------------------------------------------------------------------------------

template<typename ResourceT>
requires std::same_as<GeometryShader, ResourceT>
std::shared_ptr<GeometryShader> ResourceMgr::getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode) {

	return geometry_shaders.getOrCreate(guid, guid, device, bytecode);
}

template<typename ResourceT>
requires std::same_as<GeometryShader, ResourceT>
std::shared_ptr<GeometryShader> ResourceMgr::createOrReplace(const std::wstring& guid, const ShaderBytecode& bytecode) {

	return geometry_shaders.createOrReplace(guid, guid, device, bytecode);
}

template<typename ResourceT>
requires std::same_as<GeometryShader, ResourceT>
const ResourceMgr::shader_resource_map<std::wstring, GeometryShader>& ResourceMgr::getResourceMap() const {
	return geometry_shaders;
}


//----------------------------------------------------------------------------------
// Hull Shader
//----------------------------------------------------------------------------------

template<typename ResourceT>
requires std::same_as<HullShader, ResourceT>
std::shared_ptr<HullShader> ResourceMgr::getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode) {
	
	return hull_shaders.getOrCreate(guid, guid, device, bytecode);
}

template<typename ResourceT>
requires std::same_as<HullShader, ResourceT>
std::shared_ptr<HullShader> ResourceMgr::createOrReplace(const std::wstring& guid, const ShaderBytecode& bytecode) {

	return hull_shaders.createOrReplace(guid, guid, device, bytecode);
}

template<typename ResourceT>
requires std::same_as<HullShader, ResourceT>
const ResourceMgr::shader_resource_map<std::wstring, HullShader>& ResourceMgr::getResourceMap() const {
	return hull_shaders;
}


//----------------------------------------------------------------------------------
// Pixel Shader
//----------------------------------------------------------------------------------

template<typename ResourceT>
requires std::same_as<PixelShader, ResourceT>
std::shared_ptr<PixelShader> ResourceMgr::getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode) {
	
	return pixel_shaders.getOrCreate(guid, guid, device, bytecode);
}

template<typename ResourceT>
requires std::same_as<PixelShader, ResourceT>
std::shared_ptr<PixelShader> ResourceMgr::createOrReplace(const std::wstring& guid, const ShaderBytecode& bytecode) {

	return pixel_shaders.createOrReplace(guid, guid, device, bytecode);
}

template<typename ResourceT>
requires std::same_as<PixelShader, ResourceT>
const ResourceMgr::shader_resource_map<std::wstring, PixelShader>& ResourceMgr::getResourceMap() const {
	return pixel_shaders;
}


//----------------------------------------------------------------------------------
// Vertex Shader
//----------------------------------------------------------------------------------

template<typename ResourceT>
requires std::same_as<VertexShader, ResourceT>
std::shared_ptr<VertexShader> ResourceMgr::getOrCreate(const std::wstring& guid,
                                                       const ShaderBytecode& bytecode,
                                                       std::span<const D3D11_INPUT_ELEMENT_DESC> input_element_descs) {
	
	return vertex_shaders.getOrCreate(guid, guid, device, bytecode, input_element_descs);
}

template<typename ResourceT>
requires std::same_as<VertexShader, ResourceT>
std::shared_ptr<VertexShader> ResourceMgr::createOrReplace(const std::wstring& guid,
                                                           const ShaderBytecode& bytecode,
                                                           std::span<const D3D11_INPUT_ELEMENT_DESC> input_element_descs) {

	return vertex_shaders.createOrReplace(guid, guid, device, bytecode, input_element_descs);
}

template<typename ResourceT>
requires std::same_as<VertexShader, ResourceT>
const ResourceMgr::shader_resource_map<std::wstring, VertexShader>& ResourceMgr::getResourceMap() const {
	return vertex_shaders;
}

} //namespace render