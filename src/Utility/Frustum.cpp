#include "Frustum.h"

CameraFrustum CreateFrustumOfCamera(const Camera& camera, float aspect)
{
    CameraFrustum frustum;

    const float halfVAngle = camera.m_FOV * 0.5f * 1.5;
    const float halfHAngle = atanf(tanf(halfVAngle) * aspect);

    frustum.nearFace   = { camera.m_CameraPosition + camera.m_NEAR * camera.m_ViewDirection, camera.m_ViewDirection  };
    frustum.farFace    = { camera.m_CameraPosition + camera.m_FAR  * camera.m_ViewDirection, -camera.m_ViewDirection };

    frustum.rightFace  = { camera.m_CameraPosition, glm::rotate(camera.m_ViewDirection, halfHAngle,  camera.m_UpVector) };
    frustum.leftFace   = { camera.m_CameraPosition, glm::rotate(camera.m_ViewDirection, -halfHAngle, camera.m_UpVector) };

    frustum.topFace    = { camera.m_CameraPosition, glm::rotate(camera.m_ViewDirection, -halfVAngle, camera.m_Right) };
    frustum.bottomFace = { camera.m_CameraPosition, glm::rotate(camera.m_ViewDirection, halfVAngle,  camera.m_Right) };

    return frustum;
}

bool AABB::isOnFrustum(const CameraFrustum& frustum) const
{
    return (
        isOnOrForwardPlane(frustum.leftFace) &&
        isOnOrForwardPlane(frustum.rightFace) &&
        isOnOrForwardPlane(frustum.topFace) &&
        isOnOrForwardPlane(frustum.bottomFace) &&
        isOnOrForwardPlane(frustum.nearFace) &&
        isOnOrForwardPlane(frustum.farFace)
        );
}
bool AABB::isOnOrForwardPlane(const Plane& plane) const
{
    // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
    const float r = extents.x * std::abs(plane.normal.x) + extents.y * std::abs(plane.normal.y) + extents.z * std::abs(plane.normal.z);

    return -r <= plane.getSignedDistanceToPlane(center);
}

float Plane::getSignedDistanceToPlane(const glm::vec3& point) const
{
    return glm::dot(normal, point) - distanceToTheOrigin;
}
