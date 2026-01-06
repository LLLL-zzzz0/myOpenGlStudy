#version 330 core
out vec4 FragColor;

uniform sampler2D sampler;
uniform vec3 lightDirection;
uniform vec3 lightColor;

in vec2 uv;
in vec3 Normal; 

void main()
{
    vec3 textureColor = texture(sampler, uv).xyz;

    vec3 norm1N = normalize(Normal);
    vec3 lightDir1N = normalize(lightDirection);

    float diffuse = clamp(dot(-lightDir1N, norm1N), 0.0f, 1.0f);

    vec3 finalColor = lightColor * diffuse * textureColor;
    
    FragColor = vec4(finalColor, 1.0f);
}