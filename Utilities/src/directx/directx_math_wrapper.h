#pragma once

#include <DirectXMath.h>
#include "datatypes/scalar_types.h"
#include "datatypes/vector_types.h"

using namespace DirectX;


//----------------------------------------------------------------------------------
// XMLoad: int32_t
//----------------------------------------------------------------------------------
[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const i32_2* val) noexcept {
	return XMLoadSInt2(reinterpret_cast<const XMINT2*>(val));
}

[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const i32_3* val) noexcept {
	return XMLoadSInt3(reinterpret_cast<const XMINT3*>(val));
}

[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const i32_4* val) noexcept {
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
inline XMVECTOR XM_CALLCONV XMLoad(const u32_2* val) noexcept {
	return XMLoadUInt2(reinterpret_cast<const XMUINT2*>(val));
}

[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const u32_3* val) noexcept {
	return XMLoadUInt3(reinterpret_cast<const XMUINT3*>(val));
}

[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const u32_4* val) noexcept {
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
inline XMVECTOR XM_CALLCONV XMLoad(const f32_2* val) noexcept {
	return XMLoadFloat2(reinterpret_cast<const XMFLOAT2*>(val));
}

[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const f32_3* val) noexcept {
	return XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(val));
}

[[nodiscard]]
inline XMVECTOR XM_CALLCONV XMLoad(const f32_4* val) noexcept {
	return XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(val));
}




//----------------------------------------------------------------------------------
// XMStore Template
//----------------------------------------------------------------------------------
template<typename T>
[[nodiscard]]
T XM_CALLCONV XMStore(FXMVECTOR val) noexcept;

template<typename T>
void XM_CALLCONV XMStore(T* store, FXMVECTOR val) noexcept;


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
inline void XM_CALLCONV XMStore<i32_2>(i32_2* store, FXMVECTOR val) noexcept {
	XMStoreSInt2(reinterpret_cast<XMINT2*>(store), val);
}


template<>
[[nodiscard]]
inline i32_3 XM_CALLCONV XMStore<i32_3>(FXMVECTOR val) noexcept {
	i32_3 out;
	XMStoreSInt3(reinterpret_cast<XMINT3*>(&out), val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<i32_3>(i32_3* store, FXMVECTOR val) noexcept {
	XMStoreSInt3(reinterpret_cast<XMINT3*>(&store), val);
}


template<>
[[nodiscard]]
inline i32_4 XM_CALLCONV XMStore(FXMVECTOR val) noexcept {
	i32_4 out;
	XMStoreSInt4(reinterpret_cast<XMINT4*>(&out), val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<i32_4>(i32_4* store, FXMVECTOR val) noexcept {
	XMStoreSInt4(reinterpret_cast<XMINT4*>(&store), val);
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
inline void XM_CALLCONV XMStore<u32>(u32* store, FXMVECTOR val) noexcept {
	XMStoreInt(store, val);
}


template<>
[[nodiscard]]
inline u32_2 XM_CALLCONV XMStore<u32_2>(FXMVECTOR val) noexcept {
	u32_2 out;
	XMStoreUInt2(reinterpret_cast<XMUINT2*>(&out), val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<u32_2>(u32_2* store, FXMVECTOR val) noexcept {
	XMStoreUInt2(reinterpret_cast<XMUINT2*>(store), val);
}


template<>
[[nodiscard]]
inline u32_3 XM_CALLCONV XMStore<u32_3>(FXMVECTOR val) noexcept {
	u32_3 out;
	XMStoreUInt3(reinterpret_cast<XMUINT3*>(&out), val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<u32_3>(u32_3* store, FXMVECTOR val) noexcept {
	XMStoreUInt3(reinterpret_cast<XMUINT3*>(store), val);
}


template<>
[[nodiscard]]
inline u32_4 XM_CALLCONV XMStore<u32_4>(FXMVECTOR val) noexcept {
	u32_4 out;
	XMStoreUInt4(reinterpret_cast<XMUINT4*>(&out), val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<u32_4>(u32_4* store, FXMVECTOR val) noexcept {
	XMStoreUInt4(reinterpret_cast<XMUINT4*>(store), val);
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
inline void XM_CALLCONV XMStore<f32>(f32* store, FXMVECTOR val) noexcept {
	XMStoreFloat(store, val);
}


template<>
[[nodiscard]]
inline f32_2 XM_CALLCONV XMStore<f32_2>(FXMVECTOR val) noexcept {
	f32_2 out;
	XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(&out), val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<f32_2>(f32_2* store, FXMVECTOR val) noexcept {
	XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(store), val);
}


template<>
[[nodiscard]]
inline f32_3 XM_CALLCONV XMStore<f32_3>(FXMVECTOR val) noexcept {
	f32_3 out;
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&out), val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<f32_3>(f32_3* store, FXMVECTOR val) noexcept {
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(store), val);
}


template<>
[[nodiscard]]
inline f32_4 XM_CALLCONV XMStore<f32_4>(FXMVECTOR val) noexcept {
	f32_4 out;
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&out), val);
	return out;
}

template<>
inline void XM_CALLCONV XMStore<f32_4>(f32_4* store, FXMVECTOR val) noexcept {
	XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(store), val);
}