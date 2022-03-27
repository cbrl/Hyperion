module;

#include <span>
#include <d3d11.h>

#include "datatypes/scalar_types.h"
#include "datatypes/vector_types.h"

export module rendering.pipeline;


export namespace render {

class Pipeline final {
public:
	Pipeline() = delete;
	~Pipeline() = delete;

	// Bind a sampler to every stage
	static void bindSampler(ID3D11DeviceContext& device_context,
	                        u32 start_slot,
	                        ID3D11SamplerState* sampler) {

		CS::bindSampler(device_context, start_slot, sampler);
		DS::bindSampler(device_context, start_slot, sampler);
		GS::bindSampler(device_context, start_slot, sampler);
		HS::bindSampler(device_context, start_slot, sampler);
		PS::bindSampler(device_context, start_slot, sampler);
		VS::bindSampler(device_context, start_slot, sampler);
	}

	// Bind samplers to every stage
	static void bindSamplers(ID3D11DeviceContext& device_context,
	                         u32 start_slot,
	                         std::span<ID3D11SamplerState* const> samplers) {

		CS::bindSamplers(device_context, start_slot, samplers);
		DS::bindSamplers(device_context, start_slot, samplers);
		GS::bindSamplers(device_context, start_slot, samplers);
		HS::bindSamplers(device_context, start_slot, samplers);
		PS::bindSamplers(device_context, start_slot, samplers);
		VS::bindSamplers(device_context, start_slot, samplers);
	}

	// Bind a constant buffer to every stage
	static void bindConstantBuffer(ID3D11DeviceContext& device_context,
									u32 start_slot,
									ID3D11Buffer* buffer) {

		CS::bindConstantBuffer(device_context, start_slot, buffer);
		DS::bindConstantBuffer(device_context, start_slot, buffer);
		GS::bindConstantBuffer(device_context, start_slot, buffer);
		HS::bindConstantBuffer(device_context, start_slot, buffer);
		PS::bindConstantBuffer(device_context, start_slot, buffer);
		VS::bindConstantBuffer(device_context, start_slot, buffer);
	}

	// Bind constant buffers to every stage
	static void bindConstantBuffers(ID3D11DeviceContext& device_context,
	                                u32 start_slot,
	                                std::span<ID3D11Buffer* const> buffers) {

		CS::bindConstantBuffers(device_context, start_slot, buffers);
		DS::bindConstantBuffers(device_context, start_slot, buffers);
		GS::bindConstantBuffers(device_context, start_slot, buffers);
		HS::bindConstantBuffers(device_context, start_slot, buffers);
		PS::bindConstantBuffers(device_context, start_slot, buffers);
		VS::bindConstantBuffers(device_context, start_slot, buffers);
	}

	// Bind a shader resource view to every stage
	static void bindSRV(ID3D11DeviceContext& device_context,
						 u32 start_slot,
						 ID3D11ShaderResourceView* srv) {

		CS::bindSRV(device_context, start_slot, srv);
		DS::bindSRV(device_context, start_slot, srv);
		GS::bindSRV(device_context, start_slot, srv);
		HS::bindSRV(device_context, start_slot, srv);
		PS::bindSRV(device_context, start_slot, srv);
		VS::bindSRV(device_context, start_slot, srv);
	}

	// Bind shader resource views to every stage
	static void bindSRVs(ID3D11DeviceContext& device_context,
	                     u32 start_slot,
	                     std::span<ID3D11ShaderResourceView* const> srvs) {

		CS::bindSRVs(device_context, start_slot, srvs);
		DS::bindSRVs(device_context, start_slot, srvs);
		GS::bindSRVs(device_context, start_slot, srvs);
		HS::bindSRVs(device_context, start_slot, srvs);
		PS::bindSRVs(device_context, start_slot, srvs);
		VS::bindSRVs(device_context, start_slot, srvs);
	}

	static void draw(ID3D11DeviceContext& device_context,
	                 u32 vertex_count,
	                 u32 vertex_start) {

		device_context.Draw(vertex_count, vertex_start);
	}

	static void drawIndexed(ID3D11DeviceContext& device_context,
	                        u32 index_count,
	                        u32 index_start,
	                        u32 base_vertex = 0) {

		device_context.DrawIndexed(index_count, index_start, base_vertex);
	}

