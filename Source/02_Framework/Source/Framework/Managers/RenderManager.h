#pragma once

#include <glm/glm.hpp>
#include <memory>

struct Model;
class RenderManager
{
public:
    void RenderMesh(std::shared_ptr<Model> _Model, const glm::mat4x4& _ObjectToWorld);
private:

};