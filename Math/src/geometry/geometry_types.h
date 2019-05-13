#pragma once

#include "datatypes/datatypes.h"


struct Point3 : public f32_3 {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	using f32_3::f32_3;

	constexpr Point3() noexcept = default;
	constexpr Point3(const Point3&) noexcept = default;
	constexpr Point3(Point3&&) noexcept = default;

	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~Point3() = default;

	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	constexpr Point3& operator=(const Point3&) noexcept = default;
	constexpr Point3& operator=(Point3&&) noexcept = default;

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	constexpr f32 x() const noexcept {
		return this->operator[](0);
	}

	constexpr void x(f32 val) noexcept {
		this->operator[](0) = val;
	}

	[[nodiscard]]
	constexpr f32 y() const noexcept {
		return this->operator[](1);
	}

	constexpr void y(f32 val) noexcept {
		this->operator[](1) = val;
	}

	[[nodiscard]]
	constexpr f32 z() const noexcept {
		return this->operator[](2);
	}

	constexpr void z(f32 val) noexcept {
		this->operator[](2) = val;
	}
};


struct Direction3 : public f32_3 {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	using f32_3::f32_3;

	constexpr Direction3() noexcept = default;
	constexpr Direction3(const Direction3&) noexcept = default;
	constexpr Direction3(Direction3&&) noexcept = default;

	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~Direction3() = default;

	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	constexpr Direction3& operator=(const Direction3&) noexcept = default;
	constexpr Direction3& operator=(Direction3&&) noexcept = default;

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	constexpr f32 x() const noexcept {
		return this->operator[](0);
	}
	constexpr void x(f32 val) noexcept {
		this->operator[](0) = val;
	}

	[[nodiscard]]
	constexpr f32 y() const noexcept {
		return this->operator[](1);
	}
	constexpr void y(f32 val) noexcept {
		this->operator[](1) = val;
	}

	[[nodiscard]]
	constexpr f32 z() const noexcept {
		return this->operator[](2);
	}
	constexpr void z(f32 val) noexcept {
		this->operator[](2) = val;
	}
};


struct Ray3 {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	constexpr Ray3() noexcept = default;

	constexpr Ray3(const Point3& origin, const Direction3& direction)
		: origin(origin)
		, direction(direction) {
	}

	constexpr Ray3(const Ray3&) noexcept = default;
	constexpr Ray3(Ray3&&) noexcept = default;

	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~Ray3() = default;

	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	constexpr Ray3& operator=(const Ray3&) noexcept = default;
	constexpr Ray3& operator=(Ray3&&) noexcept = default;

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	constexpr const Point3& getOrigin() const noexcept {
		return origin;
	}
	constexpr void setOrigin(const Point3& point) noexcept {
		origin = point;
	}

	[[nodiscard]]
	constexpr const Direction3& getDirection() const noexcept {
		return direction;
	}
	constexpr void setDirections(const Direction3& dir) noexcept {
		direction = dir;
	}

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	Point3     origin;
	Direction3 direction;
};