	static void drawInstanced(ID3D11DeviceContext& device_context,
	                          u32 vertex_count_per_instance,
	                          u32 instance_count,
	                          u32 vertex_start,
	                          u32 instance_start = 0) {

		device_context.DrawInstanced(vertex_count_per_instance,
		                             instance_count,
		                             vertex_start,
		                             instance_start);
	}

	static void drawIndexedInstanced(ID3D11DeviceContext& device_context,
	                                 u32 index_count_per_instance,
	                                 u32 instance_count,
	                                 u32 index_start,
	                                 u32 base_vertex = 0,
	                                 u32 instance_start = 0) {

		device_context.DrawIndexedInstanced(index_count_per_instance,
		                                    instance_count,
		                                    index_start,
		                                    base_vertex,
		                                    instance_start);
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


		static void bindVertexBuffer(ID3D11DeviceContext& device_context,
		                             u32 slot,
		                             ID3D11Buffer* buffer,
		                             u32 stride,
		                             u32 offset) {

			ID3D11Buffer* buffers[1] = { buffer };
			bindVertexBuffers(device_context, slot, std::span{buffers}, &stride, &offset);
		}

		static void bindVertexBuffers(ID3D11DeviceContext& device_context,
		                              u32 start_slot,
		                              std::span<ID3D11Buffer* const> buffers,
		                              const u32* strides,
		                              const u32* offsets) {

			device_context.IASetVertexBuffers(start_slot, static_cast<UINT>(buffers.size()), buffers.data(), strides, offsets);
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
		                           const f32 blend_factor[4],
		                           u32 sample_mask) {

			device_context.OMSetBlendState(state, blend_factor, sample_mask);
		}

		static void bindDepthStencilState(ID3D11DeviceContext& device_context,
		                                  ID3D11DepthStencilState* state,
		                                  u32 stencil_ref) {

			device_context.OMSetDepthStencilState(state, stencil_ref);
		}

		static void bindRTVAndDSV(ID3D11DeviceContext& device_context,
		                          ID3D11RenderTargetView* rtv,
		                          ID3D11DepthStencilView* dsv) {

			if (rtv) {
				ID3D11RenderTargetView* const rtvs[1] = { rtv };
				bindRTVsAndDSV(device_context, std::span{rtvs}, dsv);
			}
			else {
				bindRTVsAndDSV(device_context, {}, dsv);
			}
		}

		static void bindRTVsAndDSV(ID3D11DeviceContext& device_context,
		                           std::span<ID3D11RenderTargetView* const> rtvs,
		                           ID3D11DepthStencilView* dsv) {

			device_context.OMSetRenderTargets(static_cast<UINT>(rtvs.size()), rtvs.data(), dsv);
		}

		static void bindRTVsAndUTVs(ID3D11DeviceContext& device_context,
		                            std::span<ID3D11RenderTargetView* const> rtvs,
		                            ID3D11DepthStencilView* dsv,
		                            u32 start_slot,
		                            std::span<ID3D11UnorderedAccessView* const> uavs,
		                            u32 initial_counts) {

			device_context.OMSetRenderTargetsAndUnorderedAccessViews(static_cast<UINT>(rtvs.size()),
			                                                         rtvs.data(),
			                                                         dsv,
			                                                         start_slot,
			                                                         static_cast<UINT>(uavs.size()),
			                                                         uavs.data(),
			                                                         &initial_counts);
		}

		static void clearRTV(ID3D11DeviceContext& device_context,
		                     ID3D11RenderTargetView* rtv) {

			static const f32 color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
			clearRTV(device_context, rtv, color);
		}

		static void clearRTV(ID3D11DeviceContext& device_context,
		                     ID3D11RenderTargetView* rtv,
		                     const f32 color[4]) {

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
		                       std::span<ID3D11ClassInstance* const> instances) {

			device_context.CSSetShader(shader, instances.data(), static_cast<UINT>(instances.size()));
		}

		static void bindSampler(ID3D11DeviceContext& device_context,
		                        u32 start_slot,
		                        ID3D11SamplerState* sampler) {

			ID3D11SamplerState* const samplers[1] = { sampler };
			bindSamplers(device_context, start_slot, std::span{samplers});
		}

		static void bindSamplers(ID3D11DeviceContext& device_context,
		                         u32 start_slot,
		                         std::span<ID3D11SamplerState* const> samplers) {

			device_context.CSSetSamplers(start_slot, static_cast<UINT>(samplers.size()), samplers.data());
		}

		static void bindConstantBuffer(ID3D11DeviceContext& device_context,
		                               u32 start_slot,
		                               ID3D11Buffer* buffer) {

			ID3D11Buffer* const buffers[1] = { buffer };
			bindConstantBuffers(device_context, start_slot, std::span{buffers});
		}

		static void bindConstantBuffers(ID3D11DeviceContext& device_context,
		                                u32 start_slot,
		                                std::span<ID3D11Buffer* const> buffers) {

			device_context.CSSetConstantBuffers(start_slot, static_cast<UINT>(buffers.size()), buffers.data());
		}

		static void bindSRV(ID3D11DeviceContext& device_context,
		                    u32 start_slot,
		                    ID3D11ShaderResourceView* srv) {

			ID3D11ShaderResourceView* const srvs[1] = { srv };
			bindSRVs(device_context, start_slot, std::span{srvs});
		}

		static void bindSRVs(ID3D11DeviceContext& device_context,
		                     u32 start_slot,
		                     std::span<ID3D11ShaderResourceView* const> srvs) {

			device_context.CSSetShaderResources(start_slot, static_cast<UINT>(srvs.size()), srvs.data());
		}

		static void bindUAV(ID3D11DeviceContext& device_context,
		                     u32 start_slot,
		                     ID3D11UnorderedAccessView* uav,
		                     const u32* initial_counts = nullptr) {

			ID3D11UnorderedAccessView* const uavs[1] = { uav };
			bindUAVs(device_context, start_slot, std::span{uavs}, initial_counts);
		}

		static void bindUAVs(ID3D11DeviceContext& device_context,
		                     u32 start_slot,
		                     std::span<ID3D11UnorderedAccessView* const> uavs,
		                     const u32* initial_counts = nullptr) {

			device_context.CSSetUnorderedAccessViews(start_slot, static_cast<UINT>(uavs.size()), uavs.data(), initial_counts);
		}
	};


