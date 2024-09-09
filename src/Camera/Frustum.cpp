#include "Frustum.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

CameraFrustum CreateCameraFrustum(const Camera& camera)
{
    CameraFrustum frustum;

    const float halfVAngle = camera.m_FOV * 0.75f   * camera.aspectRatio;
    const float halfHAngle = atanf(tanf(halfVAngle) * camera.aspectRatio);

    frustum.nearFace   = { camera.m_CameraPosition + camera.m_FRUSTUM_NEAR * camera.m_ViewDirection, camera.m_ViewDirection };
    frustum.farFace    = { camera.m_CameraPosition + camera.m_FRUSTUM_FAR * camera.m_ViewDirection, -camera.m_ViewDirection };

    frustum.rightFace  = { camera.m_CameraPosition, glm::rotate(camera.m_ViewDirection, halfHAngle,  camera.m_UpVector) };
    frustum.leftFace   = { camera.m_CameraPosition, glm::rotate(camera.m_ViewDirection, -halfHAngle, camera.m_UpVector) };

    frustum.topFace    = { camera.m_CameraPosition, glm::rotate(camera.m_ViewDirection, halfVAngle, camera.m_RightVector) };
    frustum.bottomFace = { camera.m_CameraPosition, glm::rotate(camera.m_ViewDirection, -halfVAngle,  camera.m_RightVector) };

    return frustum;
}

bool AABB::isOnFrustum(const CameraFrustum& frustum) const
{
    return (
           isOnOrForwardPlane(frustum.leftFace) && isOnOrForwardPlane(frustum.rightFace) &&
           isOnOrForwardPlane(frustum.topFace) && isOnOrForwardPlane(frustum.bottomFace) &&
           isOnOrForwardPlane(frustum.nearFace) && isOnOrForwardPlane(frustum.farFace)
           );
}
bool AABB::isOnOrForwardPlane(const Plane& plane) const
{
    // https://gdbooks.gitbooks.io/3dcollisions/content/Chapter2/static_aabb_plane.html
    
    // We are measuring how much the AABB extends in the direction of the plane's normal vector
    const float projectedExtentAlignment = extents.x * std::abs(plane.normal.x)
                                         + extents.y * std::abs(plane.normal.y)
                                         + extents.z * std::abs(plane.normal.z);

    // This tells us how far the center of the AABB is from the plane along the direction of the normal.
    float signedDistanceToPlane = glm::dot(plane.normal, center) - plane.distanceToTheOrigin;

    // If the signed distance from the AABB's center to the plane
    // is greater than or equal to the negative projected extent alignment,
    // then the AABB is considered to be "on" or "in front" of the plane.
    return -projectedExtentAlignment <= signedDistanceToPlane;
}

void drawFrustum(GLuint shaderProgram, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const Camera& camera)
{
    /*
        We will calculate the half-width and half-height of both the near and far planes using the tangent function.
        Then, we will find the center of the near and far planes, which, together with the previously calculated half-dimensions, 
        will allow us to determine the coordinates of each point that defines the frustum.
    */

    // For Frustum to look OK we set some limitations
    float frustum_far = camera.m_FRUSTUM_FAR;
    if (camera.m_FRUSTUM_FAR > 100.0f) { frustum_far = 100.0f; }

    // Calculate near and far plane dimensions
    float half_near_height = tan(glm::radians((float)camera.m_FOV) / 2.0f) * camera.m_FRUSTUM_NEAR;
    float half_near_width  = half_near_height * camera.aspectRatio;
    float half_far_height  = tan(glm::radians((float)camera.m_FOV) / 2.0f) * frustum_far;
    float half_far_width   = half_far_height * camera.aspectRatio;

    // Calculate near and far plane center points 
    glm::vec3 near_center = camera.m_CameraPosition + camera.m_ViewDirection * camera.m_FRUSTUM_NEAR;
    glm::vec3 far_center  = camera.m_CameraPosition + camera.m_ViewDirection * frustum_far;

    // Calculate near and far plane corner points
    glm::vec3 near_top_left     = near_center + (camera.m_UpVector * half_near_height) - (camera.m_RightVector * half_near_width);
    glm::vec3 near_top_right    = near_center + (camera.m_UpVector * half_near_height) + (camera.m_RightVector * half_near_width);
    glm::vec3 near_bottom_left  = near_center - (camera.m_UpVector * half_near_height) - (camera.m_RightVector * half_near_width);
    glm::vec3 near_bottom_right = near_center - (camera.m_UpVector * half_near_height) + (camera.m_RightVector * half_near_width);

    glm::vec3 far_top_left     = far_center + (camera.m_UpVector * half_far_height) - (camera.m_RightVector * half_far_width);
    glm::vec3 far_top_right    = far_center + (camera.m_UpVector * half_far_height) + (camera.m_RightVector * half_far_width);
    glm::vec3 far_bottom_left  = far_center - (camera.m_UpVector * half_far_height) - (camera.m_RightVector * half_far_width);
    glm::vec3 far_bottom_right = far_center - (camera.m_UpVector * half_far_height) + (camera.m_RightVector * half_far_width);

    // Frustum corner vertices
    glm::vec3 vertices[] =
    {
        // Near plane
        near_top_left, near_top_right,
        near_top_right, near_bottom_right,
        near_bottom_right, near_bottom_left,
        near_bottom_left, near_top_left,

        // Far plane
        far_top_left, far_top_right,
        far_top_right, far_bottom_right,
        far_bottom_right, far_bottom_left,
        far_bottom_left, far_top_left,

        // Connecting lines
        near_top_left, far_top_left,
        near_top_right, far_top_right,
        near_bottom_left, far_bottom_left,
        near_bottom_right, far_bottom_right
    };

    // Render Camera Frustum
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);

    glUseProgram(shaderProgram);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_ViewMatrix"),       1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glBindVertexArray(VAO);

    glDrawArrays(GL_LINES, 0, 24);

    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    glUseProgram(0);
}