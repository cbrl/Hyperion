#pragma once

// This is a hacky way to get around forward declarations in modules not working (and giving
// unhelpful error messages like "cannot convert MyClass* to MyClass*")

namespace render {

class Engine;

class Direct3D;
class DisplayConfig;
class GPUProfiler;
class ResourceMgr;
class Renderer;
class RenderingMgr;
class RenderingConfig;
class RenderStateMgr;
class Scene;
class SwapChain;

class LightPass;
class ForwardPass;
class DeferredPass;
class DepthPass;
class SkyPass;
class BoundingVolumePass;
class TextPass;

class Font;
class Mesh;
class ModelBlueprint;
class ModelConfig;
class ModelOutput;
class Texture;

} //namespace render

class PerspectiveCamera;
class OrthographicCamera;
class DirectionalLight;
class PointLight;
class SpotLight;
class Material;
class Model;
class Transform;
