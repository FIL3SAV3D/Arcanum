#pragma once

#include "ArcEngine/Graphics/Interface/IGraphics.h"
#include "ArcEngine/Graphics/OpenGL-4.6/Shader.h"

#include <SDL3/SDL_video.h>

#include "Camera.h"

namespace ArcEngine
{
    class OpenGLGraphics : public IGraphics
    {
    public:
        void Create(const Window& _Window) override;
        void Destroy() override;
        void Resize(const glm::uvec2& _Size) override;

        void UpdateCameraData(const glm::vec4& _Position, const glm::mat4& _View, const glm::mat4& _Projection) override;

        void FrameStart(const Window& _Window) override;
        void FrameEnd  (const Window& _Window) override;

        void Blit() override;

        void RenderMesh(std::shared_ptr<Model> _Model, const glm::mat4x4& _ObjectToWorld) override;
        void RenderMeshInstanced() override;
        void RenderMeshIndirect() override;

        void CreateMesh(MeshCreationInfo& _Info) override;
        void DestroyMesh() override;

        void CreateTexture() override;
        void DestroyTexture() override;

    private:
        SDL_GLContext glContext;

    private:
        unsigned int quadVAO;
        unsigned int quadVBO;

        unsigned int cameraUBO;

        std::vector<MeshInfo> meshArray;

        std::unique_ptr<Shader> blinn;
    };
}