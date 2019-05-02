
namespace render {

template<typename ShaderT, typename StageT>
Shader<ShaderT, StageT>::Shader(const std::wstring& guid,
                                ID3D11Device& device,
                                const ShaderBytecode& bytecode)
	: Resource<Shader>(guid) {

	createShader(device, bytecode);
}


template<>
inline void ComputeShader::createShader(ID3D11Device& device, const ShaderBytecode& bytecode) {

	ThrowIfFailed(device.CreateComputeShader(bytecode.getBytecode(),
	                                         bytecode.getBytecodeSize(),
	                                         nullptr,
	                                         shader.GetAddressOf()),
	              "Failed to create compute shader");
}


template<>
inline void DomainShader::createShader(ID3D11Device& device, const ShaderBytecode& bytecode) {

	ThrowIfFailed(device.CreateDomainShader(bytecode.getBytecode(),
	                                        bytecode.getBytecodeSize(),
	                                        nullptr,
	                                        shader.GetAddressOf()),
	              "Failed to create domain shader");
}


template<>
inline void GeometryShader::createShader(ID3D11Device& device, const ShaderBytecode& bytecode) {

	ThrowIfFailed(device.CreateGeometryShader(bytecode.getBytecode(),
	                                          bytecode.getBytecodeSize(),
	                                          nullptr,
	                                          shader.GetAddressOf()),
	              "Failed to create geometry shader");
}


template<>
inline void HullShader::createShader(ID3D11Device& device, const ShaderBytecode& bytecode) {

	ThrowIfFailed(device.CreateHullShader(bytecode.getBytecode(),
	                                      bytecode.getBytecodeSize(),
	                                      nullptr,
	                                      shader.GetAddressOf()),
	              "Failed to create hull shader");
}


template<>
inline void PixelShader::createShader(ID3D11Device& device, const ShaderBytecode& bytecode) {

	ThrowIfFailed(device.CreatePixelShader(bytecode.getBytecode(),
	                                       bytecode.getBytecodeSize(),
	                                       nullptr,
	                                       shader.GetAddressOf()),
	              "Failed to create pixel shader");
}

} //namespace render