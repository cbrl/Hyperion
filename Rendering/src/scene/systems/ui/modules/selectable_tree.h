#pragma once

#include "imgui.h"
#include "string/string.h"

template<typename T>
struct SelectableTree final {
	static_assert(std::is_integral_v<T> || std::is_pointer_v<T>,
		"SelectableTree only supports integral and pointer types");

public:
	template<typename... ArgsT>
	bool newNode(T ptr, gsl::czstring<> fmt, ArgsT... args) {
		const auto flags = ImGuiTreeNodeFlags_OpenOnArrow
		                   | ImGuiTreeNodeFlags_OpenOnDoubleClick
		                   | (selected == ptr ? ImGuiTreeNodeFlags_Selected : 0);
		return node(ptr, flags, fmt, std::forward<ArgsT>(args)...);
	}

	template<typename... ArgsT>
	bool newLeafNode(T ptr, gsl::czstring<> fmt, ArgsT... args) {
		const auto flags = ImGuiTreeNodeFlags_Leaf
		                   | ImGuiTreeNodeFlags_Bullet
		                   | (selected == ptr ? ImGuiTreeNodeFlags_Selected : 0);
		return node(ptr, flags, fmt, std::forward<ArgsT>(args)...);
	}

	void endNode() {
		ImGui::TreePop();
	}

	T getSelected() const noexcept {
		return selected;
	}

	void setSelected(T ptr) noexcept {
		selected = ptr;
	}

	bool isSelected(T ptr) const noexcept {
		return selected == ptr;
	}

private:

	template<typename... ArgsT>
	bool node(T ptr, ImGuiTreeNodeFlags flags, gsl::czstring<> fmt, ArgsT... args) {
		const bool is_open = ImGui::TreeNodeEx(reinterpret_cast<void*>(ptr), flags, fmt, std::forward<ArgsT>(args)...);
		if (ImGui::IsItemClicked()) {
			selected = ptr;
		}
		return is_open;
	}


	T selected = T{};
};