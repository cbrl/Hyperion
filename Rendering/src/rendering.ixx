export module rendering;

// rendering
export import :rendering_mgr;
export import :rendering_config;
export import :rendering_options;

// rendering/buffer_types
export import :buffer_types;
export import :constant_buffer;
export import :shadow_map_buffer;
export import :structured_buffer;

// rendering/components
export import :components.camera.camera_base;
export import :components.camera.orthographic_camera;
export import :components.camera.perspective_camera;
export import :components.hierarchy;
export import :components.model;
export import :components.name;
export import :components.text;
export import :components.transform;

export import :components.light.ambient_light;
export import :components.light.directional_light;
export import :components.light.point_light;
export import :components.light.spot_light;

// rendering/direct3d
export import :direct3d;
export import :swapchain;

// rendering/directx
export import :pipeline;
export import :vertex_types;
export import :gpu_profiler;

// rendering/display
export import :viewport;
export import :display_config;

// rendering/engine
export import :engine;

// rendering/entities
export import :entities.core_entities;

// rendering/events
export import :events.core_events;

// rendering/exporter
export import :exporter.assimp_exporter;
export import :exporter.model_exporter;
export import :exporter.texture_exporter;

// rendering/importer
export import :importer.assimp_importer;
export import :importer.model_importer;
export import :importer.texture_importer;

// rendering/renderer
export import :renderer;
export import :output_mgr;
export import :render_state_mgr;
export import :render_states;
export import :pass.bounding_volume_pass;
export import :pass.deferred_pass;
export import :pass.depth_pass;
export import :pass.forward_pass;
export import :pass.light_pass;
export import :pass.sky_pass;
export import :pass.text_pass;

// rendering/resource
export import :resource;
export import :resource_mgr;
export import :font;
export import :mesh;
export import :material;
export import :material_factory;
export import :model_blueprint;
export import :blueprint_factory;
export import :model_config;
export import :model_output;
export import :shader;
export import :shader_bytecode;
export import :shader_factory;
export import :texture;
export import :texture_factory;

// rendering/scene
export import :scene;

// rendering/systems
export import :systems.camera_system;
export import :systems.hierarchy_system;
export import :systems.model_system;
export import :systems.picking_system;
export import :systems.transform_system;
export import :systems.user_interface;
