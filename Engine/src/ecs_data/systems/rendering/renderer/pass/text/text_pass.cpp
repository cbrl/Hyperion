#include "stdafx.h"
#include "text_pass.h"
#include "scene/scene.h"


TextPass::TextPass(ID3D11DeviceContext& device_context) {
	sprite_batch = make_unique<SpriteBatch>(&device_context);
}


void TextPass::render(Scene& scene) const {

	const auto& texts = scene.getTexts();

	for (const auto& pair : texts) {

		const auto& text = pair.second;

		// Get the font
		const auto& font = text.getFont();

		// Draw the text
		sprite_batch->Begin();
		font.DrawString(sprite_batch.get(),
		                text.getText().data(),
		                text.getPosition(),
		                text.getColor(),
		                text.getRotation());
		sprite_batch->End();
	}
}
