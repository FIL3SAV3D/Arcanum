#include "CameraComponent.h"

#include "Framework/Component_Based_Architecture/Entity.h"
#include "Framework/Component_Based_Architecture/TransformComponent.h"

void CameraComponent::SetPerspective(float fov, float aspect, float near, float far) {
    fieldOfView = fov;
    aspectRatio = aspect;
    nearPlane = near;
    farPlane = far;
    projectionDirty = true;
}

glm::mat4 CameraComponent::GetViewMatrix() const {
    // Get transform component
    auto transform = GetOwner()->GetComponent<TransformComponent>();
    if (transform) {
        // Calculate view matrix from transform
        glm::vec3 position = transform->GetPosition();
        glm::quat rotation = transform->GetRotation();

        // Forward vector (local -Z)
        glm::vec3 forward = rotation * glm::vec3(0.0f, 0.0f, -1.0f);
        // Up vector (local +Y)
        glm::vec3 up = rotation * glm::vec3(0.0f, 1.0f, 0.0f);

        return glm::lookAt(position, position + forward, up);
    }
    return glm::mat4(1.0f);
}

glm::mat4 CameraComponent::GetProjectionMatrix() const {
    if (projectionDirty) {
        projectionMatrix = glm::perspective(
            glm::radians(fieldOfView),
            aspectRatio,
            nearPlane,
            farPlane
        );
        projectionDirty = false;
    }
    return projectionMatrix;
}