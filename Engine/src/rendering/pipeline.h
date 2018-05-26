#pragma once

#include <d3d11.h>
#include "datatypes\datatypes.h"


class Pipeline final {
	public:
		// Bind samplers to every stage
		static void BindSamplers(ID3D11DeviceContext& device_context,
								 u32 start_slot,
								 u32 num_samplers,
								 ID3D11SamplerState* const* samplers) {

			CS::BindSamplers(device_context, start_slot, num_samplers, samplers);
			DS::BindSamplers(device_context, start_slot, num_samplers, samplers);
			GS::BindSamplers(device_context, start_slot, num_samplers, samplers);
			HS::BindSamplers(device_context, start_slot, num_samplers, samplers);
			PS::BindSamplers(device_context, start_slot, num_samplers, samplers);
			VS::BindSamplers(device_context, start_slot, num_samplers, samplers);
		}

		// Bind constant buffers to every stage
		static void BindConstantBuffers(ID3D11DeviceContext& device_context,
										u32 start_slot,
										u32 num_buffers,
										ID3D11Buffer* const* buffers) {

			CS::BindConstantBuffers(device_context, start_slot, num_buffers, buffers);
			DS::BindConstantBuffers(device_context, start_slot, num_buffers, buffers);
			GS::BindConstantBuffers(device_context, start_slot, num_buffers, buffers);
			HS::BindConstantBuffers(device_context, start_slot, num_buffers, buffers);
			PS::BindConstantBuffers(device_context, start_slot, num_buffers, buffers);
			VS::BindConstantBuffers(device_context, start_slot, num_buffers, buffers);
		}

		// Bind shader resource views to every stage
		static void BindSRVs(ID3D11DeviceContext& device_context,
							 u32 start_slot,
							 u32 num_views,
							 ID3D11ShaderResourceView* const* srvs) {

			CS::BindSRVs(device_context, start_slot, num_views, srvs);
			DS::BindSRVs(device_context, start_slot, num_views, srvs);
			GS::BindSRVs(device_context, start_slot, num_views, srvs);
			HS::BindSRVs(device_context, start_slot, num_views, srvs);
			PS::BindSRVs(device_context, start_slot, num_views, srvs);
			VS::BindSRVs(device_context, start_slot, num_views, srvs);
		}


		//----------------------------------------------------------------------------------
		// Input Assembler
		//----------------------------------------------------------------------------------
		struct IA {
			static void BindIndexBuffer(ID3D11DeviceContext& device_context,
										ID3D11Buffer* buffer,
										DXGI_FORMAT format,
										u32 offset) {

				device_context.IASetIndexBuffer(buffer, format, offset);
			}

			static void BindVertexBuffers(ID3D11DeviceContext& device_context,
										  u32 start_slot,
										  u32 num_buffers,
										  ID3D11Buffer* const* buffers,
										  const u32* strides,
										  const u32* offsets) {

				device_context.IASetVertexBuffers(start_slot, num_buffers, buffers, strides, offsets);
			}

			static void BindInputLayout(ID3D11DeviceContext& device_context,
										ID3D11InputLayout* layout) {
				device_context.IASetInputLayout(layout);
			}

			static void BindPrimitiveTopology(ID3D11DeviceContext& device_context,
											  D3D11_PRIMITIVE_TOPOLOGY topology) {
				device_context.IASetPrimitiveTopology(topology);
			}
		};


		//----------------------------------------------------------------------------------
		// Output Merger
		//----------------------------------------------------------------------------------
		struct OM {
			static void BindBlendState(ID3D11DeviceContext& device_context,
									   ID3D11BlendState* state,
									   const float blend_factor[4],
									   u32 sample_mask) {

				device_context.OMSetBlendState(state, blend_factor, sample_mask);
			}

			static void BindDepthStencilState(ID3D11DeviceContext& device_context,
											  ID3D11DepthStencilState* state,
											  u32 stencil_ref) {

				device_context.OMSetDepthStencilState(state, stencil_ref);
			}

			static void BindRTVs(ID3D11DeviceContext& device_context,
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

			static void BindRTVsAndUAVs(ID3D11DeviceContext& device_context,
										u32 num_rtvs,
										ID3D11RenderTargetView* const* rtvs,
										ID3D11DepthStencilView* dsv,
										u32 start_slot,
										u32 num_uavs,
										ID3D11UnorderedAccessView* const* uavs,
										u32 initial_counts) {

				device_context.OMSetRenderTargetsAndUnorderedAccessViews(num_rtvs, rtvs, dsv, start_slot, num_uavs, uavs, &initial_counts);
			}

			static void ClearRTV(ID3D11DeviceContext& device_context,
								 ID3D11RenderTargetView* rtv) {

				float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
				ClearRTV(device_context, rtv, color);
			}

			static void ClearRTV(ID3D11DeviceContext& device_context,
								 ID3D11RenderTargetView* rtv,
								 const float color[4]) {

				device_context.ClearRenderTargetView(rtv, color);
			}

			static void ClearDSV(ID3D11DeviceContext& device_context,
								 ID3D11DepthStencilView* dsv) {

				device_context.ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			}
		};
	

		//----------------------------------------------------------------------------------
		// Compute Stage
		//----------------------------------------------------------------------------------
		struct CS {
			static void BindShader(ID3D11DeviceContext& device_context,
								   ID3D11ComputeShader* shader,
								   ID3D11ClassInstance* const* instances,
								   u32 num_instances) {

				device_context.CSSetShader(shader, instances, num_instances);
			}

			static void BindSamplers(ID3D11DeviceContext& device_context,
									 u32 start_slot,
									 u32 num_samplers,
									 ID3D11SamplerState* const* samplers) {

				device_context.CSSetSamplers(start_slot, num_samplers, samplers);
			}

