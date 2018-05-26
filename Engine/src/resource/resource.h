#pragma once

#include "datatypes\datatypes.h"


template<typename ResourceT>
class Resource {
	public:
		Resource() = default;

		Resource(const wstring& guid)
			: guid(guid) {
		}

		Resource(wstring&& guid)
			: guid(guid) {
		}

		virtual ~Resource() = default;

		const type_index GetTypeID() const {
			return type_id;
		}


	public:
		wstring guid;
		static const type_index type_id;
};


template<typename ResourceT>
const type_index Resource<ResourceT>::type_id = type_index(typeid(ResourceT));