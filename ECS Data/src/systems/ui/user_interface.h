#pragma once

#include "system/system.h"
#include "direct3d/direct3d.h"

class ResourceMgr;
class Scene;

class UserInterface final : public System<UserInterface> {
public:
	UserInterface(ID3D11Device& device, ResourceMgr& resource_mgr);
	~UserInterface() = default;

	void update(Engine& engine) override;


private:
	ID3D11Device& device;
	ResourceMgr&  resource_mgr;
};