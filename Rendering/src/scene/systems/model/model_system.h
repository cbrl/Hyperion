#pragma once

#include "system/system.h"

namespace render {

class RenderingMgr;

namespace systems {

class ModelSystem final : public ecs::System<ModelSystem> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	ModelSystem(const RenderingMgr& rendering_mgr);
	ModelSystem(const ModelSystem&) = delete;
	ModelSystem(ModelSystem&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructors
	//----------------------------------------------------------------------------------
	~ModelSystem() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	ModelSystem& operator=(const ModelSystem&) = delete;
	ModelSystem& operator=(ModelSystem&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void update() override;

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	const RenderingMgr& rendering_mgr;
};

} //namespace systems
} //namespace render