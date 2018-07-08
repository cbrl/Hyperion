#pragma once

#include "directx/d3d11.h"


// Compile a shader file to bytecode
HRESULT CompileShaderToBytecode(const wstring& file,
                                const string& entry_point,
                                const string& target_ver,
                                ID3DBlob** out);


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
	virtual const BYTE* getBytecode() const noexcept = 0;
	virtual size_t getBytecodeSize() const noexcept = 0;
};




class ShaderBytecodeBuffer final : public ShaderBytecode {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	ShaderBytecodeBuffer(const BYTE* bytecode_buffer, size_t buffer_size)
		: buffer(bytecode_buffer)
		, size(buffer_size) {
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
	const BYTE* getBytecode() const noexcept override {
		return buffer;
	}

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
	ShaderBytecodeBlob(const wstring& file) {
		D3DReadFileToBlob(file.c_str(), blob.GetAddressOf());
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
	const BYTE* getBytecode() const noexcept override {
		return static_cast<BYTE*>(blob->GetBufferPointer());
	}

	size_t getBytecodeSize() const noexcept override {
		return blob->GetBufferSize();
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	ComPtr<ID3DBlob> blob;
};