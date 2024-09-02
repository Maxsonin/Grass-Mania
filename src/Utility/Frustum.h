#ifndef FRUSTUM
#define FRUSTUM
// more: https://learnopengl.com/Guest-Articles/2021/Scene/Frustum-Culling

#include <glm/glm.hpp>
#include "../Camera.h"

struct Plane
{
    glm::vec3 normal = { 0.0f, 1.0f, 0.0f };

    // distance from origin to the nearest point in the plane
    float distanceToTheOrigin = 0.0f;

    Plane() = default;

    Plane(const glm::vec3& pointOnThePlane, const glm::vec3& norm)
        : normal(glm::normalize(norm)), distanceToTheOrigin(glm::dot(normal, pointOnThePlane))
    {}

    float getSignedDistanceToPlane(const glm::vec3& point) const;
};

struct CameraFrustum
{
    Plane topFace;
    Plane bottomFace;

    Plane rightFace;
    Plane leftFace;

    Plane farFace;
    Plane nearFace;
};

struct AABB 
{
    glm::vec3 center = { 0.0f, 0.0f, 0.0f };
    glm::vec3 extents = { 0.0f, 0.0f, 0.0f };

    AABB(const glm::vec3& min, const glm::vec3& max)
        : center{ (max + min) * 0.5f },
        extents{ max.x - center.x, max.y - center.y, max.z - center.z }
    {};

    bool isOnFrustum(const CameraFrustum& frustum) const;
    bool isOnOrForwardPlane(const Plane& plane) const;
};

CameraFrustum CreateFrustumOfCamera(const Camera& cam, float aspect);

#endif