	//----------------------------------------------------------------------------------
	// Domain Stage
	//----------------------------------------------------------------------------------
	struct DS {
		static void bindShader(ID3D11DeviceContext& device_context,
		                       ID3D11DomainShader* shader,
		                       std::span<ID3D11ClassInstance* const> instances) {

			device_context.DSSetShader(shader, instances.data(), static_cast<UINT>(instances.size()));
		}

		static void bindSampler(ID3D11DeviceContext& device_context,
		                        u32 start_slot,
		                        ID3D11SamplerState* sampler) {

			ID3D11SamplerState* const samplers[1] = { sampler };
			bindSamplers(device_context, start_slot, std::span{samplers});
		}

		static void bindSamplers(ID3D11DeviceContext& device_context,
		                         u32 start_slot,
		                         std::span<ID3D11SamplerState* const> samplers) {

			device_context.DSSetSamplers(start_slot, static_cast<UINT>(samplers.size()), samplers.data());
		}

		static void bindConstantBuffer(ID3D11DeviceContext& device_context,
		                               u32 start_slot,
		                               ID3D11Buffer* buffer) {

			ID3D11Buffer* const buffers[1] = { buffer };
			bindConstantBuffers(device_context, start_slot, std::span{buffers});
		}

		static void bindConstantBuffers(ID3D11DeviceContext& device_context,
		                                u32 start_slot,
		                                std::span<ID3D11Buffer* const> buffers) {

			device_context.DSSetConstantBuffers(start_slot, static_cast<UINT>(buffers.size()), buffers.data());
		}

		static void bindSRV(ID3D11DeviceContext& device_context,
		                    u32 start_slot,
		                    ID3D11ShaderResourceView* srv) {

			ID3D11ShaderResourceView* const srvs[1] = { srv };
			bindSRVs(device_context, start_slot, std::span{srvs});
		}

		static void bindSRVs(ID3D11DeviceContext& device_context,
		                     u32 start_slot,
		                     std::span<ID3D11ShaderResourceView* const> srvs) {

			device_context.DSSetShaderResources(start_slot, static_cast<UINT>(srvs.size()), srvs.data());
		}
	};


	//----------------------------------------------------------------------------------
	// Geometry Stage
	//----------------------------------------------------------------------------------
	struct GS {
		static void bindShader(ID3D11DeviceContext& device_context,
		                       ID3D11GeometryShader* shader,
		                       std::span<ID3D11ClassInstance* const> instances) {

			device_context.GSSetShader(shader, instances.data(), static_cast<UINT>(instances.size()));
		}

