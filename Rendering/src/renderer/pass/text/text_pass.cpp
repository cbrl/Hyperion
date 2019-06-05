#include "text_pass.h"
#include "scene/scene.h"


namespace render {

TextPass::TextPass(ID3D11DeviceContext& device_context) {
	sprite_batch = std::make_unique<SpriteBatch>(&device_context);
}


void TextPass::render(Scene& scene) const {

	scene.forEach<Transform, Text>([&](const ecs::Entity& entity) {
		const auto& transform = *entity.getComponent<Transform>();
		const auto  texts     = entity.getAll<Text>();

		for (const Text& text : texts) {
			if (!text.isActive())
				continue;

			const auto& font = text.getFont();

			sprite_batch->Begin();
			font.DrawString(sprite_batch.get(),
			                text.getText().c_str(),
			                transform.getPosition(),
			                text.getColor(),
			                XMVectorGetZ(transform.getRotation()),
			                XMVectorZero(),
			                transform.getScale());
			sprite_batch->End();
		}
	});
}

} //namespace render