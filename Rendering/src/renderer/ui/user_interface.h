#pragma once

class Engine;

class UserInterface final {
public:
	UserInterface() = default;
	~UserInterface() = default;

	static void draw(const Engine& engine);
};