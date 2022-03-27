module;

#include "datatypes/types.h"
#include "io/io.h"

#include "directx/d3d11.h"

export module rendering.shader_factory;

export import rendering.shader;
export import rendering.rendering_options;
import rendering.resource_mgr;
import rendering.vertex_types;


export namespace render::ShaderFactory {

namespace detail {
template<typename ShaderT>
[[nodiscard]]
std::shared_ptr<ShaderT> CreateShader(ResourceMgr& resource_mgr, const std::wstring& shader_name, const ComPtr<ID3DBlob>& blob) {
	if (!blob) return {};

	if constexpr (std::is_same_v<VertexShader, ShaderT>) {
		return resource_mgr.createOrReplace<ShaderT>(shader_name,
		                                             ShaderBytecodeBlob(blob),
		                                             std::span{VertexPositionNormalTexture::input_elements,
		                                                       VertexPositionNormalTexture::input_element_count});
	}
	else {
		return resource_mgr.createOrReplace<ShaderT>(shader_name, ShaderBytecodeBlob(blob));
	}
}
} //namespace detail


//----------------------------------------------------------------------------------
// From File/Memory
//----------------------------------------------------------------------------------
template<typename ShaderT>
[[nodiscard]]
std::shared_ptr<ShaderT> CreateShaderFromMemory(ResourceMgr& resource_mgr,
                                                std::string_view data,
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
[[nodiscard]]
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


//----------------------------------------------------------------------------------
// Forward
//----------------------------------------------------------------------------------
[[nodiscard]]
std::shared_ptr<PixelShader> CreateForwardPS(ResourceMgr& resource_mgr, BRDF brdf, bool transparency);

[[nodiscard]]
std::shared_ptr<VertexShader> CreateForwardVS(ResourceMgr& resource_mgr);


//----------------------------------------------------------------------------------
// GBuffer
//----------------------------------------------------------------------------------
[[nodiscard]]
std::shared_ptr<PixelShader> CreateGBufferPS(ResourceMgr& resource_mgr);


//----------------------------------------------------------------------------------
// Deferred
//----------------------------------------------------------------------------------
[[nodiscard]]
std::shared_ptr<PixelShader> CreateDeferredPS(ResourceMgr& resource_mgr, BRDF brdf);    


//----------------------------------------------------------------------------------
// Depth
//----------------------------------------------------------------------------------
[[nodiscard]]
std::shared_ptr<VertexShader> CreateDepthVS(ResourceMgr& resource_mgr);

[[nodiscard]]
std::shared_ptr<PixelShader> CreateDepthTransparentPS(ResourceMgr& resource_mgr);

[[nodiscard]]
std::shared_ptr<VertexShader> CreateDepthTransparentVS(ResourceMgr& resource_mgr);


//----------------------------------------------------------------------------------
// Sky
//----------------------------------------------------------------------------------
[[nodiscard]]
std::shared_ptr<PixelShader> CreateSkyPS(ResourceMgr& resource_mgr);

[[nodiscard]]
std::shared_ptr<VertexShader> CreateSkyVS(ResourceMgr& resource_mgr);


//----------------------------------------------------------------------------------
// Bounding Volume
//----------------------------------------------------------------------------------
[[nodiscard]]
std::shared_ptr<PixelShader> CreateWireframeBoxPS(ResourceMgr& resource_mgr);

[[nodiscard]]
std::shared_ptr<VertexShader> CreateWireframeBoxVS(ResourceMgr& resource_mgr);


//----------------------------------------------------------------------------------
// False Color
//----------------------------------------------------------------------------------
[[nodiscard]]
std::shared_ptr<PixelShader> CreateFalseColorPS(ResourceMgr& resource_mgr, FalseColor color);


//----------------------------------------------------------------------------------
// Fullscreen Triangle
//----------------------------------------------------------------------------------
[[nodiscard]]
std::shared_ptr<VertexShader> CreateFullscreenQuadVS(ResourceMgr& resource_mgr);

} //namespace render::ShaderFactory

