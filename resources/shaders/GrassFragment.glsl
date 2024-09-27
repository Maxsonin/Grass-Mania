#version 330 core

in float grassHeight;

out vec4 FragColor;

void main() 
{
    vec3 lightColor  = vec3(0.5, 0.9, 0.5);
    vec3 darkColor   = vec3(0.0, 0.5, 0.0);

    vec3 finalColor = mix(darkColor, lightColor, grassHeight);

    FragColor = vec4(finalColor, 1.0);
}