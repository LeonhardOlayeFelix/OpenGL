#shader vertex
#version 330 core
        
layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Normal;
layout(location = 2) in vec2 a_TexCoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;
uniform mat3 u_Normal;

out vec3 v_Normal;
out vec3 v_FragPosition;
out vec2 v_TexCoords;

void main()
{
    gl_Position = u_Proj * u_View * u_Model * a_Position;
    v_FragPosition = vec3(u_Model * a_Position);
    v_Normal = u_Normal * vec3(a_Normal);
    v_TexCoords = a_TexCoords;
};

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

struct Material 
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D emission;
    float shininess;
};

struct DirLight 
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;
    float Kc;
    float Kl;
    float Kq;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    float Kc;
    float Kl;
    float Kq;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 4
uniform PointLight u_PointLights[NR_POINT_LIGHTS];
uniform DirLight   u_DirLight;
uniform PointLight u_PointLight;
uniform SpotLight  u_SpotLight;
uniform Material   u_Material;
uniform vec3       u_ViewPosition;

in vec3 v_Normal;
in vec3 v_FragPosition;
in vec2 v_TexCoords;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 normal  = normalize(v_Normal);
    vec3 viewDir = normalize(u_ViewPosition - v_FragPosition);

    vec3 result = CalcDirLight(u_DirLight, normal, viewDir);
    result += CalcPointLight(u_PointLight, normal, v_FragPosition, viewDir);

    //for(int i = 0; i < NR_POINT_LIGHTS; i++)
    //    result += CalcPointLight(u_PointLights[i], normal, v_FragPosition, viewDir);
    //
    //result += CalcSpotLight(u_SpotLight, normal, v_FragPosition, viewDir);

    color = vec4(result, 1.0f);
};

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir   = normalize(-light.direction);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 ambient  = vec3(texture(u_Material.texture_diffuse1,  v_TexCoords)) * light.ambient;
    vec3 diffuse  = vec3(texture(u_Material.texture_diffuse1,  v_TexCoords)) * light.diffuse  * max(dot(normal, lightDir), 0.0);
    vec3 specular = vec3(texture(u_Material.texture_specular1, v_TexCoords)) * light.specular * pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);

    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir   = normalize(light.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 ambient  = vec3(texture(u_Material.texture_diffuse1,  v_TexCoords)) * light.ambient;
    vec3 diffuse  = vec3(texture(u_Material.texture_diffuse1,  v_TexCoords)) * light.diffuse  * max(dot(normal, lightDir), 0.0);
    vec3 specular = vec3(texture(u_Material.texture_specular1, v_TexCoords)) * light.specular * pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.Kc + light.Kl * distance + light.Kq * distance * distance);

    return ambient + attenuation * (diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir   = normalize(light.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 ambient  = vec3(texture(u_Material.texture_diffuse1,  v_TexCoords)) * light.ambient;
    vec3 diffuse  = vec3(texture(u_Material.texture_diffuse1,  v_TexCoords)) * light.diffuse  * max(dot(normal, lightDir), 0.0);
    vec3 specular = vec3(texture(u_Material.texture_specular1, v_TexCoords)) * light.specular * pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.Kc + light.Kl * distance + light.Kq * distance * distance);

    float theta     = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    return ambient + attenuation * intensity * (diffuse + specular);
}