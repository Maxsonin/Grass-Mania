#ifndef FRUSTUM
#define FRUSTUM

#include <glm/glm.hpp>
#include "../Camera.h"

struct Plane
{
    glm::vec3 normal = { 0.0f, 1.0f, 0.0f };

    // distance from origin to the nearest point in the plane
    float distanceToTheOrigin = 0.0f;

    Plane() = default;

    Plane(const glm::vec3& pointOnThePlane, const glm::vec3& normal)
        : distanceToTheOrigin(glm::dot(normal, pointOnThePlane)), normal(glm::normalize(normal))
    {}
};

struct CameraFrustum
{
    Plane topFace;   Plane bottomFace;
    Plane rightFace; Plane leftFace;
    Plane farFace;   Plane nearFace;
};

struct AABB 
{
    glm::vec3 center;
    glm::vec3 extents;

    AABB(const glm::vec3& min, const glm::vec3& max)
        : center{ (min + max) * 0.5f }, // Compute AABB center
        extents{ max - center }         // Compute positive extents

        /*
        * The center of the AABB is simply the midpoint of the box.
        * This gives us the center point around which the box is symmetrically extended.

        * The extents are half the lengths of the AABB along the x, y, and z axes.
        * They represent how far the AABB extends from its center in each direction.
        */
    {};

    inline void Update(const glm::vec3& min, const glm::vec3& max) { center = (min + max) * 0.5f; extents = max - center; }
    bool isOnFrustum(const CameraFrustum& frustum) const;
    bool isOnOrForwardPlane(const Plane& plane) const;
};

CameraFrustum CreateCameraFrustum(const Camera& camera);
void drawFrustum(GLuint shaderProgram, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const Camera& camera);
#endif