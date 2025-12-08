#include "OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

OrthographicCamera::OrthographicCamera(const Frustrum &frustrum, const glm::vec3 &position, float rotation)
{
    this->CameraFrustrum = frustrum;
    this->Rotation = rotation;
    this->Position = position;
    this->RecalculateMatrix();
}

void OrthographicCamera::RecalculateMatrix()
{
    // Projection matrix
    this->ProjectionMatrix = glm::ortho(
        this->CameraFrustrum.left,
        this->CameraFrustrum.right,
        this->CameraFrustrum.bottom,
        this->CameraFrustrum.top,
        this->CameraFrustrum.near,
        this->CameraFrustrum.far
    );

    // View Matrix
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, this->Position);
    transform = glm::rotate(transform, glm::radians(this->Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    this->ViewMatrix = glm::inverse(transform);

    // Combined viewprojection matrix
    this->ViewProjectionMatrix = this->ProjectionMatrix * this->ViewMatrix;
}
