#include "GrassRenderer.h"

#include <cstdlib>
#include <ctime>
#include <string>
#include "Debugging.h"

GrassRenderer::GrassRenderer(Camera* camera)
    : m_Camera(camera)
{
    m_GrassMesh = Mesh("./resources/models/grass.obj");

    m_GrassShaderProgram = ShaderProgram("GrassShader", "./resources/shaders/GrassVertex.glsl", "./resources/shaders/GrassFragment.glsl");

    // Placing Logic Here
    m_GrassPositions = { {3.0f, 0.0f, 0.0f} };
}

void GrassRenderer::Render()
{
    m_GrassShaderProgram.Bind();

    m_GrassShaderProgram.setMat4("u_ViewMatrix", m_Camera->GetViewMatrix());
    m_GrassShaderProgram.setMat4("u_ProjectionMatrix", m_Camera->GetProjectionMatrix());
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), m_GrassPositions[0]);
    m_GrassShaderProgram.setMat4("u_ModelMatrix", modelMatrix);

    glCheckError();
    m_GrassMesh.Render(m_GrassShaderProgram);
    glCheckError();

    m_GrassShaderProgram.Unbind();
}