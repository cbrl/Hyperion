#pragma once

#include "directx/d3d11.h"
#include "directx/math/directx_math.h"
#include "datatypes/datatypes.h"
#include "io/io.h"

#include "component/component.h"
#include "buffer/buffers.h"
#include "buffer/constant_buffer.h"
#include "resource/mesh/mesh.h"
#include "resource/model/model_blueprint.h"
#include "resource/model/material/material.h"


//----------------------------------------------------------------------------------
// Model
//----------------------------------------------------------------------------------
//
// A model is a collection of a mesh, material, bounding volumes, and a shader
// constant buffer. It is created from a ModelBlueprint.
//
//----------------------------------------------------------------------------------
class Model final : public ecs::Component<Model> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Model(ID3D11Device& device,
	      const std::shared_ptr<render::ModelBlueprint>& bp,
	      u32 bp_index);

	Model(ID3D11Device& device,
	      const std::string& name,
	      const render::Mesh& mesh,
	      render::Material& mat,
	      const AABB& aabb,
	      const BoundingSphere& sphere,
	      const std::shared_ptr<render::ModelBlueprint>& bp);

	Model(const Model& model) = delete;
	Model(Model&& model) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~Model() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	Model& operator=(const Model& model) = delete;
	Model& operator=(Model&& model) = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Bind
	//----------------------------------------------------------------------------------

	void bindMesh(ID3D11DeviceContext& device_context) const {
		mesh.bind(device_context);
	}

	template<typename StageT>
	void bindBuffer(ID3D11DeviceContext& device_context, u32 slot) const {
		buffer.bind<StageT>(device_context, slot);
	}

	void XM_CALLCONV updateBuffer(ID3D11DeviceContext& device_context, FXMMATRIX object_to_world);


	//----------------------------------------------------------------------------------
	// Member Functions - Name
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	const std::string getName() const noexcept {
		return name;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Vertices
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	u32 getVertexCount() const noexcept {
		return mesh.getVertexCount();
	}

	[[nodiscard]]
	u32 getIndexCount() const noexcept {
		return mesh.getIndexCount();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Material
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	render::Material& getMaterial() noexcept {
		return material;
	}

	[[nodiscard]]
	const render::Material& getMaterial() const noexcept {
		return material;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Bounding Volumes
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	const AABB& getAABB() const noexcept {
		return aabb;
	}

	[[nodiscard]]
	const BoundingSphere& getBoundingSphere() const noexcept {
		return bounding_sphere;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Shadows
	//----------------------------------------------------------------------------------

	void setShadows(bool state) noexcept {
		shadows = state;
	}

	[[nodiscard]]
	bool castsShadows() const noexcept {
		return shadows;
	}


	[[nodiscard]]
	const render::ModelBlueprint& getBlueprint() const noexcept {
		return *blueprint;
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// The name of the model
	std::string name;

	// The shader constant buffer
	render::ConstantBuffer<render::ModelBuffer> buffer;

	// The mesh that the model refers to
	const render::Mesh& mesh;

	// The material that the model refers to
	render::Material& material;

	// The bounding volumes of the model
	AABB aabb;
	BoundingSphere bounding_sphere;

	// A flag that determines if the model casts shadows
	bool shadows;

	// The blueprint whose data this model references
	std::shared_ptr<render::ModelBlueprint> blueprint;
};