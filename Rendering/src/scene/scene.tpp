template <typename TemplateT, typename... ArgsT>
EntityPtr Scene::addEntity(ArgsT&&... args) {
	auto ptr = ecs->createEntity();
	entities.push_back(ptr);
	TemplateT::applyTemplate(*ptr, std::forward<ArgsT>(args)...);
	return ptr;
}


template <typename EventT, typename... ArgsT>
void Scene::sendEvent(ArgsT&&... args) {
	ecs->sendEvent<EventT>(std::forward<ArgsT>(args)...);
}


template <typename T, typename ActionT>
void Scene::forEach(ActionT&& act) {
	ecs->forEach<T>(std::forward<ActionT>(act));
}


template <typename SystemT, typename... ArgsT>
ISystem* Scene::addSystem(ArgsT&&... args) {
	return ecs->addSystem<SystemT>(std::forward<ArgsT>(args)...);
}


template<typename SystemT>
void Scene::removeSystem() {
	ecs->removeSystem<SystemT>();
}