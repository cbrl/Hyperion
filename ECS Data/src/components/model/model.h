#pragma once

#include "directx/d3d11.h"
#include "directx/directx_math.h"
#include "datatypes/datatypes.h"
#include "io/io.h"

#include "component/component.h"
#include "buffer/buffers.h"
#include "buffer/constant_buffer.h"
#include "resource/mesh/mesh.h"
#include "resource/model/model_blueprint.h"
#include "resource/model/material/material.h"
#include "geometry/bounding_volume/bounding_volume.h"


class Model final : public Component<Model> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	Model(ID3D11Device& device,
	      shared_ptr<Mesh> mesh,
	      const ModelPart& part,
	      const Material& mat);

	Model(const Model& model) = delete;
	Model(Model&& mode) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~Model() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	Model& operator=(const Model& model) = delete;
	Model& operator=(Model&& model) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Name
	//----------------------------------------------------------------------------------

	const std::string& getName() const {
		return name;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Buffer
	//----------------------------------------------------------------------------------

	// Bind the model's cbuffer to a pipeline stage
	template<typename StageT>
	void bindBuffer(ID3D11DeviceContext& device_context, u32 slot) const {
		buffer.bind<StageT>(device_context, slot);
	}

	// Update the model's cbuffer
	void XM_CALLCONV updateBuffer(ID3D11DeviceContext& device_context, FXMMATRIX object_to_world);


	//----------------------------------------------------------------------------------
	// Member Functions - Mesh
	//----------------------------------------------------------------------------------

	// Bind the model's vertex and index buffers
	void bind(ID3D11DeviceContext& device_context) const {
		mesh->bind(device_context);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Index info
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	u32 getIndexStart() const {
		return index_start;
	}

	[[nodiscard]]
	u32 getIndexCount() const {
		return index_count;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Material
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	Material& getMaterial() {
		return material;
	}

	[[nodiscard]]
	const Material& getMaterial() const {
		return material;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Bounding Volumes
	//----------------------------------------------------------------------------------

	const AABB& getAABB() const {
		return aabb;
	}

	const BoundingSphere& getBoundingSphere() const {
		return sphere;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Shadows
	//----------------------------------------------------------------------------------

	void setShadows(bool state) {
		shadows = state;
	}

	[[nodiscard]]
	bool castsShadows() const {
		return shadows;
	}


private:
	// The model's name
	std::string name;

	// The model's cbuffer
	ConstantBuffer<ModelBuffer> buffer;

	// The vertex and index buffer of the model
	shared_ptr<Mesh> mesh;

	// The starting index and index count of the model
	u32 index_start;
	u32 index_count;

	// The model's material
	Material material;

	// Bounding volumes
	AABB aabb;
	BoundingSphere sphere;

	// Flag that decides if the model can cast shadows
	bool shadows;
};
