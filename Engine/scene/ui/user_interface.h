#pragma once

#include "util\engine_util.h"
#include "util\datatypes\datatypes.h"
#include "util\io\io.h"
#include "imgui\imgui_impl_dx11.h"
#include "rendering\buffer\buffers.h"
#include "resource\model\model.h"
#include "camera\camera.h"

class Scene;
class System;

class UserInterface {
	public:
		UserInterface() = default;
		~UserInterface() = default;

		void Draw(System& system);


	private:
		void DrawMenu(System& system, Scene& scene);
		void DrawObjectList(Scene& scene);
		void DrawObjectDetails(Scene& scene);

		void DrawModelList(Scene& scene);
		void DrawModelDetails(Model& model);
		void DrawChildModelDetails(ChildModel& child);

		void DrawLightList(Scene& scene);
		void DrawLightDetails(DirectionalLight& light);
		void DrawLightDetails(PointLight& light);
		void DrawLightDetails(SpotLight& light);

		void DrawCameraDetails(Camera& camera);
};