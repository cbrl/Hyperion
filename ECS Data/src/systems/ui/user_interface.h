#pragma once

#include "system/system.h"
#include "direct3d/direct3d.h"

class ResourceMgr;
class Scene;

class UserInterface final : public System<UserInterface> {
public:
	UserInterface() = default;
	~UserInterface() = default;

	void update(Engine& engine) override;
};