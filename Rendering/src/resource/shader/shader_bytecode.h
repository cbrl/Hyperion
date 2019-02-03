#pragma once

#include "directx/d3d11.h"


// Compile a shader file
[[nodiscard]]
HRESULT CompileShaderToBytecode(const std::wstring& file,
                                const std::string& entry_point,
                                const std::string& target_ver,
                                gsl::not_null<ID3DBlob**> out);

// Compile a shader from memory
[[nodiscard]]
HRESULT CompileShaderToBytecode(gsl::span<const char> data,
                                const std::string& entry_point,
                                const std::string& target_ver,
                                gsl::not_null<ID3DBlob**> out);


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
	ShaderBytecodeBuffer(gsl::span<const BYTE> bytecode_buffer)
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