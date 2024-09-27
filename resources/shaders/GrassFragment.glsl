#version 330 core

in float grassHeight; 

out vec4 FragColor;

const float near = 0.001;
const float far  = 10000.0;

float linearizeDepth(float depth)
{
    return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
}

float logisticDepth(float depth, float steepness, float offset)
{
    float zVal = linearizeDepth(depth);
    return (1.0 / (1.0 + exp(-steepness * (zVal - offset))));
}

void main() 
{
    vec3 lightColor  = vec3(0.5, 0.9, 0.5); // Light green color for grass
    vec3 darkColor   = vec3(0.0, 0.5, 0.0); // Dark green color for grass

    vec3 finalColor = mix(darkColor, lightColor, grassHeight);

    // Calculate depth using gl_FragCoord.z
    float depth = logisticDepth(gl_FragCoord.z, 0.02, 400.0); // Increased steepness and offset for smoother fog

    // Apply fog effect
    FragColor = vec4(finalColor, 1.0) * (1.0 - depth) + vec4(depth * vec3(0.0, 0.0, 0.0), 1.0);
} 