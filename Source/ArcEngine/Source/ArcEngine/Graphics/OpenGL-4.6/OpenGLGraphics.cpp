#include "OpenGLGraphics.h"

#include "GLAD/glad.h"
#include "SDL3/SDL.h"

#include "fmt/core.h"

void ArcEngine::OpenGLGraphics::Create(std::shared_ptr<Window> _window)
{
    window = _window;

    const auto& screenSize = _window->GetScreenSize();

    glContext = SDL_GL_CreateContext(_window->GetNativeWindow());

    if (!SDL_GL_MakeCurrent(_window->GetNativeWindow(), glContext))
    {
        fmt::println("{}", SDL_GetError());
    }

    // Check OpenGL properties
    printf("OpenGL loaded\n");
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));

    glViewport(0, 0, screenSize.x, screenSize.y);
}

void ArcEngine::OpenGLGraphics::Destroy()
{
    SDL_GL_DestroyContext(glContext);
}

void ArcEngine::OpenGLGraphics::Resize(const glm::uvec2& _Size)
{
    glViewport(0, 0, _Size.x, _Size.y);
}

void ArcEngine::OpenGLGraphics::Blit()
{

}

void ArcEngine::OpenGLGraphics::RenderMesh()
{
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);



    SDL_GL_SwapWindow(window->GetNativeWindow());
}

void ArcEngine::OpenGLGraphics::RenderMeshInstanced()
{
}

void ArcEngine::OpenGLGraphics::RenderMeshIndirect()
{
}
