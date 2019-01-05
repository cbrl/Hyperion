#pragma once

#include "datatypes/datatypes.h"
#include "resource/texture/texture.h"


struct Material {

	std::string name;

	struct {
		vec4_f32 base_color;
		f32      metalness;
		f32      roughness;
		vec3_f32 emissive;

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
};