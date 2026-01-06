#version 330 core
out vec4 FragColor;

uniform sampler2D sampler;
uniform sampler2D specularMaskSampler;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float k2;
uniform float k1;
uniform float kc;

uniform vec3 cameraPosition;
uniform vec3 ambientColor;
uniform float specularIntensity; //光强度
uniform float shiness; 

in vec2 uv;
in vec3 Normal; 
in vec3 worldPosition;

void main()
{
    vec3 textureColor = texture(sampler, uv).xyz;

    vec3 norm1N = normalize(Normal);
    vec3 lightDir1N = normalize(worldPosition - lightPosition);
    vec3 viewDir1N = normalize(worldPosition - cameraPosition);

    //计算衰减
    float dist = length(worldPosition - lightPosition);
    float attenuation = 1.0 / (k2 * dist * dist + k1 * dist + kc);

    //漫反射
    float diffuse = clamp(dot(-lightDir1N, norm1N), 0.0f, 1.0f);
    vec3 diffuseColor = lightColor * diffuse * textureColor;

    //镜面反射
    //防止背面光效果
    float dotResult = dot(-lightDir1N, norm1N);
    float flag = step(0.0, dotResult);

    vec3 lightReflect1N = normalize(reflect(lightDir1N, norm1N));
    float specular = clamp(dot(lightReflect1N, -viewDir1N), 0.0f, 1.0f);

    //控制光斑大小
    specular = pow(specular, shiness);

    float specularMask = texture(specularMaskSampler, uv).r;

    vec3 specularColor = lightColor * specular * flag * specularIntensity * specularMask;

    vec3 ambientColor = ambientColor * textureColor;

    vec3 finalColor = (diffuseColor + specularColor) * attenuation + ambientColor;
    
    FragColor = vec4(finalColor, 1.0f);
}