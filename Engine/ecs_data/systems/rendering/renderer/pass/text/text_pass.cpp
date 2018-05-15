#include "stdafx.h"
#include "text_pass.h"


TextPass::TextPass(ID3D11DeviceContext& device_context) {
	sprite_batch = make_unique<SpriteBatch>(&device_context);
}


void TextPass::Render(Scene& scene) {

	scene.ForEach<Text>([&](const Text& text) {
		// Get the font
		const auto& font = text.GetFont();

		// Draw the text
		sprite_batch->Begin();
		font.DrawString(sprite_batch.get(), text.GetText().data(), text.GetPosition(), text.GetColor(), text.GetRotation());
		sprite_batch->End();
	});
}