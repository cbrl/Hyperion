#pragma once

#include "datatypes/datatypes.h"
#include "resource/texture/texture.h"
#include "resource/shader/shader.h"


struct Material {

	std::string name;

	struct {
		f32_4 base_color;
		f32      metalness;
		f32      roughness;
		f32_3 emissive;

		//bool mirror = false;
		//bool wireframe = false;
		//bool two_sided = false;
	} params;

	struct {
		std::shared_ptr<Texture> base_color;
		std::shared_ptr<Texture> material_params; //R: metalness, G: roughness
		std::shared_ptr<Texture> normal;
		std::shared_ptr<Texture> emissive;
	} maps;


	std::shared_ptr<PixelShader> shader;
};