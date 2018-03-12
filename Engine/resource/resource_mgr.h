#pragma once

#include "util\datatypes\datatypes.h"
#include "util\string\string.h"
#include "resource\mesh\mesh.h"
#include "resource\model\model.h"
#include "loader\obj_loader.h"
#include "resource\model\model_factory.h"
#include "resource\texture\texture_factory.h"


class ResourceMgr {
	public:
		ResourceMgr(ID3D11Device* device, ID3D11DeviceContext* device_context);
		~ResourceMgr() = default;

		Model CreateModel(wstring folder, wstring filename, bool right_hand_coords);

		shared_ptr<Texture> CreateTexture(wstring filename);
		shared_ptr<Texture> CreateTexture(float4 color);
		shared_ptr<Texture> CreateTexture(vector<wstring> filenames);


	private:
		// Device and device context
		ComPtr<ID3D11Device>        device;
		ComPtr<ID3D11DeviceContext> device_context;

		// Resource factories
		unique_ptr<TextureFactory> texture_factory;
		unique_ptr<ModelFactory>   model_factory;

		// Models
		map<wstring, Model> models;

		// Textures
		map<wstring, shared_ptr<Texture>>         textures;
		map<u32, shared_ptr<Texture>>             color_textures;
		map<vector<wstring>, shared_ptr<Texture>> texture_arrays;
};