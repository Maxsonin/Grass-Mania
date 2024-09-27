#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal; // Assuming this is needed
layout(location = 2) in vec2 aOffset;

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

const vec2 windDirection = normalize(vec2(0.2, 0.5));
const float windSpeed = 0.2;
const float windDisplacement = 0.0001;

uniform float u_MinHeight;
uniform float u_MaxHeight;

uniform float u_Time;

out float height;
out vec4 eyeSpacePosition;
out float grassHeight;

// Function to create rotation around the Y-axis
mat4 rotationY(in float angle) 
{
    return mat4(cos(angle), 0, sin(angle), 0,
                0, 1.0, 0, 0,
                -sin(angle), 0, cos(angle), 0,
                0, 0, 0, 1);
}

// Random number generator function
float rand(vec2 co) { return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453); }

// Simple noise function
float noise(vec2 p) {
    vec2 i = floor(p); // Integer part
    vec2 f = fract(p); // Fractional part

    // Compute noise contributions from the corners of the square
    float a = sin(dot(i, vec2(127.1, 311.7))) * 43758.5453;
    float b = sin(dot(i + vec2(1.0, 0.0), vec2(127.1, 311.7))) * 43758.5453;
    float c = sin(dot(i + vec2(0.0, 1.0), vec2(127.1, 311.7))) * 43758.5453;
    float d = sin(dot(i + vec2(1.0, 1.0), vec2(127.1, 311.7))) * 43758.5453;

    // Interpolate along x and y axes
    float u = f.x * f.x * (3.0 - 2.0 * f.x);
    float v = f.y * f.y * (3.0 - 2.0 * f.y);
    return mix(mix(a, b, u), mix(c, d, u), v) * 0.5; // Scale the noise
}

// Displacement map function based on grass position
float getDisplacementMap(vec2 grassPosition) 
{
    // Add noise to the grass position to break the pattern
    float noiseFactor = noise(grassPosition * 100.0); // Scale noise for better effect
    return abs(sin((grassPosition.x * windDirection.x + grassPosition.y * windDirection.y + noiseFactor) + windSpeed * u_Time)) * 1.3 +
           (sin(u_Time * 10 + rand(grassPosition) * 40) * 0.03);
}

void main() 
{
    vec3 worldPosition = aPosition;
    vec2 grassPosition = aOffset;

    float random = rand(grassPosition);
    float localWindVariance = random * 0.5;
    float rotation = rand(grassPosition + vec2(5)) * 360.0;

    vec3 localPos = (rotationY(rotation) * vec4(worldPosition, 1.0)).xyz;
    vec3 finalPosition = vec3(localPos.x + grassPosition.x, localPos.y, localPos.z + grassPosition.y);

    height = finalPosition.y / 0.06; // Adjust height based on finalPosition

    // Calculate displacement based on grass position
    vec2 displacement = getDisplacementMap(grassPosition) * windDirection;
    finalPosition += vec3(displacement.x + localWindVariance, 0, displacement.y + localWindVariance) * (height * height) * windDisplacement;

    gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(finalPosition, 1.0); // Use the original uniforms

    eyeSpacePosition = u_ViewMatrix * vec4(finalPosition, 1.0); // Maintain uniform usage

    // Normalize the height for output
    grassHeight = (worldPosition.y - u_MinHeight) / (u_MaxHeight - u_MinHeight);
}