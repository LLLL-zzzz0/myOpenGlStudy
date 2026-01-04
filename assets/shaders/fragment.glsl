#version 330 core
out vec4 FragColor;

uniform sampler2D sampler;

in vec2 uv;
in vec3 Normal; 
in vec3 FragPos;  

uniform vec3 lightPos;  
uniform vec3 lightColor;

void main()
{

    vec3 ambient = vec3(0.1) * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);  
    vec4 textureColor = texture(sampler, uv);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * textureColor.rgb;
    FragColor = vec4(result, textureColor.a);
}