#pragma once

#include "util\datatypes\datatypes.h"


class IResource {
	friend class ResourceMgr;

	public:
		IResource()
			: guid(L"")
		{}

		IResource(const wstring& guid)
			: guid(guid)
		{}

		IResource(wstring&& guid)
			: guid(guid)
		{}

		virtual ~IResource() = default;

		const wstring& GetGUID() const {
			return guid;
		}

		virtual const type_index GetTypeID() const = 0;


	protected:
		wstring guid;
};


template<typename ResourceT>
class Resource : public IResource {
	public:
		Resource() = default;
		virtual ~Resource() = default;

		const type_index GetTypeID() const {
			return type_id;
		}


	public:
		static const type_index type_id;
};


template<typename ResourceT>
const type_index Resource<ResourceT>::type_id = type_index(typeid(ResourceT));