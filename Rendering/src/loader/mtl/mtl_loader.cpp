#include "mtl_loader.h"
#include "mtl_tokens.h"
#include "resource/resource_mgr.h"


MtlLoader::MtlLoader(ResourceMgr& resource_mgr, std::vector<Material>& materials)
	: resource_mgr(resource_mgr)
	, materials(materials) {
}


void MtlLoader::load(const fs::path& file) {

	readFile(file);
}


void MtlLoader::readLine() {
	
	const std::string token = readToken<std::string>();

	if (token[0] == MtlTokens::comment) {
		return;
	}

	// New Material
	if (token == MtlTokens::new_mtl) {
		materials.emplace_back();
		materials.back().name = readToken<std::string>();
		materials.back().has_texture = false;
	}

	// Diffuse Color
	else if (token == MtlTokens::diffuse_color) {
		materials.back().diffuse.x = readToken<f32>();
		materials.back().diffuse.y = readToken<f32>();
		materials.back().diffuse.z = readToken<f32>();
		materials.back().diffuse.w = 1.0f;
	}

	// Ambient Color
	else if (token == MtlTokens::ambient_color) {
		materials.back().ambient.x = readToken<f32>();
		materials.back().ambient.y = readToken<f32>();
		materials.back().ambient.z = readToken<f32>();
	}

	// Specular Color
	else if (token == MtlTokens::specular_color) {
		materials.back().specular.x = readToken<f32>();
		materials.back().specular.y = readToken<f32>();
		materials.back().specular.z = readToken<f32>();
	}

	// Emissive Color
	else if (token == MtlTokens::emissive_color) {
		materials.back().emissive.x = readToken<f32>();
		materials.back().emissive.y = readToken<f32>();
		materials.back().emissive.z = readToken<f32>();
		materials.back().emissive.w = 1.0f;
	}

	// Specular Expononet
	else if (token == MtlTokens::specular_exponent) {
		materials.back().specular.w = readToken<f32>();
	}

	// Optical Density
	else if (token == MtlTokens::optical_density) {
		materials.back().optical_density = readToken<f32>();
	}

	// Dissolve (transparency)
	else if (token == MtlTokens::transparency) {
		materials.back().diffuse.w = readToken<f32>();

		if (materials.back().diffuse.w < 1.0f) {
			materials.back().transparent = true;
		}
	}

	// Illumination
	else if (token == MtlTokens::illumination_model) {
		materials.back().illum = readToken<i32>();
	}

	// Diffuse Map
	else if (token == MtlTokens::diffuse_color_map) {
		const auto name = readToken<std::string>();
		auto file = getFilePath();
		file.replace_filename(name);
		materials.back().map_diffuse = resource_mgr.getOrCreate<Texture>(file);
	}

	// Alpha Map
	else if (token == MtlTokens::alpha_texture_map) {
		const auto name = readToken<std::string>();
		auto file = getFilePath();
		file.replace_filename(name);
		materials.back().map_alpha = resource_mgr.getOrCreate<Texture>(file);
		materials.back().transparent = true;
	}

	// Ambient Map
	else if (token == MtlTokens::ambient_color_map) {
		const auto name = readToken<std::string>();
		auto file = getFilePath();
		file.replace_filename(name);
		materials.back().map_ambient = resource_mgr.getOrCreate<Texture>(file);
	}

	// Specular Map
	else if (token == MtlTokens::specular_color_map) {
		const auto name = readToken<std::string>();
		auto file = getFilePath();
		file.replace_filename(name);
		materials.back().map_specular = resource_mgr.getOrCreate<Texture>(file);
	}

	// Specular Highlight Map
	else if (token == MtlTokens::spec_highlight_map) {
		const auto name = readToken<std::string>();
		auto file = getFilePath();
		file.replace_filename(name);
		materials.back().map_spec_highlight = resource_mgr.getOrCreate<Texture>(file);
	}

	// Bump Map
	else if (token == MtlTokens::bump_map || token == MtlTokens::bump_map2) {
		const auto name = readToken<std::string>();
		auto file = getFilePath();
		file.replace_filename(name);
		materials.back().map_bump = resource_mgr.getOrCreate<Texture>(file);
	}

	readUnusedTokens();
}