#pragma once

#include "datatypes/datatypes.h"
#include "resource/texture/texture.h"


//struct Material {
//
//	std::string name;
//
//	struct {
//		vec4_f32 diffuse;
//		vec4_f32 ambient;
//		vec4_f32 specular;
//		vec4_f32 emissive;
//		vec4_f32 transparent;
//
//		float opacity          = 1.0f;
//		float spec_scale       = 1.0f;
//		float spec_exponent    = 1.0f;
//		float refractive_index = 1.0f;
//		bool  mirror           = false;
//		float reflectivity     = 1.0f;
//		/*TODO*/ bool  wireframe        = false;
//		/*TODO*/ bool  two_sided        = false;
//	} params;
//
//	struct {
//		std::shared_ptr<Texture> diffuse;
//		std::shared_ptr<Texture> ambient;
//		std::shared_ptr<Texture> normal;
//		std::shared_ptr<Texture> specular;
//		std::shared_ptr<Texture> spec_exponent;
//		std::shared_ptr<Texture> emissive;
//		std::shared_ptr<Texture> opacity;
//		std::shared_ptr<Texture> height;
//		std::shared_ptr<Texture> light;
//	} maps;
//};


struct Material {

	std::string name;

	struct {
		vec4_f32 base_color;
		f32 metalness;
		f32 roughness;
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