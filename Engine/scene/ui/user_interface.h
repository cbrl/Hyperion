#pragma once

#include "util\engine_util.h"
#include "util\datatypes\datatypes.h"
#include "util\io\io.h"
#include "imgui\imgui_impl_dx11.h"
#include "rendering\buffer\buffers.h"
#include "resource\model\blueprint_factory.h"
#include "ecs_data\components\components.h"

class Scene;
class Engine;

class UserInterface {
	public:
		UserInterface() = default;
		~UserInterface() = default;

		void Draw(Engine& engine);


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

		void DrawSceneDetails(Scene& scene);
		void DrawLightList(Scene& scene);
		void DrawLightDetails(DirectionalLightBuffer& light);
		void DrawLightDetails(PointLightBuffer& light);
		void DrawLightDetails(SpotLightBuffer& light);

		//void DrawCameraDetails(PlayerCamera& camera);

		void DrawPopups(ID3D11Device* device, ResourceMgr& resource_mgr, Scene& scene, Popups popup);
};