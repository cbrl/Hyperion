
namespace ShaderFactory {

namespace detail {
template<typename ShaderT>
[[nodiscard]]
std::shared_ptr<ShaderT> CreateShader(ResourceMgr& resource_mgr, const std::wstring& shader_name, const ComPtr<ID3DBlob>& blob) {
	if (!blob) return {};

	if constexpr (std::is_same_v<VertexShader, ShaderT>) {
		return resource_mgr.createOrReplace<ShaderT>(shader_name,
		                                             ShaderBytecodeBlob(blob),
		                                             gsl::make_span(VertexPositionNormalTexture::input_elements,
		                                                            VertexPositionNormalTexture::input_element_count));
	}
	else {
		return resource_mgr.createOrReplace<ShaderT>(shader_name, ShaderBytecodeBlob(blob));
	}
}
} //namespace detail


template<typename ShaderT>
std::shared_ptr<ShaderT> CreateShaderFromMemory(ResourceMgr& resource_mgr,
                                                gsl::span<const char> data,
                                                const std::wstring& shader_name,
                                                const std::string& entry_point,
                                                const std::string& target_ver) {
	ComPtr<ID3DBlob> blob;
	const HRESULT result = CompileShaderToBytecode(data,
	                                               entry_point,
	                                               target_ver,
	                                               gsl::make_not_null(blob.GetAddressOf()));

	if (SUCCEEDED(result)) {
		return detail::CreateShader<ShaderT>(resource_mgr, shader_name, blob);
	}
	return {};
}


template<typename ShaderT>
std::shared_ptr<ShaderT> CreateShaderFromFile(ResourceMgr& resource_mgr,
                                              const fs::path& file,
                                              const std::string& entry_point,
                                              const std::string& target_ver) {
	ComPtr<ID3DBlob> blob;
	const HRESULT result = CompileShaderToBytecode(file,
	                                               entry_point,
	                                               target_ver,
	                                               gsl::make_not_null(blob.GetAddressOf()));

	if (SUCCEEDED(result)) {
		return detail::CreateShader<ShaderT>(resource_mgr, file.filename().wstring(), blob);
	}
	return {};
}

} //namespace ShaderFactory