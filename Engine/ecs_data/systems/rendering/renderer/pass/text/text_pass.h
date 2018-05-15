#pragma once

#include <d3d11.h>
#include <SpriteBatch.h>

#include "util\datatypes\datatypes.h"
#include "scene\scene.h"


class TextPass final {
	public:
		TextPass(ID3D11DeviceContext& device_context);
		~TextPass() = default;

		void Render(Scene& scene);


	private:
		unique_ptr<SpriteBatch> sprite_batch;
};