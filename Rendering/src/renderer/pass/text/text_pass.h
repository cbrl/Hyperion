#pragma once

#include "directx/d3d11.h"
#include "directx/directxtk.h"
#include "datatypes/datatypes.h"


namespace ecs {
	class ECS;
}

namespace render {

class Scene;

class TextPass final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	TextPass(ID3D11DeviceContext& device_context);
	TextPass(const TextPass&) = delete;
	TextPass(TextPass&&) = default;

	//----------------------------------------------------------------------------------
	// Destructors
	//----------------------------------------------------------------------------------
	~TextPass() = default;

	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	TextPass& operator=(const TextPass&) = delete;
	TextPass& operator=(TextPass&&) = default;

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void render(const ecs::ECS& ecs) const;

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::unique_ptr<SpriteBatch> sprite_batch;
};

} //namespace render
