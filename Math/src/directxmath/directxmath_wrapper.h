#pragma once

#include <DirectXMath.h>
#include <DirectXColors.h>

#include "datatypes/scalar_types.h"
#include "datatypes/vector_types.h"

using namespace DirectX;


//----------------------------------------------------------------------------------
// XMLoad: int32_t
//----------------------------------------------------------------------------------
[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const i32_2* val) noexcept {
	const auto dx_val = XMINT2{(*val)[0], (*val)[1]};
	return XMLoadSInt2(&dx_val);
}

[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const i32_3* val) noexcept {
	const auto dx_val = XMINT3{(*val)[0], (*val)[1], (*val)[2]};
	return XMLoadSInt3(&dx_val);
}

[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const i32_4* val) noexcept {
	const auto dx_val = XMINT4{(*val)[0], (*val)[1], (*val)[2], (*val)[3]};
	return XMLoadSInt4(&dx_val);
}


//----------------------------------------------------------------------------------
// XMLoad: uint32_t
//----------------------------------------------------------------------------------
[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const u32* val) noexcept {
	return XMLoadInt(val);
}

[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const u32_2* val) noexcept {
	const auto dx_val = XMUINT2{(*val)[0], (*val)[1]};
	return XMLoadUInt2(&dx_val);
}

[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const u32_3* val) noexcept {
	const auto dx_val = XMUINT3{(*val)[0], (*val)[1], (*val)[2]};
	return XMLoadUInt3(&dx_val);
}

[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const u32_4* val) noexcept {
	const auto dx_val = XMUINT4{(*val)[0], (*val)[1], (*val)[2], (*val)[3]};
	return XMLoadUInt4(&dx_val);
}


//----------------------------------------------------------------------------------
// XMLoad: float
//----------------------------------------------------------------------------------
[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const f32* val) noexcept {
	return XMLoadFloat(val);
}

[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const f32_2* val) noexcept {
	const auto dx_val = XMFLOAT2{(*val)[0], (*val)[1]};
	return XMLoadFloat2(&dx_val);
}

[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const f32_3* val) noexcept {
	const auto dx_val = XMFLOAT3{(*val)[0], (*val)[1],(*val)[2]};
	return XMLoadFloat3(&dx_val);
}

[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const f32_4* val) noexcept {
	const auto dx_val = XMFLOAT4{(*val)[0], (*val)[1], (*val)[2], (*val)[3]};
	return XMLoadFloat4(&dx_val);
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
inline i32_2 XM_CALLCONV XMStore<i32_2>(FXMVECTOR val) noexcept {
	i32_2 out;
	XMStoreSInt2(reinterpret_cast<XMINT2*>(&out), val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<i32_2>(i32_2* dest, FXMVECTOR val) noexcept {
	XMStoreSInt2(reinterpret_cast<XMINT2*>(dest), val);
}


template<>
[[nodiscard]]
inline i32_3 XM_CALLCONV XMStore<i32_3>(FXMVECTOR val) noexcept {
	i32_3 out;
	XMStoreSInt3(reinterpret_cast<XMINT3*>(&out), val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<i32_3>(i32_3* dest, FXMVECTOR val) noexcept {
	XMStoreSInt3(reinterpret_cast<XMINT3*>(&dest), val);
}


template<>
[[nodiscard]]
inline i32_4 XM_CALLCONV XMStore(FXMVECTOR val) noexcept {
	i32_4 out;
	XMStoreSInt4(reinterpret_cast<XMINT4*>(&out), val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<i32_4>(i32_4* dest, FXMVECTOR val) noexcept {
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
inline u32_2 XM_CALLCONV XMStore<u32_2>(FXMVECTOR val) noexcept {
	u32_2 out;
	XMStoreUInt2(reinterpret_cast<XMUINT2*>(&out), val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<u32_2>(u32_2* dest, FXMVECTOR val) noexcept {
	XMStoreUInt2(reinterpret_cast<XMUINT2*>(dest), val);
}


template<>
[[nodiscard]]
inline u32_3 XM_CALLCONV XMStore<u32_3>(FXMVECTOR val) noexcept {
	u32_3 out;
	XMStoreUInt3(reinterpret_cast<XMUINT3*>(&out), val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<u32_3>(u32_3* dest, FXMVECTOR val) noexcept {
	XMStoreUInt3(reinterpret_cast<XMUINT3*>(dest), val);
}


template<>
[[nodiscard]]
inline u32_4 XM_CALLCONV XMStore<u32_4>(FXMVECTOR val) noexcept {
	u32_4 out;
	XMStoreUInt4(reinterpret_cast<XMUINT4*>(&out), val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<u32_4>(u32_4* dest, FXMVECTOR val) noexcept {
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
inline f32_2 XM_CALLCONV XMStore<f32_2>(FXMVECTOR val) noexcept {
	f32_2 out;
	XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(&out), val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<f32_2>(f32_2* dest, FXMVECTOR val) noexcept {
	XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(dest), val);
}


template<>
[[nodiscard]]
inline f32_3 XM_CALLCONV XMStore<f32_3>(FXMVECTOR val) noexcept {
	f32_3 out;
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&out), val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<f32_3>(f32_3* dest, FXMVECTOR val) noexcept {
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(dest), val);
}


template<>
[[nodiscard]]
inline f32_4 XM_CALLCONV XMStore<f32_4>(FXMVECTOR val) noexcept {
	f32_4 out;
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&out), val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<f32_4>(f32_4* dest, FXMVECTOR val) noexcept {
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(dest), val);
}
