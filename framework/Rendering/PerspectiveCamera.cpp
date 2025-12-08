#include "PerspectiveCamera.h"
#include <glm/gtc/matrix_transform.hpp>


PerspectiveCamera::PerspectiveCamera(const Frustrum &frustrum, const glm::vec3 &position, const glm::vec3 &lookAt, const glm::vec3 &upVector)
{
    this->CameraFrustrum = frustrum;
    this->Position = position;
    this->LookAt = lookAt;
    this->UpVector = upVector;
    this->RecalculateMatrix();
}

void PerspectiveCamera::RecalculateMatrix()
{
    // Projection matrix
    this->ProjectionMatrix = glm::perspective(
        glm::radians(this->CameraFrustrum.angle),
        this->CameraFrustrum.width / this->CameraFrustrum.height,
        this->CameraFrustrum.near,
        this->CameraFrustrum.far
    );

    // View matrix
    this->ViewMatrix = glm::lookAt(
        this->Position,
        this->LookAt,
        this->UpVector
    );

    // Combined viewprojection matrix
    this->ViewProjectionMatrix = this->ProjectionMatrix * this->ViewMatrix;
}
