#pragma once

#include "util\engine_util.h"
#include "util\datatypes\datatypes.h"
#include "util\io\io.h"
#include "imgui\imgui_impl_dx11.h"
#include "rendering\buffer\buffers.h"
#include "resource\model\model.h"
#include "resource\model\blueprint_factory.h"
#include "camera\player_camera.h"

class Scene;
class System;

class UserInterface {
	public:
		UserInterface() = default;
		~UserInterface() = default;

		void Draw(System& system);


	private:
		enum class Popups {
			Null,
			NewModelCopy,
			NewCube,
			NewBox,
			NewSphere,
			NewGeoSphere,
			NewCylinder,
			NewTorus,
			NewCone,
			NewTetrahedron,
			NewOctahedron,
			NewDodecahedron,
			NewIcosahedron
		};


	private:
		void DrawMenu(ID3D11Device* device, ResourceMgr& resource_mgr, Scene& scene, Popups& popup);
		void DrawObjectLists(Scene& scene);
		void DrawObjectDetails(Scene& scene);

		void DrawModelList(Scene& scene);
		void DrawModelDetails(Model& model);
		void DrawChildModelDetails(ModelChild& child);

		void DrawLightList(Scene& scene);
		void DrawLightDetails(DirectionalLight& light);
		void DrawLightDetails(PointLight& light);
		void DrawLightDetails(SpotLight& light);

		void DrawCameraDetails(PlayerCamera& camera);

		void DrawPopups(ID3D11Device* device, Scene& scene, Popups popup);
};