		static void bindSampler(ID3D11DeviceContext& device_context,
		                        u32 start_slot,
		                        ID3D11SamplerState* sampler) {

			ID3D11SamplerState* const samplers[1] = { sampler };
			bindSamplers(device_context, start_slot, std::span{samplers});
		}

		static void bindSamplers(ID3D11DeviceContext& device_context,
		                         u32 start_slot,
		                         std::span<ID3D11SamplerState* const> samplers) {

			device_context.GSSetSamplers(start_slot, static_cast<UINT>(samplers.size()), samplers.data());
		}

		static void bindConstantBuffer(ID3D11DeviceContext& device_context,
		                               u32 start_slot,
		                               ID3D11Buffer* buffer) {

			ID3D11Buffer* const buffers[1] = { buffer };
			bindConstantBuffers(device_context, start_slot, std::span{buffers});
		}

		static void bindConstantBuffers(ID3D11DeviceContext& device_context,
		                                u32 start_slot,
		                                std::span<ID3D11Buffer* const> buffers) {

			device_context.GSSetConstantBuffers(start_slot, static_cast<UINT>(buffers.size()), buffers.data());
		}

		static void bindSRV(ID3D11DeviceContext& device_context,
		                    u32 start_slot,
		                    ID3D11ShaderResourceView* srv) {

			ID3D11ShaderResourceView* const srvs[1] = { srv };
			bindSRVs(device_context, start_slot, std::span{srvs});
		}

		static void bindSRVs(ID3D11DeviceContext& device_context,
		                     u32 start_slot,
		                     std::span<ID3D11ShaderResourceView* const> srvs) {

			device_context.GSSetShaderResources(start_slot, static_cast<UINT>(srvs.size()), srvs.data());
		}
	};


	//----------------------------------------------------------------------------------
	// Hull Stage
	//----------------------------------------------------------------------------------
	struct HS {
		static void bindShader(ID3D11DeviceContext& device_context,
		                       ID3D11HullShader* shader,
		                       std::span<ID3D11ClassInstance* const> instances) {

			device_context.HSSetShader(shader, instances.data(), static_cast<UINT>(instances.size()));
		}

		static void bindSampler(ID3D11DeviceContext& device_context,
		                        u32 start_slot,
		                        ID3D11SamplerState* sampler) {

			ID3D11SamplerState* const samplers[1] = { sampler };
			bindSamplers(device_context, start_slot, std::span{samplers});
		}

		static void bindSamplers(ID3D11DeviceContext& device_context,
		                         u32 start_slot,
		                         std::span<ID3D11SamplerState* const> samplers) {

			device_context.HSSetSamplers(start_slot, static_cast<UINT>(samplers.size()), samplers.data());
		}

		static void bindConstantBuffer(ID3D11DeviceContext& device_context,
		                               u32 start_slot,
		                               ID3D11Buffer* buffer) {

			ID3D11Buffer* const buffers[1] = { buffer };
			bindConstantBuffers(device_context, start_slot, std::span{buffers});
		}

		static void bindConstantBuffers(ID3D11DeviceContext& device_context,
		                                u32 start_slot,
		                                std::span<ID3D11Buffer* const> buffers) {

			device_context.HSSetConstantBuffers(start_slot, static_cast<UINT>(buffers.size()), buffers.data());
		}

		static void bindSRV(ID3D11DeviceContext& device_context,
		                    u32 start_slot,
		                    ID3D11ShaderResourceView* srv) {

			ID3D11ShaderResourceView* const srvs[1] = { srv };
			bindSRVs(device_context, start_slot, std::span{srvs});
		}

		static void bindSRVs(ID3D11DeviceContext& device_context,
		                     u32 start_slot,
		                     std::span<ID3D11ShaderResourceView* const> srvs) {

			device_context.HSSetShaderResources(start_slot, static_cast<UINT>(srvs.size()), srvs.data());
		}
	};


	//----------------------------------------------------------------------------------
	// Pixel Stage
	//----------------------------------------------------------------------------------
	struct PS {
		static void bindShader(ID3D11DeviceContext& device_context,
		                       ID3D11PixelShader* shader,
		                       std::span<ID3D11ClassInstance* const> instances) {

			device_context.PSSetShader(shader, instances.data(), static_cast<UINT>(instances.size()));
		}

		static void bindSampler(ID3D11DeviceContext& device_context,
		                        u32 start_slot,
		                        ID3D11SamplerState* sampler) {

			ID3D11SamplerState* const samplers[1] = { sampler };
			bindSamplers(device_context, start_slot, std::span{samplers});
		}

