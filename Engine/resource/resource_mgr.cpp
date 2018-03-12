#include "stdafx.h"
#include "resource_mgr.h"


ResourceMgr::ResourceMgr(ID3D11Device* device, ID3D11DeviceContext* device_context)
	: device(device)
	, device_context(device_context) {

	model_factory = make_unique<ModelFactory>();
	texture_factory = make_unique<TextureFactory>();
}


Model ResourceMgr::CreateModel(wstring folder, wstring filename, bool right_hand_coords) {
	auto path = folder + filename;

	if (models.find(path) != models.end()) {

		return models.at(path);
	}
	else {
		models[path] = model_factory->CreateModel(device.Get(), *this,
												  folder, filename, right_hand_coords);

		return models.at(path);
	}
}


shared_ptr<Texture> ResourceMgr::CreateTexture(wstring filename) {

	// Check for valid file. Return default texture if failed.
	if (filename.empty() || !fs::exists(filename))
		return CreateTexture(float4(1.0f, 1.0f, 1.0f, 1.0f));

	// Create the texture if it doesn't exist, then return it
	if (textures.find(filename) != textures.end()) {

		return textures.at(filename);
	}
	else {
		textures[filename] = texture_factory->CreateTexture(device.Get(), device_context.Get(), filename);

		return textures.at(filename);
	}
}


shared_ptr<Texture> ResourceMgr::CreateTexture(float4 color) {

	// Convert the float4 into a single hex color value
	u32 texColor = (u32)(color.x * 0xff) + ((u32)(color.y * 0xff) << 8) + ((u32)(color.z * 0xff) << 16) + ((u32)(color.w * 0xff) << 24);

	if (color_textures.find(texColor) != color_textures.end()) {
		return color_textures.at(texColor);
	}
	else {
		color_textures[texColor] = texture_factory->CreateColorTexture(device.Get(), texColor);

		return color_textures.at(texColor);
	}
}


shared_ptr<Texture> ResourceMgr::CreateTexture(vector<wstring> filenames) {

	// Check for valid file, return default texture if failed
	if (filenames.empty())
		return CreateTexture(float4(1.0f, 1.0f, 1.0f, 1.0f));

	for (auto& file : filenames) {
		if (!fs::exists(file))
			return CreateTexture(float4(0.0f, 0.0f, 1.0f, 0.0f));
	}

	// Create the texture if it doesn't exist
	if (texture_arrays.find(filenames) != texture_arrays.end()) {

		return texture_arrays.at(filenames);
	}
	else {
		texture_arrays[filenames] = texture_factory->CreateTexture2DArray(device.Get(), device_context.Get(), filenames);

		return texture_arrays.at(filenames);
	}
}