template<typename T>
ResourcePool<T>::~ResourcePool() {
	for (auto* obj : objects) {
		obj->~T();
		pool.deallocate(obj, sizeof(T), alignof(T));
	}
	objects.clear();
}

template<typename T>
void* ResourcePool<T>::allocate() {

	void* object = pool.allocate(sizeof(T), alignof(T));
	objects.push_back(static_cast<T*>(object));
	return object;
}


template<typename T>
template<typename... ArgsT>
T* ResourcePool<T>::construct(ArgsT&&... args) {

	T* memory = static_cast<T*>(allocate());
	new(memory) T(std::forward<ArgsT>(args)...);

	return memory;
}


template<typename T>
void ResourcePool<T>::deallocate(T* object) {

	object->~T();
	objects.remove(object);
	pool.deallocate(object, sizeof(T), alignof(T));
}