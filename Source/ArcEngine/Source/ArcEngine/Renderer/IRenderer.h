#pragma once

#include "Asset/Types/Model.h"
#include "Asset/Types/Mesh.h"

#include "Jolt/Jolt.h"
#include "Jolt/Math/Float2.h"
#include <OpenGL/Camera.h>

class Window;

struct RenderParams
{
	JPH::Vec3 cameraPosition;
	JPH::Vec3 cameraViewMatrix;
	JPH::Vec3 cameraProjectionMatrix;

	std::shared_ptr<Window> window;
};

class IRenderer
{
public:
	virtual void Create (std::shared_ptr<Window> _Window) = 0;
	virtual void Destroy() = 0;

	// Start Pass Functions
	virtual void BeginRender(const RenderParams& _Params) const = 0;

	// Main Pass Functions
	virtual void RenderModel(const Model& model) = 0;

	virtual void RenderMesh(const Mesh& mesh) = 0;
	virtual void RenderMeshInstanced(const Mesh& mesh) = 0;


	// End Pass Functions
	virtual void EndRender(std::shared_ptr<Window> _Window) const = 0;

	// Helper Functions
	virtual void Blit() const = 0;

	// Callback Functions
	virtual void Resize(std::shared_ptr<Window> _Window) const = 0;
};