#version 330 core
out vec4 color;

in vec3 LightingColor;

uniform vec3 objectColor;

void main()
{
    color = vec4(LightingColor * objectColor, 1.0);
}
