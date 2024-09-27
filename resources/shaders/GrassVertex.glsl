#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aOffset;

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

const float frequency = 0.5;
const float amplitude = 0.5;

// Grass Settings
uniform float u_MinHeight;
uniform float u_MaxHeight;

uniform float u_Time;

out float grassHeight;

void main() 
{
    vec3 worldPosition = aPosition + vec3(aOffset.x, 0.0, aOffset.y); 

    if (worldPosition.y >= 2)
    {
        worldPosition.x += sin(worldPosition.x * frequency + u_Time) * amplitude;
        worldPosition.z += sin(worldPosition.z * frequency + u_Time) * amplitude;
    }


    gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(worldPosition, 1.0);

    // Normalize the height for output
    grassHeight = (worldPosition.y - u_MinHeight) / (u_MaxHeight - u_MinHeight);
}