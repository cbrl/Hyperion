
template<typename TransformT>
void TransformSystem::UpdateWorld(ECS& ecs_engine, TransformT& transform) {

	XMMATRIX this_world   = XMMatrixIdentity();
	XMMATRIX parent_world = XMMatrixIdentity();


	// If the transform has no parent and doesn't need an update,
	// then nothing needs to be done.
	if (transform.GetParent() == handle64::invalid_handle) {
		if (!transform.needs_update) return;
	}

	// If the transform has a parent, then process that first.
	// If the parent was updated, or if this transform already needs
	// an update, then get the parent's matrix.
	else {
		const auto parent     = ecs_engine.getComponent<Transform>(transform.GetParent());
		const auto parent_cam = ecs_engine.getComponent<CameraTransform>(transform.GetParent());

		if (parent) {
			UpdateWorld(ecs_engine, *parent);

			if (parent->updated)
				transform.needs_update = true;

			// This may be true even if the above statement is not
			if (transform.needs_update)
				parent_world = parent->GetObjectToWorldMatrix();
		}
		else if (parent_cam) {
			UpdateWorld(ecs_engine, *parent_cam);

			if (parent_cam->updated)
				transform.needs_update = true;

			if (transform.needs_update)
				parent_world = parent_cam->GetObjectToWorldMatrix();
		}
	}


	if (transform.needs_update) {
		this_world = CalculateWorld(transform);
		transform.world = this_world * parent_world;

		transform.needs_update = false;
		transform.updated = true;
	}
}