#version 330 core

in float grassHeight;

out vec4 FragColor;

void main() 
{
    vec3 darkGreen  = vec3(0.0, 0.5, 0.0);
    vec3 lightColor = vec3(0.5, 0.9, 0.5);

    vec3 color = mix(darkGreen, lightColor, grassHeight); // Interpolate between darkGreen and lightColor

    FragColor = vec4(color, 1.0);
}