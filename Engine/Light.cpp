#include "stdafx.h"
#include "Light.h"


Light::Light() {
}


Light::~Light() {
}


void Light::SetDirection(XMFLOAT3 direction) {
	m_Direction = direction;
}


void Light::SetAmbientColor(XMFLOAT4 color) {
	m_AmbientColor = color;
}


void Light::SetDiffuseColor(XMFLOAT4 color) {
	m_DiffuseColor = color;
}


void Light::SetSpecularColor(XMFLOAT4 color) {
	m_SpecularColor = color;
}


void Light::SetSpecularPower(float power) {
	m_SpecularPower = power;
}


XMFLOAT3 Light::GetDirection() {
	return m_Direction;
}


XMFLOAT4 Light::GetAmbientColor() {
	return m_AmbientColor;
}


XMFLOAT4 Light::GetDiffuseColor() {
	return m_DiffuseColor;
}


XMFLOAT4 Light::GetSpecularColor() {
	return m_SpecularColor;
}


float Light::GetSpecularPower() {
	return m_SpecularPower;
}
