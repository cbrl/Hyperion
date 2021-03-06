#pragma once

#include "datatypes/types.h"
#include "resource/resource_mgr.h"
#include "resource/shader/shader.h"
#include "rendering_options.h"


namespace render::ShaderFactory {

//----------------------------------------------------------------------------------
// From File/Memory
//----------------------------------------------------------------------------------
template<typename ShaderT>
[[nodiscard]]
std::shared_ptr<ShaderT> CreateShaderFromMemory(ResourceMgr& resource_mgr,
                                                std::string_view data,
                                                const std::wstring& shader_name,
                                                const std::string& entry_point,
                                                const std::string& target_ver);

template<typename ShaderT>
[[nodiscard]]
std::shared_ptr<ShaderT> CreateShaderFromFile(ResourceMgr& resource_mgr,
                                              const fs::path& file,
                                              const std::string& entry_point,
                                              const std::string& target_ver);


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

#include "shader_factory.tpp"