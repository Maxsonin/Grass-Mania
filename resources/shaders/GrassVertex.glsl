#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

// Grass Settings
uniform float u_MinHeight;
uniform float u_MaxHeight;

out float grassHeight;

void main() 
{
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(aPosition, 1.0);
    grassHeight = (aPosition.y - u_MinHeight) / (u_MaxHeight - u_MinHeight); // Normalize height
}