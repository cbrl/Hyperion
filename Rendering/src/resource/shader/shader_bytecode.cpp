#include "shader_bytecode.h"
#include "io/io.h"

HRESULT CompileShaderToBytecode(const std::wstring& file,
                                const std::string& entry_point,
                                const std::string& target_ver,
                                gsl::not_null<ID3DBlob**> out) {

	u32 flags = D3DCOMPILE_ENABLE_STRICTNESS;

	#ifdef _DEBUG
	flags |= D3DCOMPILE_DEBUG;
	flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	#endif

	ComPtr<ID3DBlob> error_msgs;
	const HRESULT hr = D3DCompileFromFile(file.c_str(),
	                                      nullptr,
	                                      D3D_COMPILE_STANDARD_FILE_INCLUDE,
	                                      entry_point.c_str(),
	                                      target_ver.c_str(),
	                                      flags,
	                                      NULL,
	                                      out,
	                                      error_msgs.GetAddressOf());

	if (FAILED(hr)) {

		if (error_msgs) {
			// Send error message to the debugger
			OutputDebugStringA(static_cast<const char*>(error_msgs->GetBufferPointer()));

			// Copy error message to file and notify user
			std::ofstream fout("shader_error.txt");

			if (fout) {
				const char*  compile_errors = static_cast<const char*>(error_msgs->GetBufferPointer());
				const size_t buffer_size    = error_msgs->GetBufferSize();

				for (size_t i = 0; i < buffer_size; ++i) {
					fout << compile_errors[i];
				}
				fout.close();

				MessageBox(nullptr, L"Error compiling shader. Check shader_error.txt", L"Shader Compilation Failed", MB_OK);
			}
		}
	}

	return hr;
}