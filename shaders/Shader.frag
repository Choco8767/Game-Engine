#version 450

layout(location = 0) in vec4 fragColor;

layout(location = 0) out vec4 outColor;

layout(std140, set = 0, binding = 0) uniform UniformData
{
    float time;
};

void main()
{
    float intensity = (sin(time * 5.0) + 1.0) * 0.5;

    vec3 color = vec3(intensity);

    outColor = vec4(color, 1.0);
}
