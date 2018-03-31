#pragma once

#include "util\engine_util.h"
#include "util\datatypes\datatypes.h"
#include "scene\scene.h"


class TestScene final : public Scene {

	public:
		TestScene(ID3D11Device* device,
				  ID3D11DeviceContext* device_context,
				  ResourceMgr& resource_mgr);

		~TestScene() = default;

		void Init(ID3D11Device* device,
				  ID3D11DeviceContext* device_context,
				  ResourceMgr& resource_mgr);

		void Tick(float delta_time);
};