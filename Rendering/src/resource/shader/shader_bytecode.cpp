#include "shader_bytecode.h"

namespace render {

HRESULT CompileShaderToBytecode(const fs::path& file,
                                const std::string& entry_point,
                                const std::string& target_ver,
                                gsl::not_null<ID3DBlob**> out) {

	u32 flags = D3DCOMPILE_ENABLE_STRICTNESS;

	#ifdef _DEBUG
	flags |= D3DCOMPILE_DEBUG;
	flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	#endif

	ComPtr<ID3DBlob> error_msgs;
	const auto filename = file.wstring();
	const HRESULT hr = D3DCompileFromFile(filename.c_str(),
	                                      nullptr,
	                                      D3D_COMPILE_STANDARD_FILE_INCLUDE,
	                                      entry_point.c_str(),
	                                      target_ver.c_str(),
	                                      flags,
	                                      NULL,
	                                      out,
	                                      error_msgs.GetAddressOf());

	if (FAILED(hr) && error_msgs) {
		// Get the buffer pointer and size
		const auto*  buffer = static_cast<const char*>(error_msgs->GetBufferPointer());
		const size_t size = error_msgs->GetBufferSize();

		// Create a string_view and print the error message
		const std::string_view error{buffer, size};
		Logger::log(LogLevel::err, "Shader compilation failed: {}", error);
	}

	return hr;
}


HRESULT CompileShaderToBytecode(std::string_view data,
                                const std::string& entry_point,
                                const std::string& target_ver,
                                gsl::not_null<ID3DBlob**> out) {

	u32 flags = D3DCOMPILE_ENABLE_STRICTNESS;

	#ifdef _DEBUG
	flags |= D3DCOMPILE_DEBUG;
	flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	#endif

	ComPtr<ID3DBlob> error_msgs;
	const HRESULT hr = D3DCompile(data.data(),
	                              data.size(),
	                              nullptr,
	                              nullptr,
	                              D3D_COMPILE_STANDARD_FILE_INCLUDE,
	                              entry_point.c_str(),
	                              target_ver.c_str(),
	                              flags,
	                              NULL,
	                              out,
	                              error_msgs.GetAddressOf());

	if (FAILED(hr) && error_msgs) {
		// Get the buffer pointer and size
		const auto* buffer = static_cast<const char*>(error_msgs->GetBufferPointer());
		const size_t size = error_msgs->GetBufferSize();

		// Create a string_view and print the error message
		const std::string_view error{buffer, size};
		Logger::log(LogLevel::err, "Shader compilation failed: {}", error);
	}

	return hr;
}

} //namespace render