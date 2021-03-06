#include "text_pass.h"
#include "scene/scene.h"


namespace render {

TextPass::TextPass(ID3D11DeviceContext& device_context) {
	sprite_batch = std::make_unique<SpriteBatch>(&device_context);
}


void TextPass::render(const ecs::ECS& ecs) const {
	ecs.forEach<Transform, Text>([&](handle64 entity) {
		const auto& transform = ecs.get<Transform>(entity);
		const auto& text      = ecs.get<Text>(entity);

		if (not text.isActive())
			return;

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
	});
}

} //namespace render