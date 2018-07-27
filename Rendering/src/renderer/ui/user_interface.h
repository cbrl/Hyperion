#pragma once

#include "datatypes/datatypes.h"
#include "direct3d/direct3d.h"

class ResourceMgr;
class Scene;

class UserInterface final {
public:
	UserInterface(ID3D11Device& device, ResourceMgr& resource_mgr)
		: device(device)
		, resource_mgr(resource_mgr) {
	}

	~UserInterface() = default;

	void draw(Scene& scene) const;


private:
	std::reference_wrapper<ID3D11Device> device;
	std::reference_wrapper<ResourceMgr> resource_mgr;
};