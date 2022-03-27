#pragma once

// This is a hacky way to get around forward declarations in modules not working (and giving
// unhelpful error messages like "cannot convert MyClass* to MyClass*")
namespace render {

class Engine;

class DisplayConfig;
class ResourceMgr;
class RenderingMgr;
class RenderingConfig;
class Scene;
class SwapChain;

class LightPass;
class ForwardPass;
class DeferredPass;
class SkyPass;
class BoundingVolumePass;
class TextPass;

} //namespace render
