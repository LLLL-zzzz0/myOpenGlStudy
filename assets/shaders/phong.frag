#version 330 core
out vec4 FragColor;

#define MAX_LIGHT_NUM 5

uniform sampler2D sampler;
uniform sampler2D specularMaskSampler;

uniform vec3 cameraPosition;
uniform vec3 ambientColor;
uniform float shiness;
uniform int spotLightNum;
uniform int pointLightNum;
uniform int dirLightNum;

in vec2 uv;
in vec3 Normal; 
in vec3 worldPosition;

struct DirectionalLight
{
    vec3 direction;
    vec3 color;
    float specularIntensity;
};

struct PointLight
{
    vec3 position;
    vec3 color;
    float specularIntensity;
    float k2;
    float k1;
    float kc;
};

struct SpotLight
{
    vec3 position;
    vec3 targetDirection;
    vec3 color;
    float outerLine;
    float innerLine;
    float specularIntensity;
};

uniform SpotLight spotLights[MAX_LIGHT_NUM];
uniform DirectionalLight directionalLights[MAX_LIGHT_NUM];
uniform PointLight pointLights[MAX_LIGHT_NUM];

//计算漫反射光照
vec3 calculateDiffuse(vec3 lightColor, vec3 objectColor, vec3 lightDir, vec3 normal)
{
 //漫反射
    float diffuse = clamp(dot(-lightDir, normal), 0.0f, 1.0f);
    vec3 diffuseColor = lightColor * diffuse * objectColor;

    return diffuseColor;
}

//计算镜面反射
vec3 calculateSpecular(vec3 lightColor, vec3 lightDir, vec3 normal, vec3 viewDir, float intensity)
{
    //镜面反射
    //防止背面光效果
    float dotResult = dot(-lightDir, normal);
    float flag = step(0.0, dotResult);

    vec3 lightReflect = reflect(lightDir, normal);
    float specular = clamp(dot(lightReflect, -viewDir), 0.0f, 1.0f);

    //控制光斑大小
    specular = pow(specular, shiness);

    float specularMask = texture(specularMaskSampler, uv).r;

    vec3 specularColor = lightColor * specular * flag * intensity * specularMask;

    return specularColor;
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{
    vec3 objectColor = texture(sampler, uv).xyz;
    vec3 lightDir = normalize(worldPosition - light.position);
    vec3 targetDirection = normalize(light.targetDirection);

    //计算spotlight的照射范围
    float fCosGamma = dot(lightDir, targetDirection);
    float intensity = clamp((fCosGamma - light.outerLine) / (light.innerLine - light.outerLine), 0.0,1.0);

    //计算diffuse
    vec3 diffuseColor = calculateDiffuse(light.color, objectColor, lightDir, normal);

    //计算specular
    vec3 specularColor = calculateSpecular(light.color, lightDir, normal, viewDir, light.specularIntensity);

    return (diffuseColor + specularColor) * intensity;
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 objectColor = texture(sampler, uv).xyz;
    vec3 lightDir = normalize(light.direction);

     //计算diffuse
    vec3 diffuseColor = calculateDiffuse(light.color, objectColor, lightDir, normal);

    //计算specular
    vec3 specularColor = calculateSpecular(light.color, lightDir, normal, viewDir, light.specularIntensity);
    
    return diffuseColor + specularColor;
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir)
{
    vec3 objectColor = texture(sampler, uv).xyz;
    vec3 lightDir = normalize(worldPosition - light.position);

    //计算衰减
    float dist = length(worldPosition - light.position);
    float attenuation = 1.0 / (light.k2 * dist * dist + light.k1 * dist + light.kc);

     //计算diffuse
    vec3 diffuseColor = calculateDiffuse(light.color, objectColor, lightDir, normal);

    //计算specular
    vec3 specularColor = calculateSpecular(light.color, lightDir, normal, viewDir, light.specularIntensity);

    return (diffuseColor + specularColor) * attenuation;
}

void main()
{
    vec3 resultColor = vec3(0.0, 0.0, 0.0);

    vec3 objectColor = texture(sampler, uv).xyz;
    vec3 norm1N = normalize(Normal);
    vec3 viewDir1N = normalize(worldPosition - cameraPosition);

    for(int i = 0; i < spotLightNum; i++)
    {
        resultColor += calculateSpotLight(spotLights[i], norm1N, viewDir1N);
    }

    for(int i = 0; i < dirLightNum; i++)
    {
        resultColor += calculateDirectionalLight(directionalLights[i], norm1N, viewDir1N);
    }

    for(int i = 0; i < pointLightNum; i++)
    {
        resultColor += calculatePointLight(pointLights[i], norm1N, viewDir1N);
    }

    vec3 ambientColor = objectColor * ambientColor;

    vec3 finalColor = resultColor  + ambientColor;
    
    FragColor = vec4(finalColor, 1.0f);
}