#pragma once

#include "datatypes/datatypes.h"
#include "string/string.h"

#include <variant>


class VarMap final {
private:
	using Var = std::variant<bool,
	                         i8, i16, i32,
	                         u8, u16, u32,
	                         f32, f64,
	                         std::string>;


public:
	VarMap() = default;
	VarMap(const VarMap& map) noexcept = default;
	VarMap(VarMap&& map) noexcept = default;

	~VarMap() = default;

	VarMap& operator=(const VarMap& map) noexcept = default;
	VarMap& operator=(VarMap&& map) noexcept = default;

	template<typename T>
	[[nodiscard]]
	bool contains(const std::string& key) const;

	template<typename T>
	void addVar(const std::string& key, T value);

	template<typename T>
	void setVar(const std::string& key, T value);

	template<typename T>
	[[nodiscard]]
	const T* getVar(const std::string& key) const;


private:
	std::map<std::string, Var> variables;
};


#include "var_map.tpp"