#pragma once

#include "entity/entity.h"
#include "scene/components/transform/transform.h"


namespace EntityTemplates {

//----------------------------------------------------------------------------------
// WorldObjecct
//----------------------------------------------------------------------------------
//
// The basic template used for any nonspecific entity with a position in the scene.
//
//----------------------------------------------------------------------------------
struct WorldObject final {
	static void applyTemplate(Entity& entity) {
		entity.addComponent<Transform>();
	}
};


} // namespace EntityTemplates