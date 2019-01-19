#pragma once

#include <DirectXMath.h>
#include "datatypes/scalar_types.h"
#include "datatypes/vector_types.h"

using namespace DirectX;


//----------------------------------------------------------------------------------
// XMLoad: int32_t
//----------------------------------------------------------------------------------
[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const vec2_i32* val) noexcept {
	return XMLoadSInt2(reinterpret_cast<const XMINT2*>(val));
}

[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const vec3_i32* val) noexcept {
	return XMLoadSInt3(reinterpret_cast<const XMINT3*>(val));
}

[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const vec4_i32* val) noexcept {
	return XMLoadSInt4(reinterpret_cast<const XMINT4*>(val));
}


//----------------------------------------------------------------------------------
// XMLoad: uint32_t
//----------------------------------------------------------------------------------
[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const u32* val) noexcept {
	return XMLoadInt(val);
}

[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const vec2_u32* val) noexcept {
	return XMLoadUInt2(reinterpret_cast<const XMUINT2*>(val));
}

[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const vec3_u32* val) noexcept {
	return XMLoadUInt3(reinterpret_cast<const XMUINT3*>(val));
}

[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const vec4_u32* val) noexcept {
	return XMLoadUInt4(reinterpret_cast<const XMUINT4*>(val));
}


//----------------------------------------------------------------------------------
// XMLoad: float
//----------------------------------------------------------------------------------
[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const f32* val) noexcept {
	return XMLoadFloat(val);
}

[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const vec2_f32* val) noexcept {
	return XMLoadFloat2(reinterpret_cast<const XMFLOAT2*>(val));
}

[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const vec3_f32* val) noexcept {
	return XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(val));
}

[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const vec4_f32* val) noexcept {
	return XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(val));
}




//----------------------------------------------------------------------------------
// XMStore Template
//----------------------------------------------------------------------------------
template<typename T>
[[nodiscard]]
T XM_CALLCONV XMStore(FXMVECTOR val) noexcept;

template<typename T>
void XM_CALLCONV XMStore(T* dest, FXMVECTOR val) noexcept;


//----------------------------------------------------------------------------------
// XMStore: int32_t
//----------------------------------------------------------------------------------
template<>
[[nodiscard]]
inline vec2_i32 XM_CALLCONV XMStore<vec2_i32>(FXMVECTOR val) noexcept {
	vec2_i32 out;
	XMStoreSInt2(reinterpret_cast<XMINT2*>(&out), val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<vec2_i32>(vec2_i32* dest, FXMVECTOR val) noexcept {
	XMStoreSInt2(reinterpret_cast<XMINT2*>(dest), val);
}


template<>
[[nodiscard]]
inline vec3_i32 XM_CALLCONV XMStore<vec3_i32>(FXMVECTOR val) noexcept {
	vec3_i32 out;
	XMStoreSInt3(reinterpret_cast<XMINT3*>(&out), val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<vec3_i32>(vec3_i32* dest, FXMVECTOR val) noexcept {
	XMStoreSInt3(reinterpret_cast<XMINT3*>(&dest), val);
}


template<>
[[nodiscard]]
inline vec4_i32 XM_CALLCONV XMStore(FXMVECTOR val) noexcept {
	vec4_i32 out;
	XMStoreSInt4(reinterpret_cast<XMINT4*>(&out), val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<vec4_i32>(vec4_i32* dest, FXMVECTOR val) noexcept {
	XMStoreSInt4(reinterpret_cast<XMINT4*>(&dest), val);
}



//----------------------------------------------------------------------------------
// XMStore: uint32_t
//----------------------------------------------------------------------------------
template<>
[[nodiscard]]
inline u32 XM_CALLCONV XMStore<u32>(FXMVECTOR val) noexcept {
	u32 out;
	XMStoreInt(&out, val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<u32>(u32* dest, FXMVECTOR val) noexcept {
	XMStoreInt(dest, val);
}


template<>
[[nodiscard]]
inline vec2_u32 XM_CALLCONV XMStore<vec2_u32>(FXMVECTOR val) noexcept {
	vec2_u32 out;
	XMStoreUInt2(reinterpret_cast<XMUINT2*>(&out), val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<vec2_u32>(vec2_u32* dest, FXMVECTOR val) noexcept {
	XMStoreUInt2(reinterpret_cast<XMUINT2*>(dest), val);
}


template<>
[[nodiscard]]
inline vec3_u32 XM_CALLCONV XMStore<vec3_u32>(FXMVECTOR val) noexcept {
	vec3_u32 out;
	XMStoreUInt3(reinterpret_cast<XMUINT3*>(&out), val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<vec3_u32>(vec3_u32* dest, FXMVECTOR val) noexcept {
	XMStoreUInt3(reinterpret_cast<XMUINT3*>(dest), val);
}


template<>
[[nodiscard]]
inline vec4_u32 XM_CALLCONV XMStore<vec4_u32>(FXMVECTOR val) noexcept {
	vec4_u32 out;
	XMStoreUInt4(reinterpret_cast<XMUINT4*>(&out), val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<vec4_u32>(vec4_u32* dest, FXMVECTOR val) noexcept {
	XMStoreUInt4(reinterpret_cast<XMUINT4*>(dest), val);
}



//----------------------------------------------------------------------------------
// XMStore: float
//----------------------------------------------------------------------------------
template<>
[[nodiscard]]
inline f32 XM_CALLCONV XMStore<f32>(FXMVECTOR val) noexcept {
	f32 out;
	XMStoreFloat(&out, val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<f32>(f32* dest, FXMVECTOR val) noexcept {
	XMStoreFloat(dest, val);
}


template<>
[[nodiscard]]
inline vec2_f32 XM_CALLCONV XMStore<vec2_f32>(FXMVECTOR val) noexcept {
	vec2_f32 out;
	XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(&out), val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<vec2_f32>(vec2_f32* dest, FXMVECTOR val) noexcept {
	XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(dest), val);
}


template<>
[[nodiscard]]
inline vec3_f32 XM_CALLCONV XMStore<vec3_f32>(FXMVECTOR val) noexcept {
	vec3_f32 out;
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&out), val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<vec3_f32>(vec3_f32* dest, FXMVECTOR val) noexcept {
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(dest), val);
}


template<>
[[nodiscard]]
inline vec4_f32 XM_CALLCONV XMStore<vec4_f32>(FXMVECTOR val) noexcept {
	vec4_f32 out;
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&out), val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<vec4_f32>(vec4_f32* dest, FXMVECTOR val) noexcept {
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(dest), val);
}
