module;

#include <concepts>
#include <functional>
#include <memory>
#include <string>
#include <type_traits>

#include <DirectXMath.h>

#include "datatypes/scalar_types.h"
#include "memory/managed_resource_map.h"

#include "directx/d3d11.h"

export module rendering:resource_mgr;

import :resource;
import :font;
import :mesh;
import :model_blueprint;
import :model_config;
import :model_output;
import :shader;
import :shader_bytecode;
import :texture;


namespace render {

export class ResourceMgr final {
	template<typename KeyT, typename ValueT>
	using resource_map = ConcurrentWeakResourceMap<KeyT, ValueT>;

	template<typename KeyT, typename ValueT>
	using shader_resource_map = ConcurrentSharedResourceMap<KeyT, ValueT>;

public:

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	ResourceMgr(ID3D11Device& device, ID3D11DeviceContext& device_context)
		: device(device)
		, device_context(device_context) {
	}

	ResourceMgr(const ResourceMgr& mgr) = delete;
	ResourceMgr(ResourceMgr&& mgr) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~ResourceMgr() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	ResourceMgr& operator=(const ResourceMgr& mgr) = delete;
	ResourceMgr& operator=(ResourceMgr&& mgr) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - ModelBlueprint
	//----------------------------------------------------------------------------------

	template<typename ResourceT, typename VertexT>
	requires std::same_as<ModelBlueprint, ResourceT>
	[[nodiscard]]
	std::shared_ptr<ModelBlueprint> getOrCreate(const std::wstring& name,
	                                            const ModelOutput& model_data,
	                                            const ModelConfig<VertexT>& config);

	template<typename ResourceT, typename VertexT>
	requires std::same_as<ModelBlueprint, ResourceT>
	[[nodiscard]]
	std::shared_ptr<ModelBlueprint> createOrReplace(const std::wstring& name,
	                                                const ModelOutput& model_data,
	                                                const ModelConfig<VertexT>& config);

	template<typename ResourceT>
	requires std::same_as<ModelBlueprint, ResourceT>
	[[nodiscard]]
	const resource_map<std::wstring, ModelBlueprint>& getResourceMap() const;
		


	//----------------------------------------------------------------------------------
	// Member Functions - Texture
	//----------------------------------------------------------------------------------

	template<typename ResourceT>
	requires std::same_as<Texture, ResourceT>
	[[nodiscard]]
	std::shared_ptr<Texture> getOrCreate(const std::wstring& filename);

	template<typename ResourceT>
	requires std::same_as<Texture, ResourceT>
	[[nodiscard]]
	std::shared_ptr<Texture> getOrCreate(const std::wstring& name,
	                                     const D3D11_TEXTURE2D_DESC& desc,
	                                     const D3D11_SUBRESOURCE_DATA& init_data);

	template<typename ResourceT>
	requires std::same_as<Texture, ResourceT>
	[[nodiscard]]
	std::shared_ptr<Texture> createOrReplace(const std::wstring& filename);

	template<typename ResourceT>
	requires std::same_as<Texture, ResourceT>
	[[nodiscard]]
	std::shared_ptr<Texture> createOrReplace(const std::wstring& name,
	                                         const D3D11_TEXTURE2D_DESC& desc,
	                                         const D3D11_SUBRESOURCE_DATA& init_data);

	template<typename ResourceT>
	requires std::same_as<Texture, ResourceT>
	[[nodiscard]]
	const resource_map<std::wstring, Texture>& getResourceMap() const;


	//----------------------------------------------------------------------------------
	// Member Functions - Font
	//----------------------------------------------------------------------------------

	template<typename ResourceT>
	requires std::same_as<Font, ResourceT>
	[[nodiscard]]
	std::shared_ptr<Font> getOrCreate(const std::wstring& filename);

	template<typename ResourceT>
	requires std::same_as<Font, ResourceT>
	[[nodiscard]]
	std::shared_ptr<Font> createOrReplace(const std::wstring& filename);

	template<typename ResourceT>
	requires std::same_as<Font, ResourceT>
	[[nodiscard]]
	const resource_map<std::wstring, Font>& getResourceMap() const;


	//----------------------------------------------------------------------------------
	// Member Functions - Compute Shader
	//----------------------------------------------------------------------------------

	template<typename ResourceT>
	requires std::same_as<ComputeShader, ResourceT>
	[[nodiscard]]
	std::shared_ptr<ComputeShader> getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode);

	template<typename ResourceT>
	requires std::same_as<ComputeShader, ResourceT>
	[[nodiscard]]
	std::shared_ptr<ComputeShader> createOrReplace(const std::wstring& guid, const ShaderBytecode& bytecode);

	template<typename ResourceT>
	requires std::same_as<ComputeShader, ResourceT>
	[[nodiscard]]
	const shader_resource_map<std::wstring, ComputeShader>& getResourceMap() const;


