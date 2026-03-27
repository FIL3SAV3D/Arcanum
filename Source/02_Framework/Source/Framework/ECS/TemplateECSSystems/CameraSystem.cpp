#include "CameraSystem.h"

#include "Framework/ECS/Coordinator.h"
#include "Framework/ECS/Components/TransformComponent.h"
#include "Framework/ECS/Components/CameraComponent.h"
#include <Framework/ECS/Components/InputComponent.h>

#include <SDL3/SDL_events.h>

#include <ArcEngine/ArcEngine.h>
#include <ArcEngine/Graphics/CameraData.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>

void CameraSystem::GetSignature(SignatureParameters& _Parameters)
{
    Signature& signature = _Parameters.signature;
    ComponentManager& componentManager = _Parameters.componentManager;

    signature.set(componentManager.GetComponentType<TransformComponent>());
    signature.set(componentManager.GetComponentType<CameraComponent   >());
    signature.set(componentManager.GetComponentType<InputComponent    >());
}

void CameraSystem::OnInput(InputState& _InputState)
{
    const Entity& cameraEntity = *mEntities.begin();

    auto& inputComponent = _InputState.coordinator->GetComponent<InputComponent>(cameraEntity);

    if (_InputState.event->key.repeat != 0)
    {
        return;
    }

    const auto& scancode = _InputState.event->key.scancode;

    switch (_InputState.event->type)
    {
    case (SDL_EVENT_KEY_DOWN):
    {
        if (scancode == SDL_SCANCODE_W) { inputComponent.movementInput.z += 1.0f; }
        if (scancode == SDL_SCANCODE_S) { inputComponent.movementInput.z -= 1.0f; }
        if (scancode == SDL_SCANCODE_D) { inputComponent.movementInput.x += 1.0f; }
        if (scancode == SDL_SCANCODE_A) { inputComponent.movementInput.x -= 1.0f; }

        if (scancode == SDL_SCANCODE_Q) { inputComponent.movementInput.y -= 1.0f; }
        if (scancode == SDL_SCANCODE_E) { inputComponent.movementInput.y += 1.0f; }

        if (scancode == SDL_SCANCODE_LSHIFT) { inputComponent.isPressingShift = true; }

        break;
    }
    case (SDL_EVENT_KEY_UP):
    {
        if (scancode == SDL_SCANCODE_W) { inputComponent.movementInput.z -= 1.0f; }
        if (scancode == SDL_SCANCODE_S) { inputComponent.movementInput.z += 1.0f; }
        if (scancode == SDL_SCANCODE_D) { inputComponent.movementInput.x -= 1.0f; }
        if (scancode == SDL_SCANCODE_A) { inputComponent.movementInput.x += 1.0f; }

        if (scancode == SDL_SCANCODE_Q) { inputComponent.movementInput.y += 1.0f; }
        if (scancode == SDL_SCANCODE_E) { inputComponent.movementInput.y -= 1.0f; }

        if (scancode == SDL_SCANCODE_LSHIFT) { inputComponent.isPressingShift = false; }

        break;
    }
    case (SDL_EVENT_MOUSE_BUTTON_DOWN):
    {
        if (_InputState.event->button.button == SDL_BUTTON_RIGHT)
        {
            SDL_RaiseWindow(SDL_GL_GetCurrentWindow());
            SDL_SetWindowRelativeMouseMode(SDL_GL_GetCurrentWindow(), true);
        }
        break;
    }
    case (SDL_EVENT_MOUSE_BUTTON_UP):
    {
        if (_InputState.event->button.button == SDL_BUTTON_RIGHT) {
            SDL_SetWindowRelativeMouseMode(SDL_GL_GetCurrentWindow(), false);
        }
        break;
    }
    case (SDL_EVENT_MOUSE_MOTION):
    {
        if (SDL_GetWindowRelativeMouseMode(SDL_GL_GetCurrentWindow()))
        {
            inputComponent.mouseInput.x = _InputState.event->motion.xrel;
            inputComponent.mouseInput.y = _InputState.event->motion.yrel;
        }
        break;
    }
    }
}

void CameraSystem::OnLateUpdate(GameState& _State, const float& _DeltaTime)
{
    const Entity& cameraEntity = *mEntities.begin();

    auto& inputComponent     = _State.coordinator->GetComponent<InputComponent>(cameraEntity);
    auto& transformComponent = _State.coordinator->GetComponent<TransformComponent>(cameraEntity);
    auto& cameraComponent = _State.coordinator->GetComponent<CameraComponent>(cameraEntity);

    glm::vec2 motion = inputComponent.mouseInput * m_CameraSensitivity;
    inputComponent.mouseInput = {};

    yaw += motion.x;
    pitch -= motion.y;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    const glm::vec3 forward = glm::normalize(direction);
    const glm::vec3 right = glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::vec3 up = glm::cross(right, forward);

    const auto& position = glm::vec3(transformComponent.transform[3]);

    const glm::mat4 inverted = glm::inverse(transformComponent.transform);

    glm::vec3 rightVelocity   = right * inputComponent.movementInput.x;
    glm::vec3 upVelocity      = up * inputComponent.movementInput.y;
    glm::vec3 forwardVelocity = forward * inputComponent.movementInput.z;

    glm::vec requestedVelocity = rightVelocity + upVelocity + forwardVelocity;

    float speed = inputComponent.isPressingShift ? m_CameraShiftSpeed : m_CameraSpeed;

    transformComponent.transform = glm::translate(transformComponent.transform, requestedVelocity * speed * _DeltaTime);

    const auto& windowSize = _State.engine->GetWindowSize();

    cameraComponent.view = glm::lookAt(position, position + forward, up);
    ;
    cameraComponent.projection = glm::perspective(glm::radians(45.0f), (float)windowSize.x / (float)windowSize.y, 0.1f, 1000.0f);
}

void CameraSystem::OnRenderStart(RenderState& _RenderState)
{
    const Entity& cameraEntity = *mEntities.begin();

    const auto& transformComponent = _RenderState.coordinator->GetComponent<TransformComponent>(cameraEntity);
    const auto& cameraComponent    = _RenderState.coordinator->GetComponent<CameraComponent>(cameraEntity);

    _RenderState.engine->UpdateCameraData(transformComponent.transform[3], cameraComponent.view, cameraComponent.projection);
}
