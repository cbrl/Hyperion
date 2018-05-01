#pragma once


class ResourceMgr;
class IResource;


//----------------------------------------------------------------------------------
// ResourcePtr
//----------------------------------------------------------------------------------
//
// Wraps the raw pointer to a resource, and a pointer to the
// resource manager that created it. Automatically frees the
// resource and its memory when the ResourcePtr is deleted.
//
//----------------------------------------------------------------------------------

template<typename T>
class ResourcePtr final {
	static_assert(std::is_base_of_v<IResource, T>, "ResourcePtr created with non-resource type.");

	friend class ResourceMgr;

	public:
		ResourcePtr()
			: data(nullptr)
			, mgr(nullptr) {
		}
		ResourcePtr(const ResourcePtr<T>& ptr) {
			data = ptr.data;
			mgr  = ptr.mgr;
		}
		ResourcePtr(ResourcePtr<T>&& ptr) {
			data = std::move(ptr.data);
			mgr  = std::move(ptr.mgr);
		}
		ResourcePtr(ResourceMgr* res_mgr, T* data_ptr)
			: data(data_ptr)
			, mgr(res_mgr) {
		}

		~ResourcePtr() {
			if (mgr) mgr->Destroy(data);
			data = nullptr;
		}

		T* get() const {
			return data;
		}

		void operator=(const ResourcePtr<T>& ptr) {
			data = ptr.data;
			mgr  = ptr.mgr;
		}
		void operator=(ResourcePtr<T>&& ptr) {
			data = std::move(ptr.data);
			mgr  = std::move(ptr.mgr);
		}

		T* operator->() const {
			if (mgr) {
				if (mgr->Expired(data))
					return nullptr;
			}
			return data;
		}

		operator bool() const {
			return (data != nullptr);
		}

	private:
		T* data;
		ResourceMgr* mgr;
};