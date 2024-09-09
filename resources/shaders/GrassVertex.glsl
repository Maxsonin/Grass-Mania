#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aOffset;

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

// Grass Settings
uniform float u_MinHeight;
uniform float u_MaxHeight;

out float grassHeight;

void main() 
{
    vec3 worldPosition = aPosition + vec3(aOffset.x, 0.0, aOffset.y);
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(worldPosition, 1.0);

    // Normalize the height for output
    grassHeight = (worldPosition.y - u_MinHeight) / (u_MaxHeight - u_MinHeight);
}