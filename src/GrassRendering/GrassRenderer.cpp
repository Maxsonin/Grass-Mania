#include "GrassRenderer.h"

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <string>
#include "../Utility/Debugging.h"
#include "../Utility/Random.h"


GrassRenderer::GrassRenderer(CameraManager* cameraManager)
    : m_CameraManager(cameraManager)
{
    m_GrassMesh = Mesh("./resources/models/grass.obj");
    m_LowPollyGrassMesh = Mesh("./resources/models/lowPolyGrass.obj");

    m_GrassShaderProgram = ShaderProgram("GrassShader", "./resources/shaders/GrassVertex.glsl", "./resources/shaders/GrassFragment.glsl");
    m_CameraShaderProgram = ShaderProgram("CameraShader", "./resources/shaders/CameraVertex.glsl", "./resources/shaders/CameraFragment.glsl");

    glCheckError();

    // Start measuring time
    auto start = std::chrono::high_resolution_clock::now();

    // Set number of m_NumOfLayars
    int numChunks = (2 * m_NumOfLayars + 1) * (2 * m_NumOfLayars + 1); // Calculate total chunks for the specified m_NumOfLayars
    int chunksLeft = numChunks;

    // Create Chunks in a square-like expanding pattern from (0, 0)
    int currentLayer = 0;

    while (chunksLeft > 0 && currentLayer < m_NumOfLayars)
    {
        for (int x = -currentLayer; x <= currentLayer && chunksLeft > 0; ++x)
        {
            for (int z = -currentLayer; z <= currentLayer && chunksLeft > 0; ++z)
            {
                // Check if the current position is on the boundary of the current layer
                if (abs(x) == currentLayer || abs(z) == currentLayer)
                {
                    m_GrassChunks.push_back(GrassChunk(glm::vec2(m_ChunkSideLenght * x, m_ChunkSideLenght * z), m_ChunkSideLenght, m_MeshesPerChunk));
                    chunksLeft--;
                }
            }
        }
        currentLayer++; // Move to the next layer
    }

    // End measuring time
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Chunks created: " << m_GrassChunks.size() << std::endl;
    std::cout << "Grass Meshes: " << m_GrassChunks.size() * m_MeshesPerChunk << std::endl;
    std::cout << "Time taken to create chunks: " << elapsed.count() << " seconds" << std::endl;
}

void GrassRenderer::Render()
{
    frustum = CreateCameraFrustum(*m_CameraManager->GetMainCamera());
    if (m_CameraManager->GetMainCamera() != m_CameraManager->GetCameraInUse())
    {
        m_CameraShaderProgram.Bind();
        drawFrustum(m_CameraShaderProgram.m_RendererID, m_CameraManager->GetCameraInUse()->GetViewMatrix(), m_CameraManager->GetMainCamera()->GetProjectionMatrix(), *m_CameraManager->GetMainCamera());
        m_CameraShaderProgram.Unbind();
        glCheckError();
    }

    m_GrassShaderProgram.Bind();

    m_GrassShaderProgram.setMat4("u_ViewMatrix", m_CameraManager->GetCameraInUse()->GetViewMatrix());
    m_GrassShaderProgram.setMat4("u_ProjectionMatrix", m_CameraManager->GetCameraInUse()->GetProjectionMatrix());

    m_GrassShaderProgram.setFloat("u_MinHeight", 5.0f);
    m_GrassShaderProgram.setFloat("u_MaxHeight", 10.0f);

    m_GrassShaderProgram.setFloat("u_Time", glfwGetTime());

    for (GrassChunk grassChunk : m_GrassChunks)
    {
        if (!grassChunk.getCunkAABB().isOnFrustum(frustum)) continue; // Face Culling

        float distance = glm::length(grassChunk.getPosition() - m_CameraManager->GetMainCamera()->m_CameraPosition);
        Mesh& meshToRender = distance < m_CameraManager->GetMainCamera()->m_DistOfLowPolly ? m_GrassMesh : m_LowPollyGrassMesh;
        grassChunk.Render(m_GrassShaderProgram, meshToRender, frustum);
    }

    m_GrassShaderProgram.Unbind();
}