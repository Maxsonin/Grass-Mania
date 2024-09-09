#include "GrassRenderer.h"

#include <cstdlib>
#include <ctime>
#include <string>
#include "../Utility/Debugging.h"
#include "../Utility/Random.h"


GrassRenderer::GrassRenderer(CameraManager* cameraManager)
    : m_CameraManager(cameraManager)
{
    m_GrassMesh = Mesh("./resources/models/grass.obj");

    m_GrassShaderProgram = ShaderProgram("GrassShader", "./resources/shaders/GrassVertex.glsl", "./resources/shaders/GrassFragment.glsl");
    m_CameraShaderProgram = ShaderProgram("CameraShader", "./resources/shaders/CameraVertex.glsl", "./resources/shaders/CameraFragment.glsl");

#pragma region plane
    m_WorldShaderProgram = ShaderProgram("WorldShader", "./resources/shaders/WorldVertex.glsl", "./resources/shaders/WorldFragment.glsl");

    m_WorldShaderProgram.Bind();

    float plane[] =
    {
        0.0f,  0.0f, 0.0f,
        0.0f,  0.0f, 10.0f,
        10.0f, 0.0f, 10.0f,

        10.0f, 0.0f, 10.0f,
        10.0f,  0.0f, 0.0f,
        0.0f,  0.0f, 0.0f
    };

    GLuint worldVBO = 0;
    glGenBuffers(1, &worldVBO);
    glBindBuffer(GL_ARRAY_BUFFER, worldVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane), plane, GL_STATIC_DRAW);

    glGenVertexArrays(1, &worldVAO);
    glBindVertexArray(worldVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_WorldShaderProgram.Unbind();

    glCheckError();
#pragma endregion

    // Create Chunks
    int end = sqrt(m_NumOfChunks);
    int chunksLeft = m_NumOfChunks;
    for (size_t x = 0; x < end; x++)
    {
        for (size_t z = 0; z < end; z++)
        {
            if (chunksLeft <= 0) { return; }
            m_GrassChunks.push_back(GrassChunk(glm::vec2(m_ChunkSideLenght * x, m_ChunkSideLenght * z), m_ChunkSideLenght, m_MeshesPerChunk));
            chunksLeft--;
        }
    }
}

void GrassRenderer::Render()
{
#pragma region plane
    m_WorldShaderProgram.Bind();

    glm::mat4 planeModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(15.0f, 1.0f, 15.0f));
    m_WorldShaderProgram.setMat4("u_ModelMatrix", planeModelMatrix);

    m_WorldShaderProgram.setMat4("u_ViewMatrix", m_CameraManager->GetCameraInUse()->GetViewMatrix());
    m_WorldShaderProgram.setMat4("u_ProjectionMatrix", m_CameraManager->GetCameraInUse()->GetProjectionMatrix());

    glBindVertexArray(worldVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    m_WorldShaderProgram.Unbind();

    glCheckError();
#pragma endregion

    m_CameraShaderProgram.Bind();
    frustum = CreateCameraFrustum(*m_CameraManager->GetMainCamera());
    drawFrustum(m_CameraShaderProgram.m_RendererID, m_CameraManager->GetCameraInUse()->GetViewMatrix(), m_CameraManager->GetMainCamera()->GetProjectionMatrix(), *m_CameraManager->GetMainCamera());
    m_CameraShaderProgram.Unbind();

    glCheckError();

    m_GrassShaderProgram.Bind();

    m_GrassShaderProgram.setMat4("u_ViewMatrix", m_CameraManager->GetCameraInUse()->GetViewMatrix());
    m_GrassShaderProgram.setMat4("u_ProjectionMatrix", m_CameraManager->GetCameraInUse()->GetProjectionMatrix());

    m_GrassShaderProgram.setFloat("u_MinHeight", 5.0f); // Info from grass.obj
    m_GrassShaderProgram.setFloat("u_MaxHeight", 10.0f);

    for (GrassChunk grassChunk : m_GrassChunks)
    {
        if (!grassChunk.getCunkAABB().isOnFrustum(frustum)) continue; // Face Culling
        else grassChunk.Render(m_GrassShaderProgram, m_GrassMesh, frustum);
    }

    m_GrassShaderProgram.Unbind();
}