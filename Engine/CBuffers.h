#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl\client.h>
#include "EngineUtil.h"
#include "BufferTypes.h"
#include "ConstantBuffer.h"

using std::unique_ptr;
using std::make_unique;
using Microsoft::WRL::ComPtr;

struct CBuffers {
		CBuffers(const ComPtr<ID3D11Device>& device) {
			matrixBuffer = make_unique<ConstantBuffer<MatrixBuffer>>(device);

			cameraBuffer = make_unique<ConstantBuffer<CameraBuffer>>(device);

			lightBuffer = make_unique<ConstantBuffer<LightBuffer>>(device);
		}

		~CBuffers() = default;

		unique_ptr<ConstantBuffer<MatrixBuffer>> matrixBuffer;
		unique_ptr<ConstantBuffer<CameraBuffer>> cameraBuffer;
		unique_ptr<ConstantBuffer<LightBuffer>>  lightBuffer;
};