			static void BindConstantBuffers(ID3D11DeviceContext& device_context,
											u32 start_slot,
											u32 num_buffers,
											ID3D11Buffer* const* buffers) {

				device_context.CSSetConstantBuffers(start_slot, num_buffers, buffers);
			}

			static void BindSRVs(ID3D11DeviceContext& device_context,
								 u32 start_slot,
								 u32 num_views,
								 ID3D11ShaderResourceView* const* srvs) {

				device_context.CSSetShaderResources(start_slot, num_views, srvs);
			}

			static void BindUAVs(ID3D11DeviceContext& device_context,
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
			static void BindShader(ID3D11DeviceContext& device_context,
								   ID3D11DomainShader* shader,
								   ID3D11ClassInstance* const* instances,
								   u32 num_instances) {

				device_context.DSSetShader(shader, instances, num_instances);
			}

			static void BindSamplers(ID3D11DeviceContext& device_context,
									 u32 start_slot,
									 u32 num_samplers,
									 ID3D11SamplerState* const* samplers) {

				device_context.DSSetSamplers(start_slot, num_samplers, samplers);
			}

			static void BindConstantBuffers(ID3D11DeviceContext& device_context,
											u32 start_slot,
											u32 num_buffers,
											ID3D11Buffer* const* buffers) {

				device_context.DSSetConstantBuffers(start_slot, num_buffers, buffers);
			}

			static void BindSRVs(ID3D11DeviceContext& device_context,
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
			static void BindShader(ID3D11DeviceContext& device_context,
								   ID3D11GeometryShader* shader,
								   ID3D11ClassInstance* const* instances,
								   u32 num_instances) {

				device_context.GSSetShader(shader, instances, num_instances);
			}

			static void BindSamplers(ID3D11DeviceContext& device_context,
									 u32 start_slot,
									 u32 num_samplers,
									 ID3D11SamplerState* const* samplers) {

				device_context.GSSetSamplers(start_slot, num_samplers, samplers);
			}

			static void BindConstantBuffers(ID3D11DeviceContext& device_context,
											u32 start_slot,
											u32 num_buffers,
											ID3D11Buffer* const* buffers) {

				device_context.GSSetConstantBuffers(start_slot, num_buffers, buffers);
			}

			static void BindSRVs(ID3D11DeviceContext& device_context,
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
			static void BindShader(ID3D11DeviceContext& device_context,
								   ID3D11HullShader* shader,
								   ID3D11ClassInstance* const* instances,
								   u32 num_instances) {

				device_context.HSSetShader(shader, instances, num_instances);
			}

			static void BindSamplers(ID3D11DeviceContext& device_context,
									 u32 start_slot,
									 u32 num_samplers,
									 ID3D11SamplerState* const* samplers) {

				device_context.HSSetSamplers(start_slot, num_samplers, samplers);
			}

			static void BindConstantBuffers(ID3D11DeviceContext& device_context,
											u32 start_slot,
											u32 num_buffers,
											ID3D11Buffer* const* buffers) {

				device_context.HSSetConstantBuffers(start_slot, num_buffers, buffers);
			}

			static void BindSRVs(ID3D11DeviceContext& device_context,
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
			static void BindShader(ID3D11DeviceContext& device_context,
								   ID3D11PixelShader* shader,
								   ID3D11ClassInstance* const* instances,
								   u32 num_instances) {

				device_context.PSSetShader(shader, instances, num_instances);
			}

			static void BindSamplers(ID3D11DeviceContext& device_context,
									 u32 start_slot,
									 u32 num_samplers,
									 ID3D11SamplerState* const* samplers) {

				device_context.PSSetSamplers(start_slot, num_samplers, samplers);
			}

			static void BindConstantBuffers(ID3D11DeviceContext& device_context,
											u32 start_slot,
											u32 num_buffers,
											ID3D11Buffer* const* buffers) {

				device_context.PSSetConstantBuffers(start_slot, num_buffers, buffers);
			}

			static void BindSRVs(ID3D11DeviceContext& device_context,
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
			static void BindScissorRects(ID3D11DeviceContext& device_context,
										 u32 num_rects, const D3D11_RECT* rects) {

				device_context.RSSetScissorRects(num_rects, rects);
			}

			static void BindState(ID3D11DeviceContext& device_context,
								  ID3D11RasterizerState* state) {

				device_context.RSSetState(state);
			}

			static void BindViewports(ID3D11DeviceContext& device_context,
									  u32 num_viewports, const D3D11_VIEWPORT* viewports) {

				device_context.RSSetViewports(num_viewports, viewports);
			}
		};


		//----------------------------------------------------------------------------------
		// Vertex Stage
		//----------------------------------------------------------------------------------
		struct VS {
			static void BindShader(ID3D11DeviceContext& device_context,
								   ID3D11VertexShader* shader,
								   ID3D11ClassInstance* const* instances,
								   u32 num_instances) {

				device_context.VSSetShader(shader, instances, num_instances);
			}

			static void BindSamplers(ID3D11DeviceContext& device_context,
									 u32 start_slot,
									 u32 num_samplers,
									 ID3D11SamplerState* const* samplers) {

				device_context.VSSetSamplers(start_slot, num_samplers, samplers);
			}

			static void BindConstantBuffers(ID3D11DeviceContext& device_context,
											u32 start_slot,
											u32 num_buffers,
											ID3D11Buffer* const* buffers) {

				device_context.VSSetConstantBuffers(start_slot, num_buffers, buffers);
			}

			static void BindSRVs(ID3D11DeviceContext& device_context,
								 u32 start_slot,
								 u32 num_views,
								 ID3D11ShaderResourceView* const* srvs) {

				device_context.VSSetShaderResources(start_slot, num_views, srvs);
			}
		};
};