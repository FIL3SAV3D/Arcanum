#include "MeshRenderSystem.h"

#include "OpenGL/DeferredRenderer.h"

MeshRenderSystem::MeshRenderSystem(std::shared_ptr<Window> _window, const RENDER_TYPE& _type)
{
    switch (_type)
    {
    case MeshRenderSystem::FORWARD:
    {
        assert(false && "has not been added yet");
        break;
    }
    case MeshRenderSystem::DEFFERED:
    {
        renderer = std::make_shared<DeferredRenderer>();
        break;
    }
    case MeshRenderSystem::FORWARDPLUS:
    {
        assert(false && "has not been made yet");
        break;
    }
    case MeshRenderSystem::DEFFEREDPLUS:
    {
        assert(false && "has not been made yet");
        break;
    }
    default:
    {
        assert(false && "invalid render type");
        break;
    }
    }

    window = _window;

    camera = std::make_shared<Camera>();
    camera->SetPosition(glm::vec3(0.0f, 1.0f, 0.0f));
}

MeshRenderSystem::~MeshRenderSystem()
{
}

void MeshRenderSystem::OnUpdate(const float& _deltaTime)
{
    camera->Update(_deltaTime);
    renderer->RenderSceneCB(camera->GetProjectionMatrix(), camera->GetViewMatrix(), camera->GetPosition(), false, glm::vec2(0.0, 0.5));


    //for (Entity const& entity : mEntities)
    //{
    //}
}

void MeshRenderSystem::OnCreate()
{
    renderer->Initialize(glm::vec2(window->screenHeight, window->screenWidth));
}

void MeshRenderSystem::OnDestroy()
{
}