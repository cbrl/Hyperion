#pragma once

#include "util\engine_util.h"
#include "util\datatypes\datatypes.h"
#include "imgui\imgui_impl_dx11.h"
#include "scene\scene.h"


class UIRenderer final {
	public:
		UIRenderer() = default;
		~UIRenderer() = default;

		void Render(Scene& scene);

		void DrawMenu(Scene& scene);
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