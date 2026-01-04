#version 330 core

// 输入
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

// 输出
out vec4 FragColor;

// 材质结构
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    
    bool useDiffuseTexture;
    bool useSpecularTexture;
};

// 光源结构
struct Light {
    int type;  // 0:方向光, 1:点光源, 2:聚光灯
    
    vec3 position;
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    // 衰减参数
    float constant;
    float linear;
    float quadratic;
    
    // 聚光灯参数
    float cutOff;
    float outerCutOff;
};

// Uniforms
uniform Material material;
uniform Light light;
uniform vec3 viewPos;  // 相机位置

// 纹理
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

// 函数声明
vec3 calcDirectionalLight(Light light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    // 属性
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 result;
    
    // 根据光源类型计算光照
    if (light.type == 0) {
        // 方向光
        result = calcDirectionalLight(light, norm, viewDir);
    } else if (light.type == 1) {
        // 点光源
        result = calcPointLight(light, norm, FragPos, viewDir);
    } else if (light.type == 2) {
        // 聚光灯
        result = calcSpotLight(light, norm, FragPos, viewDir);
    } else {
        // 默认：方向光
        result = calcDirectionalLight(light, norm, viewDir);
    }
    
    FragColor = vec4(result, 1.0);
}

// 计算方向光
vec3 calcDirectionalLight(Light light, vec3 normal, vec3 viewDir) {
    // 光源方向（方向光的方向是固定的，从光源指向物体）
    vec3 lightDir = normalize(-light.direction);
    
    // 漫反射
    float diff = max(dot(normal, lightDir), 0.0);
    
    // 镜面反射
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // 计算各分量
    vec3 ambient = light.ambient * material.ambient;
    
    vec3 diffuse = light.diffuse * diff;
    if (material.useDiffuseTexture) {
        diffuse *= texture(diffuseTexture, TexCoord).rgb;
    } else {
        diffuse *= material.diffuse;
    }
    
    vec3 specular = light.specular * spec;
    if (material.useSpecularTexture) {
        specular *= texture(specularTexture, TexCoord).rgb;
    } else {
        specular *= material.specular;
    }
    
    return (ambient + diffuse + specular);
}

// 计算点光源
vec3 calcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    // 光源方向
    vec3 lightDir = normalize(light.position - fragPos);
    
    // 漫反射
    float diff = max(dot(normal, lightDir), 0.0);
    
    // 镜面反射
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // 衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                               light.quadratic * (distance * distance));
    
    // 计算各分量
    vec3 ambient = light.ambient * material.ambient;
    
    vec3 diffuse = light.diffuse * diff;
    if (material.useDiffuseTexture) {
        diffuse *= texture(diffuseTexture, TexCoord).rgb;
    } else {
        diffuse *= material.diffuse;
    }
    
    vec3 specular = light.specular * spec;
    if (material.useSpecularTexture) {
        specular *= texture(specularTexture, TexCoord).rgb;
    } else {
        specular *= material.specular;
    }
    
    // 应用衰减
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}

// 计算聚光灯
vec3 calcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    // 光源方向
    vec3 lightDir = normalize(light.position - fragPos);
    
    // 检查是否在聚光灯锥形内
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    // 漫反射
    float diff = max(dot(normal, lightDir), 0.0);
    
    // 镜面反射
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // 衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                               light.quadratic * (distance * distance));
    
    // 计算各分量
    vec3 ambient = light.ambient * material.ambient;
    
    vec3 diffuse = light.diffuse * diff;
    if (material.useDiffuseTexture) {
        diffuse *= texture(diffuseTexture, TexCoord).rgb;
    } else {
        diffuse *= material.diffuse;
    }
    
    vec3 specular = light.specular * spec;
    if (material.useSpecularTexture) {
        specular *= texture(specularTexture, TexCoord).rgb;
    } else {
        specular *= material.specular;
    }
    
    // 应用衰减和强度
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    
    return (ambient + diffuse + specular);
}