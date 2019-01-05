#pragma once

struct ID3D11Device;
class ResourceMgr;
class Scene;
class EntityPtr;

namespace NewModelMenu {
void DoMenu();
void ProcNewModelPopup(ID3D11Device& device, ResourceMgr& resource_mgr, EntityPtr entity);
};