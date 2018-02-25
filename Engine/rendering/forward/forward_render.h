#pragma once

#include "util\math\math.h"
#include "util\datatypes\datatypes.h"
#include "geometry\frustum\frustum.h"
#include "shader\shader.h"
#include "shader\hlsl.h"
#include "rendering\buffer\buffers.h"
#include "rendering\buffer\constant_buffer.h"
#include "rendering\buffer\structured_buffer.h"
#include "scene\scene.h"


using namespace DirectX;


class ForwardRenderer {
	public:
		ForwardRenderer();
		~ForwardRenderer() = default;

		void Render(Scene& scene);


	private:
		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> device_context;

		unique_ptr<PixelShader>  pixel_shader;
		unique_ptr<VertexShader> vertex_shader;

		ConstantBuffer<ModelBuffer>        model_buffer;
		ConstantBuffer<LightBuffer>        light_buffer;
		StructuredBuffer<DirectionalLight> directional_light_buffer;
		StructuredBuffer<PointLight>       point_light_buffer;
		StructuredBuffer<SpotLight>        spot_light_buffer;
};