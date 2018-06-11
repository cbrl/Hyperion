#pragma once

#include "os/d3d11.h"
#include <SpriteBatch.h>

#include "datatypes/datatypes.h"

class Scene;

class TextPass final {
public:
	TextPass(ID3D11DeviceContext& device_context);
	~TextPass() = default;

	void render(Scene& scene) const;


private:
	unique_ptr<SpriteBatch> sprite_batch;
};
