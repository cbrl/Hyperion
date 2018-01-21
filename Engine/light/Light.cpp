#include "stdafx.h"
#include "Light.h"


Light::Light() {
}


Light::~Light() {
}


void Light::SetDirection(XMFLOAT3 direction) {
	m_Buffer.lightDirection = direction;
}


void Light::SetAmbientColor(XMFLOAT4 color) {
	m_Buffer.ambientColor = color;
}


void Light::SetDiffuseColor(XMFLOAT4 color) {
	m_Buffer.diffuseColor = color;
}


void Light::SetSpecularColor(XMFLOAT4 color) {
	m_Buffer.specularColor = color;
}


void Light::SetSpecularPower(float power) {
	m_Buffer.specularPower = power;
}


LightBuffer Light::GetBuffer() {
	return m_Buffer;
}


XMFLOAT3 Light::GetDirection() {
	return m_Buffer.lightDirection;
}


XMFLOAT4 Light::GetAmbientColor() {
	return m_Buffer.ambientColor;
}


XMFLOAT4 Light::GetDiffuseColor() {
	return m_Buffer.diffuseColor;
}


XMFLOAT4 Light::GetSpecularColor() {
	return m_Buffer.specularColor;
}


float Light::GetSpecularPower() {
	return m_Buffer.specularPower;
}