		static void bindSamplers(ID3D11DeviceContext& device_context,
		                         u32 start_slot,
		                         std::span<ID3D11SamplerState* const> samplers) {

			device_context.PSSetSamplers(start_slot, static_cast<UINT>(samplers.size()), samplers.data());
		}

		static void bindConstantBuffer(ID3D11DeviceContext& device_context,
		                               u32 start_slot,
		                               ID3D11Buffer* buffer) {

			ID3D11Buffer* const buffers[1] = { buffer };
			bindConstantBuffers(device_context, start_slot, std::span{buffers});
		}

		static void bindConstantBuffers(ID3D11DeviceContext& device_context,
		                                u32 start_slot,
		                                std::span<ID3D11Buffer* const> buffers) {

			device_context.PSSetConstantBuffers(start_slot, static_cast<UINT>(buffers.size()), buffers.data());
		}

		static void bindSRV(ID3D11DeviceContext& device_context,
		                    u32 start_slot,
		                    ID3D11ShaderResourceView* srv) {

			ID3D11ShaderResourceView* const srvs[1] = { srv };
			bindSRVs(device_context, start_slot, std::span{srvs});
		}

		static void bindSRVs(ID3D11DeviceContext& device_context,
		                     u32 start_slot,
		                     std::span<ID3D11ShaderResourceView* const> srvs) {

			device_context.PSSetShaderResources(start_slot, static_cast<UINT>(srvs.size()), srvs.data());
		}
	};


	//----------------------------------------------------------------------------------
	// Rasterizer Stage
	//----------------------------------------------------------------------------------
	struct RS {
		static void bindScissorRects(ID3D11DeviceContext& device_context,
		                             std::span<const D3D11_RECT> rects) {

			device_context.RSSetScissorRects(static_cast<UINT>(rects.size()), rects.data());
		}

		static void bindState(ID3D11DeviceContext& device_context,
		                      ID3D11RasterizerState* state) {

			device_context.RSSetState(state);
		}

		static void bindViewports(ID3D11DeviceContext& device_context,
		                          std::span<const D3D11_VIEWPORT> viewports) {

			device_context.RSSetViewports(static_cast<UINT>(viewports.size()), viewports.data());
		}
	};


	//----------------------------------------------------------------------------------
	// Vertex Stage
	//----------------------------------------------------------------------------------
	struct VS {
		static void bindShader(ID3D11DeviceContext& device_context,
		                       ID3D11VertexShader* shader,
		                       std::span<ID3D11ClassInstance* const> instances) {

			device_context.VSSetShader(shader, instances.data(), static_cast<UINT>(instances.size()));
		}

		static void bindSampler(ID3D11DeviceContext& device_context,
		                        u32 start_slot,
		                        ID3D11SamplerState* sampler) {

			ID3D11SamplerState* const samplers[1] = { sampler };
			bindSamplers(device_context, start_slot, std::span{samplers});
		}

		static void bindSamplers(ID3D11DeviceContext& device_context,
		                         u32 start_slot,
		                         std::span<ID3D11SamplerState* const> samplers) {

			device_context.VSSetSamplers(start_slot, static_cast<UINT>(samplers.size()), samplers.data());
		}

		static void bindConstantBuffer(ID3D11DeviceContext& device_context,
		                               u32 start_slot,
		                               ID3D11Buffer* buffer) {

			ID3D11Buffer* const buffers[1] = { buffer };
			bindConstantBuffers(device_context, start_slot, std::span{buffers});
		}

		static void bindConstantBuffers(ID3D11DeviceContext& device_context,
		                                u32 start_slot,
		                                std::span<ID3D11Buffer* const> buffers) {

			device_context.VSSetConstantBuffers(start_slot, static_cast<UINT>(buffers.size()), buffers.data());
		}

		static void bindSRV(ID3D11DeviceContext& device_context,
		                    u32 start_slot,
		                    ID3D11ShaderResourceView* srv) {

			ID3D11ShaderResourceView* const srvs[1] = { srv };
			bindSRVs(device_context, start_slot, std::span{srvs});
		}

		static void bindSRVs(ID3D11DeviceContext& device_context,
		                     u32 start_slot,
		                     std::span<ID3D11ShaderResourceView* const> srvs) {

			device_context.VSSetShaderResources(start_slot, static_cast<UINT>(srvs.size()), srvs.data());
		}
	};
};

} //namespace render
