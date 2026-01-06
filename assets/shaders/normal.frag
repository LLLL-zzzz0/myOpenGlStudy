#version 330 core
out vec4 FragColor;

uniform sampler2D sampler;

in vec2 uv;
in vec3 Normal; 

void main()
{
    vec3 norm1N = normalize(Normal);
    vec3 noramlColor = clamp(norm1N, 0.0, 1.0);
    vec4 textureColor = texture(sampler, uv);
    FragColor = vec4(noramlColor, textureColor.a);
}