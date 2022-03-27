module;

#include "io/io.h"
#include "log/log.h"

#include "directx/d3d11.h"

export module rendering.shader_bytecode;


export namespace render {

// Compile a shader file
[[nodiscard]]
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

// Compile a shader from memory
[[nodiscard]]
HRESULT CompileShaderToBytecode(std::string_view data,
                                const std::string& entry_point,
                                const std::string& target_ver,
                                gsl::not_null<ID3DBlob**> out){
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


class ShaderBytecode {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	ShaderBytecode() = default;
	ShaderBytecode(const ShaderBytecode& buffer) noexcept = default;
	ShaderBytecode(ShaderBytecode&& buffer) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	virtual ~ShaderBytecode() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	ShaderBytecode& operator=(const ShaderBytecode& buffer) noexcept = default;
	ShaderBytecode& operator=(ShaderBytecode&& buffer) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	virtual const BYTE* getBytecode() const noexcept = 0;

	[[nodiscard]]
	virtual size_t getBytecodeSize() const noexcept = 0;
};




class ShaderBytecodeBuffer final : public ShaderBytecode {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	ShaderBytecodeBuffer(std::span<const BYTE> bytecode_buffer)
		: buffer(bytecode_buffer.data())
		, size(bytecode_buffer.size()) {
	}
	ShaderBytecodeBuffer(const ShaderBytecodeBuffer& buffer) noexcept = default;
	ShaderBytecodeBuffer(ShaderBytecodeBuffer&& buffer) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~ShaderBytecodeBuffer() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	ShaderBytecodeBuffer& operator=(const ShaderBytecodeBuffer& buffer) noexcept = default;
	ShaderBytecodeBuffer& operator=(ShaderBytecodeBuffer&& buffer) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	const BYTE* getBytecode() const noexcept override {
		return buffer;
	}

	[[nodiscard]]
	size_t getBytecodeSize() const noexcept override {
		return size;
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	const BYTE* buffer;
	size_t size;
};




class ShaderBytecodeBlob final : public ShaderBytecode {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	ShaderBytecodeBlob(const std::wstring& file) {
		D3DReadFileToBlob(file.c_str(), blob.GetAddressOf());
	}
	ShaderBytecodeBlob(ComPtr<ID3DBlob> blob)
		: blob(std::move(blob)) {
	}
	ShaderBytecodeBlob(const ShaderBytecodeBlob& buffer) noexcept = default;
	ShaderBytecodeBlob(ShaderBytecodeBlob&& buffer) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~ShaderBytecodeBlob() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	ShaderBytecodeBlob& operator=(const ShaderBytecodeBlob& buffer) noexcept = default;
	ShaderBytecodeBlob& operator=(ShaderBytecodeBlob&& buffer) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	const BYTE* getBytecode() const noexcept override {
		return static_cast<BYTE*>(blob->GetBufferPointer());
	}

	[[nodiscard]]
	size_t getBytecodeSize() const noexcept override {
		return blob->GetBufferSize();
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	ComPtr<ID3DBlob> blob;
};

} //namespace render