	//----------------------------------------------------------------------------------
	// Member Functions - Domain Shader
	//----------------------------------------------------------------------------------

	template<typename ResourceT>
	requires std::same_as<DomainShader, ResourceT>
	[[nodiscard]]
	std::shared_ptr<DomainShader> getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode);

	template<typename ResourceT>
	requires std::same_as<DomainShader, ResourceT>
	[[nodiscard]]
	std::shared_ptr<DomainShader> createOrReplace(const std::wstring& guid, const ShaderBytecode& bytecode);

	template<typename ResourceT>
	requires std::same_as<DomainShader, ResourceT>
	[[nodiscard]]
	const shader_resource_map<std::wstring, DomainShader>& getResourceMap() const;


	//----------------------------------------------------------------------------------
	// Member Functions - Geometry Shader
	//----------------------------------------------------------------------------------

	template<typename ResourceT>
	requires std::same_as<GeometryShader, ResourceT>
	[[nodiscard]]
	std::shared_ptr<GeometryShader> getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode);

	template<typename ResourceT>
	requires std::same_as<GeometryShader, ResourceT>
	[[nodiscard]]
	std::shared_ptr<GeometryShader> createOrReplace(const std::wstring& guid, const ShaderBytecode& bytecode);

	template<typename ResourceT>
	requires std::same_as<GeometryShader, ResourceT>
	[[nodiscard]]
	const shader_resource_map<std::wstring, GeometryShader>& getResourceMap() const;


	//----------------------------------------------------------------------------------
	// Member Functions - Hull Shader
	//----------------------------------------------------------------------------------

	template<typename ResourceT>
	requires std::same_as<HullShader, ResourceT>
	[[nodiscard]]
	std::shared_ptr<HullShader> getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode);

	template<typename ResourceT>
	requires std::same_as<HullShader, ResourceT>
	[[nodiscard]]
	std::shared_ptr<HullShader> createOrReplace(const std::wstring& guid, const ShaderBytecode& bytecode);

	template<typename ResourceT>
	requires std::same_as<HullShader, ResourceT>
	[[nodiscard]]
	const shader_resource_map<std::wstring, HullShader>& getResourceMap() const;


	//----------------------------------------------------------------------------------
	// Member Functions - Pixel Shader
	//----------------------------------------------------------------------------------

	template<typename ResourceT>
	requires std::same_as<PixelShader, ResourceT>
	[[nodiscard]]
	std::shared_ptr<PixelShader> getOrCreate(const std::wstring& guid, const ShaderBytecode& bytecode);

	template<typename ResourceT>
	requires std::same_as<PixelShader, ResourceT>
	[[nodiscard]]
	std::shared_ptr<PixelShader> createOrReplace(const std::wstring& guid, const ShaderBytecode& bytecode);

	template<typename ResourceT>
	requires std::same_as<PixelShader, ResourceT>
	[[nodiscard]]
	const shader_resource_map<std::wstring, PixelShader>& getResourceMap() const;


	//----------------------------------------------------------------------------------
	// Member Functions - Vertex Shader
	//----------------------------------------------------------------------------------

	template<typename ResourceT>
	requires std::same_as<VertexShader, ResourceT>
	[[nodiscard]]
	std::shared_ptr<VertexShader> getOrCreate(const std::wstring& guid,
	                                          const ShaderBytecode& bytecode,
	                                          std::span<const D3D11_INPUT_ELEMENT_DESC> input_element_descs);

	template<typename ResourceT>
	requires std::same_as<VertexShader, ResourceT>
	[[nodiscard]]
	std::shared_ptr<VertexShader> createOrReplace(const std::wstring& guid,
	                                              const ShaderBytecode& bytecode,
	                                              std::span<const D3D11_INPUT_ELEMENT_DESC> input_element_descs);

	template<typename ResourceT>
	requires std::same_as<VertexShader, ResourceT>
	[[nodiscard]]
	const shader_resource_map<std::wstring, VertexShader>& getResourceMap() const;

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Device and device context
	std::reference_wrapper<ID3D11Device>        device;
	std::reference_wrapper<ID3D11DeviceContext> device_context;

	// Resources
	resource_map<std::wstring, ModelBlueprint> models;
	resource_map<std::wstring, Texture>        textures;
	resource_map<std::wstring, Font>           fonts;

	shader_resource_map<std::wstring, ComputeShader>  compute_shaders;
	shader_resource_map<std::wstring, DomainShader>   domain_shaders;
	shader_resource_map<std::wstring, GeometryShader> geometry_shaders;
	shader_resource_map<std::wstring, HullShader>     hull_shaders;
	shader_resource_map<std::wstring, PixelShader>    pixel_shaders;
	shader_resource_map<std::wstring, VertexShader>   vertex_shaders;
};

} //namespace render

#include "resource_mgr.tpp"
