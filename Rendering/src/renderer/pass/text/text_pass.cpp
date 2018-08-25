#include "text_pass.h"
#include "scene/scene.h"


TextPass::TextPass(ID3D11DeviceContext& device_context) {
	sprite_batch = std::make_unique<SpriteBatch>(&device_context);
}


void TextPass::render(Scene& scene) const {

	auto& ecs = scene.getECS();

	ecs.forEach<Text>([&](Text& text) {

		auto* owner = ecs.getEntity(text.getOwner());
		const auto* transform = owner->getComponent<Transform>();
		if (!transform) return;

		const auto& font = text.getFont();

		sprite_batch->Begin();
		font.DrawString(sprite_batch.get(),
		                text.getText().c_str(),
		                transform->getPosition(),
		                text.getColor(),
		                XMVectorGetZ(transform->getRotation()),
		                transform->getObjectOrigin(),
		                transform->getScale());
		sprite_batch->End();
	});
}
