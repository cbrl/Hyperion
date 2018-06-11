#pragma once

#include "os/d3d11.h"
#include "datatypes/datatypes.h"


class Pipeline final {
public:
	// Bind samplers to every stage
	static void bindSamplers(ID3D11DeviceContext& device_context,
	                         u32 start_slot,
	                         u32 num_samplers,
	                         ID3D11SamplerState* const* samplers) {

		CS::bindSamplers(device_context, start_slot, num_samplers, samplers);
		DS::bindSamplers(device_context, start_slot, num_samplers, samplers);
		GS::bindSamplers(device_context, start_slot, num_samplers, samplers);
		HS::bindSamplers(device_context, start_slot, num_samplers, samplers);
		PS::bindSamplers(device_context, start_slot, num_samplers, samplers);
		VS::bindSamplers(device_context, start_slot, num_samplers, samplers);
	}

	// Bind constant buffers to every stage
	static void bindConstantBuffers(ID3D11DeviceContext& device_context,
	                                u32 start_slot,
	                                u32 num_buffers,
	                                ID3D11Buffer* const* buffers) {

		CS::bindConstantBuffers(device_context, start_slot, num_buffers, buffers);
		DS::bindConstantBuffers(device_context, start_slot, num_buffers, buffers);
		GS::bindConstantBuffers(device_context, start_slot, num_buffers, buffers);
		HS::bindConstantBuffers(device_context, start_slot, num_buffers, buffers);
		PS::bindConstantBuffers(device_context, start_slot, num_buffers, buffers);
		VS::bindConstantBuffers(device_context, start_slot, num_buffers, buffers);
	}

	// Bind shader resource views to every stage
	static void bindSRVs(ID3D11DeviceContext& device_context,
	                     u32 start_slot,
	                     u32 num_views,
	                     ID3D11ShaderResourceView* const* srvs) {

		CS::bindSRVs(device_context, start_slot, num_views, srvs);
		DS::bindSRVs(device_context, start_slot, num_views, srvs);
		GS::bindSRVs(device_context, start_slot, num_views, srvs);
		HS::bindSRVs(device_context, start_slot, num_views, srvs);
		PS::bindSRVs(device_context, start_slot, num_views, srvs);
		VS::bindSRVs(device_context, start_slot, num_views, srvs);
	}


	//----------------------------------------------------------------------------------
	// Input Assembler
	//----------------------------------------------------------------------------------
	struct IA {
		static void bindIndexBuffer(ID3D11DeviceContext& device_context,
		                            ID3D11Buffer* buffer,
		                            DXGI_FORMAT format,
		                            u32 offset) {

			device_context.IASetIndexBuffer(buffer, format, offset);
		}

		static void bindVertexBuffers(ID3D11DeviceContext& device_context,
		                              u32 start_slot,
		                              u32 num_buffers,
		                              ID3D11Buffer* const* buffers,
		                              const u32* strides,
		                              const u32* offsets) {

			device_context.IASetVertexBuffers(start_slot, num_buffers, buffers, strides, offsets);
		}

		static void bindInputLayout(ID3D11DeviceContext& device_context,
		                            ID3D11InputLayout* layout) {
			device_context.IASetInputLayout(layout);
		}

		static void bindPrimitiveTopology(ID3D11DeviceContext& device_context,
		                                  D3D11_PRIMITIVE_TOPOLOGY topology) {
			device_context.IASetPrimitiveTopology(topology);
		}
	};


	//----------------------------------------------------------------------------------
	// Output Merger
	//----------------------------------------------------------------------------------
	struct OM {
		static void bindBlendState(ID3D11DeviceContext& device_context,
		                           ID3D11BlendState* state,
		                           const float blend_factor[4],
		                           u32 sample_mask) {

			device_context.OMSetBlendState(state, blend_factor, sample_mask);
		}

		static void bindDepthStencilState(ID3D11DeviceContext& device_context,
		                                  ID3D11DepthStencilState* state,
		                                  u32 stencil_ref) {

			device_context.OMSetDepthStencilState(state, stencil_ref);
		}

		static void bindRTVs(ID3D11DeviceContext& device_context,
		                     u32 num_views,
		                     ID3D11RenderTargetView* const* rtvs,
		                     ID3D11DepthStencilView* dsv) {

			if (rtvs) {
				device_context.OMSetRenderTargets(num_views, rtvs, dsv);
			}
			else {
				device_context.OMSetRenderTargets(0, nullptr, dsv);
			}
		}

