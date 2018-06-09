#include "stdafx.h"
#include "transform.h"
#include "ecs.h"


Transform::Transform()
	: parent(handle64::invalid_handle)
	, world(XMMatrixIdentity())
	, translation(XMVectorZero())
	, rotation(XMVectorZero())
	, scale(XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f))
	, needs_update(false)
	, updated(false) {
}


void Transform::Move(const float3& move) {
	translation += XMLoadFloat3(&move);
	needs_update = true;
}


void Transform::SetPosition(const float3& position) {
	translation = XMLoadFloat3(&position);
	needs_update = true;
}


void XM_CALLCONV Transform::Move(FXMVECTOR move) {
	translation += move;
	needs_update = true;
}


void XM_CALLCONV Transform::SetPosition(FXMVECTOR position) {
	translation = position;
	needs_update = true;
}


void Transform::Rotate(const float3& rotate) {
	rotation += XMLoadFloat3(&rotate);
	needs_update = true;
}


void Transform::SetRotation(const float3& rotation) {
	this->rotation = XMLoadFloat3(&rotation);
	needs_update = true;
}


void XM_CALLCONV Transform::Rotate(FXMVECTOR rotate) {
	rotation += rotate;
	needs_update = true;
}


void XM_CALLCONV Transform::SetRotation(FXMVECTOR rotation) {
	this->rotation = rotation;
	needs_update = true;
}


void Transform::Scale(const float3& scale) {
	this->scale *= XMLoadFloat3(&scale);
	needs_update = true;
}


void Transform::SetScale(const float3& scale) {
	this->scale = XMLoadFloat3(&scale);
	needs_update = true;
}


void XM_CALLCONV Transform::Scale(FXMVECTOR scale) {
	this->scale *= scale;
	needs_update = true;
}


void XM_CALLCONV Transform::SetScale(FXMVECTOR scale) {
	this->scale = scale;
	needs_update = true;
}
