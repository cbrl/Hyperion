
namespace render {

template<typename TemplateT, typename... ArgsT>
ecs::EntityPtr Scene::addEntity(ArgsT&&... args) {
	auto ptr = ecs->createEntity();

	TemplateT t;
	t(*ptr, std::forward<ArgsT>(args)...);

	return ptr;
}


template<typename SystemT, typename... ArgsT>
SystemT& Scene::addSystem(ArgsT&&... args) {
	return ecs->addSystem<SystemT>(std::forward<ArgsT>(args)...);
}


template<typename SystemT>
void Scene::removeSystem() {
	ecs->removeSystem<SystemT>();
}


template<typename EventT, typename... ArgsT>
void Scene::sendEvent(ArgsT&&... args) {
	ecs->sendEvent<EventT>(std::forward<ArgsT>(args)...);
}


template<typename ComponentT>
void Scene::forEach(const std::function<void(ComponentT&)>& act) {
	ecs->forEach<ComponentT>(std::ref(act));
}


template<typename ComponentT>
void Scene::forEach(const std::function<void(const ComponentT&)>& act) const {
	ecs->forEach<ComponentT>(std::ref(act));
}


template<typename... ComponentT>
void Scene::forEach(const std::function<void(ecs::Entity&)>& act) {
	ecs->forEach<ComponentT...>(std::ref(act));
}


template<typename... ComponentT>
void Scene::forEach(const std::function<void(const ecs::Entity&)>& act) const {
	ecs->forEach<ComponentT...>(std::ref(act));
}


template<typename T>
size_t Scene::countOf() const {
	return ecs->countOf<T>();
}

} //namespace render