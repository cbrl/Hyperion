#pragma once

struct ID3D11Device;
class  ResourceMgr;
class  Scene;
namespace ecs { class EntityPtr; }

class NewModelMenu {
	enum class ModelType {
		None,
		Cube,
		Box,
		Sphere,
		GeoSphere,
		Cylinder,
		Torus,
		Cone,
		Tetrahedron,
		Octahedron,
		Dodecahedron,
		Icosahedron
	};

public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	NewModelMenu() = default;
	NewModelMenu(const NewModelMenu&) = default;
	NewModelMenu(NewModelMenu&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~NewModelMenu() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	NewModelMenu& operator=(const NewModelMenu&) = default;
	NewModelMenu& operator=(NewModelMenu&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void drawMenu();

	void procNewModelPopup(ID3D11Device& device,
	                       ResourceMgr& resource_mgr,
	                       Scene& scene,
	                       ecs::EntityPtr entity);

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	ModelType model_type = ModelType::None;
};