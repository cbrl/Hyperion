#pragma once

#include "imgui.h"
#include "string/string.h"
#include "entity/entity_ptr.h"

template <typename T>
struct SelectableTree final {
public:
	template <typename... ArgsT>
	bool newNode(T* ptr, gsl::czstring<> fmt, ArgsT... args) {
		const auto flags = ImGuiTreeNodeFlags_OpenOnArrow
		                   | ImGuiTreeNodeFlags_OpenOnDoubleClick
		                   | (selected == ptr ? ImGuiTreeNodeFlags_Selected : 0);
		return node(ptr, flags, fmt, std::forward<ArgsT>(args)...);
	}

	template <typename... ArgsT>
	bool newLeafNode(T* ptr, gsl::czstring<> fmt, ArgsT... args) {
		const auto flags = ImGuiTreeNodeFlags_Leaf
		                   | ImGuiTreeNodeFlags_Bullet
		                   | (selected == ptr ? ImGuiTreeNodeFlags_Selected : 0);
		return node(ptr, flags, fmt, std::forward<ArgsT>(args)...);
	}

	void endNode() {
		ImGui::TreePop();
	}

	T* getSelected() const noexcept {
		return selected;
	}

	void setSelected(T* ptr) noexcept {
		selected = ptr;
	}

	bool isSelected(T* ptr) const noexcept {
		return selected == ptr;
	}

private:

	template <typename... ArgsT>
	bool node(T* ptr, ImGuiTreeNodeFlags flags, gsl::czstring<> fmt, ArgsT... args) {
		const bool is_open = ImGui::TreeNodeEx(ptr, flags, fmt, std::forward<ArgsT>(args)...);
		if (ImGui::IsItemClicked()) {
			selected = ptr;
		}
		return is_open;
	}


	T* selected = nullptr;
};


template <>
struct SelectableTree<ecs::EntityPtr> final {
public:
	template <typename... ArgsT>
	bool newNode(const ecs::EntityPtr& ptr, gsl::czstring<> fmt, ArgsT... args) {
		const auto flags = ImGuiTreeNodeFlags_OpenOnArrow
		                   | ImGuiTreeNodeFlags_OpenOnDoubleClick
		                   | (selected == ptr ? ImGuiTreeNodeFlags_Selected : 0);
		return node(ptr, flags, fmt, std::forward<ArgsT>(args)...);
	}

	template <typename... ArgsT>
	bool newLeafNode(const ecs::EntityPtr& ptr, gsl::czstring<> fmt, ArgsT... args) {
		const auto flags = ImGuiTreeNodeFlags_Leaf
		                   | ImGuiTreeNodeFlags_Bullet
		                   | (selected == ptr ? ImGuiTreeNodeFlags_Selected : 0);
		return node(ptr, flags, fmt, std::forward<ArgsT>(args)...);
	}

	void endNode() {
		ImGui::TreePop();
	}

	ecs::EntityPtr getSelected() const noexcept {
		return selected;
	}

	void setSelected(const ecs::EntityPtr& ptr) noexcept {
		selected = ptr;
	}

	bool isSelected(const ecs::EntityPtr& ptr) const noexcept {
		return selected == ptr;
	}

private:

	template <typename... ArgsT>
	bool node(const ecs::EntityPtr& ptr, ImGuiTreeNodeFlags flags, gsl::czstring<> fmt, ArgsT... args) {
		const bool is_open = ImGui::TreeNodeEx(ptr.get(), flags, fmt, std::forward<ArgsT>(args)...);
		if (ImGui::IsItemClicked()) {
			selected = ptr;
		}
		return is_open;
	}


	ecs::EntityPtr selected = {};
};