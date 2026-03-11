#include "OpenGLGraphics.h"

#include "GLAD/glad.h"
#include "SDL3/SDL.h"

#include "fmt/core.h"
#include <stb_image.h>

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

void ArcEngine::OpenGLGraphics::CreateMesh(MeshCreationInfo& _Info)
{
    MeshInfo info{};

    glGenVertexArrays(1, &info.VAO);
    glGenBuffers(1, &info.VBO);
    glGenBuffers(1, &info.EBO);

    glBindVertexArray(info.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, info.VBO);
    glBufferData(GL_ARRAY_BUFFER, _Info.vertices.size() * sizeof(VertexData), &_Info.vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, info.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _Info.indices.size() * sizeof(unsigned int), &_Info.indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);

    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, aNormal));

    //glEnableVertexAttribArray(2);
    //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, aTangent));

    //glEnableVertexAttribArray(3);
    //glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, aBiTangent));

    //glEnableVertexAttribArray(4);
    //glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, aTexCoords));

    glBindVertexArray(0);

    meshArray.push_back(info);
}

void ArcEngine::OpenGLGraphics::DestroyMesh()
{
    for (auto& mesh : meshArray)
    {
        glDeleteBuffers(1, &mesh.EBO);
        glDeleteBuffers(1, &mesh.VBO);
        glDeleteVertexArrays(1, &mesh.VAO);
    }
}

void ArcEngine::OpenGLGraphics::CreateTexture()
{
    //std::string filename = std::string(path);
    //filename = directory + '\\' + filename;

    //unsigned int textureID;
    //glGenTextures(1, &textureID);

    //stbi_set_flip_vertically_on_load(false);
    //int width, height, nrComponents;
    //unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    //if (data)
    //{
    //    GLenum format;
    //    if (nrComponents == 1)
    //        format = GL_RED;
    //    else if (nrComponents == 3)
    //        format = GL_RGB;
    //    else if (nrComponents == 4)
    //        format = GL_RGBA;

    //    glBindTexture(GL_TEXTURE_2D, textureID);
    //    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    //    glGenerateMipmap(GL_TEXTURE_2D);

    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //    stbi_image_free(data);
    //}
    //else
    //{
    //    std::cout << "Texture failed to load at path: " << path << std::endl;
    //    stbi_image_free(data);
    //}

    //return textureID;
}

void ArcEngine::OpenGLGraphics::DestroyTexture()
{
}
