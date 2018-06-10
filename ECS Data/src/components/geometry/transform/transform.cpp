#include "transform.h"
#include "ecs.h"


Transform::Transform()
	: parent(handle64::invalid_handle)
	, world(XMMatrixIdentity())
	, translation(XMVectorZero())
	, rotation(XMVectorZero())
	, scaling(XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f))
	, needs_update(false)
	, updated(false) {
}


void Transform::move(const float3& move) {
	translation += XMLoadFloat3(&move);
	needs_update = true;
}


void Transform::setPosition(const float3& position) {
	translation = XMLoadFloat3(&position);
	needs_update = true;
}


void XM_CALLCONV Transform::move(FXMVECTOR move) {
	translation += move;
	needs_update = true;
}


void XM_CALLCONV Transform::setPosition(FXMVECTOR position) {
	translation = position;
	needs_update = true;
}


void Transform::rotate(const float3& rotate) {
	rotation += XMLoadFloat3(&rotate);
	needs_update = true;
}


void Transform::setRotation(const float3& rotation) {
	this->rotation = XMLoadFloat3(&rotation);
	needs_update = true;
}


void XM_CALLCONV Transform::rotate(FXMVECTOR rotate) {
	rotation += rotate;
	needs_update = true;
}


void XM_CALLCONV Transform::setRotation(FXMVECTOR rotation) {
	this->rotation = rotation;
	needs_update = true;
}


void Transform::scale(const float3& scale) {
	this->scaling *= XMLoadFloat3(&scale);
	needs_update = true;
}


void Transform::setScale(const float3& scale) {
	this->scaling = XMLoadFloat3(&scale);
	needs_update = true;
}


void XM_CALLCONV Transform::scale(FXMVECTOR scale) {
	this->scaling *= scale;
	needs_update = true;
}


void XM_CALLCONV Transform::setScale(FXMVECTOR scale) {
	this->scaling = scale;
	needs_update = true;
}
