module;

#include "datatypes/types.h"
#include "memory/handle/handle.h"

#include "directx/d3d11.h"
#include "directx/directxtk.h"

export module rendering:pass.text_pass;

import ecs;
import :components.text;
import :components.transform;


namespace render {

export class TextPass final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	TextPass(ID3D11DeviceContext& device_context) {
		sprite_batch = std::make_unique<SpriteBatch>(&device_context);
	}

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
	void render(const ecs::ECS& ecs) const {
		ecs.forEach<Transform, Text>([&](handle64 entity) {
			const auto& transform = ecs.get<Transform>(entity);
			const auto& text = ecs.get<Text>(entity);

			if (not text.isActive())
				return;

			const auto& font = text.getFont();

			sprite_batch->Begin();
			font.DrawString(sprite_batch.get(),
				text.getText().c_str(),
				transform.getRelativePosition(),
				text.getColor(),
				XMVectorGetZ(transform.getRelativeRotation()),
				XMVectorZero(),
				transform.getRelativeScale());
			sprite_batch->End();
		});
	}

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::unique_ptr<SpriteBatch> sprite_batch;
};

} //namespace render
