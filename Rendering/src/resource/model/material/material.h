#pragma once

#include "datatypes/datatypes.h"
#include "resource/texture/texture.h"
#include "resource/shader/shader.h"


namespace render {

struct Material {

	// The material's name
	std::string name;

	// The scalar paramters of the material
	struct {
		f32_4 base_color;
		f32   metalness;
		f32   roughness;
		f32_3 emissive;
	} params;

	// The textures for the material
	struct {
		std::shared_ptr<Texture> base_color;
		std::shared_ptr<Texture> material_params; //R: metalness, G: roughness
		std::shared_ptr<Texture> normal;
		std::shared_ptr<Texture> emissive;
	} maps;

	// (Optional) shader that overrides the default global shader
	std::shared_ptr<PixelShader> shader;
};

} //namespace render