		static void bindRTVsAndUTVs(ID3D11DeviceContext& device_context,
		                            u32 num_rtvs,
		                            ID3D11RenderTargetView* const* rtvs,
		                            ID3D11DepthStencilView* dsv,
		                            u32 start_slot,
		                            u32 num_uavs,
		                            ID3D11UnorderedAccessView* const* uavs,
		                            u32 initial_counts) {

			device_context.OMSetRenderTargetsAndUnorderedAccessViews(num_rtvs,
			                                                         rtvs,
			                                                         dsv,
			                                                         start_slot,
			                                                         num_uavs,
			                                                         uavs,
			                                                         &initial_counts);
		}

		static void clearRTV(ID3D11DeviceContext& device_context,
		                     ID3D11RenderTargetView* rtv) {

			float color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
			clearRTV(device_context, rtv, color);
		}

		static void clearRTV(ID3D11DeviceContext& device_context,
		                     ID3D11RenderTargetView* rtv,
		                     const float color[4]) {

			device_context.ClearRenderTargetView(rtv, color);
		}

		static void clearDSV(ID3D11DeviceContext& device_context,
		                     ID3D11DepthStencilView* dsv) {

			device_context.ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}
	};


	//----------------------------------------------------------------------------------
	// Compute Stage
	//----------------------------------------------------------------------------------
	struct CS {
		static void bindShader(ID3D11DeviceContext& device_context,
		                       ID3D11ComputeShader* shader,
		                       ID3D11ClassInstance* const* instances,
		                       u32 num_instances) {

			device_context.CSSetShader(shader, instances, num_instances);
		}

		static void bindSamplers(ID3D11DeviceContext& device_context,
		                         u32 start_slot,
		                         u32 num_samplers,
		                         ID3D11SamplerState* const* samplers) {

			device_context.CSSetSamplers(start_slot, num_samplers, samplers);
		}

		static void bindConstantBuffers(ID3D11DeviceContext& device_context,
		                                u32 start_slot,
		                                u32 num_buffers,
		                                ID3D11Buffer* const* buffers) {

			device_context.CSSetConstantBuffers(start_slot, num_buffers, buffers);
		}

		static void bindSRVs(ID3D11DeviceContext& device_context,
		                     u32 start_slot,
		                     u32 num_views,
		                     ID3D11ShaderResourceView* const* srvs) {

			device_context.CSSetShaderResources(start_slot, num_views, srvs);
		}

		static void bindUaVs(ID3D11DeviceContext& device_context,
		                     u32 start_slot,
		                     u32 num_uavs,
		                     ID3D11UnorderedAccessView* const* uavs,
		                     u32 initial_counts) {

			device_context.CSSetUnorderedAccessViews(start_slot, num_uavs, uavs, &initial_counts);
		}
	};


	//----------------------------------------------------------------------------------
	// Domain Stage
	//----------------------------------------------------------------------------------
	struct DS {
		static void bindShader(ID3D11DeviceContext& device_context,
		                       ID3D11DomainShader* shader,
		                       ID3D11ClassInstance* const* instances,
		                       u32 num_instances) {

			device_context.DSSetShader(shader, instances, num_instances);
		}

		static void bindSamplers(ID3D11DeviceContext& device_context,
		                         u32 start_slot,
		                         u32 num_samplers,
		                         ID3D11SamplerState* const* samplers) {

			device_context.DSSetSamplers(start_slot, num_samplers, samplers);
		}

		static void bindConstantBuffers(ID3D11DeviceContext& device_context,
		                                u32 start_slot,
		                                u32 num_buffers,
		                                ID3D11Buffer* const* buffers) {

			device_context.DSSetConstantBuffers(start_slot, num_buffers, buffers);
		}

		static void bindSRVs(ID3D11DeviceContext& device_context,
		                     u32 start_slot,
		                     u32 num_views,
		                     ID3D11ShaderResourceView* const* srvs) {

			device_context.DSSetShaderResources(start_slot, num_views, srvs);
		}
	};


	//----------------------------------------------------------------------------------
	// Geometry Stage
	//----------------------------------------------------------------------------------
	struct GS {
		static void bindShader(ID3D11DeviceContext& device_context,
		                       ID3D11GeometryShader* shader,
		                       ID3D11ClassInstance* const* instances,
		                       u32 num_instances) {

			device_context.GSSetShader(shader, instances, num_instances);
		}

		static void bindSamplers(ID3D11DeviceContext& device_context,
		                         u32 start_slot,
		                         u32 num_samplers,
		                         ID3D11SamplerState* const* samplers) {

			device_context.GSSetSamplers(start_slot, num_samplers, samplers);
		}

		static void bindConstantBuffers(ID3D11DeviceContext& device_context,
		                                u32 start_slot,
		                                u32 num_buffers,
		                                ID3D11Buffer* const* buffers) {

			device_context.GSSetConstantBuffers(start_slot, num_buffers, buffers);
		}

		static void bindSRVs(ID3D11DeviceContext& device_context,
		                     u32 start_slot,
		                     u32 num_views,
		                     ID3D11ShaderResourceView* const* srvs) {

			device_context.GSSetShaderResources(start_slot, num_views, srvs);
		}
	};


	//----------------------------------------------------------------------------------
	// Hull Stage
	//----------------------------------------------------------------------------------
	struct HS {
		static void bindShader(ID3D11DeviceContext& device_context,
		                       ID3D11HullShader* shader,
		                       ID3D11ClassInstance* const* instances,
		                       u32 num_instances) {

			device_context.HSSetShader(shader, instances, num_instances);
		}

		static void bindSamplers(ID3D11DeviceContext& device_context,
		                         u32 start_slot,
		                         u32 num_samplers,
		                         ID3D11SamplerState* const* samplers) {

			device_context.HSSetSamplers(start_slot, num_samplers, samplers);
		}

		static void bindConstantBuffers(ID3D11DeviceContext& device_context,
		                                u32 start_slot,
		                                u32 num_buffers,
		                                ID3D11Buffer* const* buffers) {

			device_context.HSSetConstantBuffers(start_slot, num_buffers, buffers);
		}

		static void bindSRVs(ID3D11DeviceContext& device_context,
		                     u32 start_slot,
		                     u32 num_views,
		                     ID3D11ShaderResourceView* const* srvs) {

			device_context.HSSetShaderResources(start_slot, num_views, srvs);
		}
	};


	//----------------------------------------------------------------------------------
	// Pixel Stage
	//----------------------------------------------------------------------------------
	struct PS {
		static void bindShader(ID3D11DeviceContext& device_context,
		                       ID3D11PixelShader* shader,
		                       ID3D11ClassInstance* const* instances,
		                       u32 num_instances) {

			device_context.PSSetShader(shader, instances, num_instances);
		}

		static void bindSamplers(ID3D11DeviceContext& device_context,
		                         u32 start_slot,
		                         u32 num_samplers,
		                         ID3D11SamplerState* const* samplers) {

			device_context.PSSetSamplers(start_slot, num_samplers, samplers);
		}

		static void bindConstantBuffers(ID3D11DeviceContext& device_context,
		                                u32 start_slot,
		                                u32 num_buffers,
		                                ID3D11Buffer* const* buffers) {

			device_context.PSSetConstantBuffers(start_slot, num_buffers, buffers);
		}

		static void bindSRVs(ID3D11DeviceContext& device_context,
		                     u32 start_slot,
		                     u32 num_views,
		                     ID3D11ShaderResourceView* const* srvs) {

			device_context.PSSetShaderResources(start_slot, num_views, srvs);
		}
	};


	//----------------------------------------------------------------------------------
	// Rasterizer Stage
	//----------------------------------------------------------------------------------
	struct RS {
		static void bindScissorRects(ID3D11DeviceContext& device_context,
		                             u32 num_rects,
		                             const D3D11_RECT* rects) {

			device_context.RSSetScissorRects(num_rects, rects);
		}

		static void bindState(ID3D11DeviceContext& device_context,
		                      ID3D11RasterizerState* state) {

			device_context.RSSetState(state);
		}

		static void bindViewports(ID3D11DeviceContext& device_context,
		                          u32 num_viewports,
		                          const D3D11_VIEWPORT* viewports) {

			device_context.RSSetViewports(num_viewports, viewports);
		}
	};


	//----------------------------------------------------------------------------------
	// Vertex Stage
	//----------------------------------------------------------------------------------
	struct VS {
		static void bindShader(ID3D11DeviceContext& device_context,
		                       ID3D11VertexShader* shader,
		                       ID3D11ClassInstance* const* instances,
		                       u32 num_instances) {

			device_context.VSSetShader(shader, instances, num_instances);
		}

		static void bindSamplers(ID3D11DeviceContext& device_context,
		                         u32 start_slot,
		                         u32 num_samplers,
		                         ID3D11SamplerState* const* samplers) {

			device_context.VSSetSamplers(start_slot, num_samplers, samplers);
		}

		static void bindConstantBuffers(ID3D11DeviceContext& device_context,
		                                u32 start_slot,
		                                u32 num_buffers,
		                                ID3D11Buffer* const* buffers) {

			device_context.VSSetConstantBuffers(start_slot, num_buffers, buffers);
		}

		static void bindSRVs(ID3D11DeviceContext& device_context,
		                     u32 start_slot,
		                     u32 num_views,
		                     ID3D11ShaderResourceView* const* srvs) {

			device_context.VSSetShaderResources(start_slot, num_views, srvs);
		}
	};
};
