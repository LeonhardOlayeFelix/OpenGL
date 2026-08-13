#shader vertex
#version 330 core
        
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

layout (std140) uniform Matrices
{
    mat4 u_Proj;
    mat4 u_View;
};

uniform mat4 u_Model;

out VS_OUT 
{
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoord;
} vs_out;

void main()
{
    gl_Position = u_Proj * u_View * u_Model * vec4(a_Position, 1.0);
    vs_out.Normal = transpose(inverse(mat3(u_Model))) * a_Normal;
    vs_out.FragPos = vec3(u_Model * vec4(a_Position, 1.0));
    vs_out.TexCoord = a_TexCoord;
};

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

struct PointLight
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float Kc;
    float Kl;
    float Kq;
};
struct Material 
{
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
    float shininess;
};

uniform PointLight u_PointLight;

uniform Material u_WoodMaterial;

uniform vec3 u_ViewPosition;

in VS_OUT 
{
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoord;
} fs_in;

vec3 CalcPointLight(PointLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoord);

void main()
{
    color = vec4(0.0, 0.0, 0.0, 0.0);
    vec3 viewDir = normalize(u_ViewPosition - fs_in.FragPos);

    color += vec4(CalcPointLight(u_PointLight, u_WoodMaterial, fs_in.Normal, fs_in.FragPos, viewDir, fs_in.TexCoord), 1.0);

    //color = texture(u_WoodMaterial.texture_diffuse1, fs_in.TexCoord);
};


vec3 CalcPointLight(PointLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoord){
    vec3 lightDir   = normalize(light.position - fragPos);
    normal = normalize(normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    vec3 ambient  = vec3(texture(material.texture_diffuse1,  texCoord)) * light.ambient;
    vec3 diffuse  = vec3(texture(material.texture_diffuse1,  texCoord)) * light.diffuse  * max(dot(normal, lightDir), 0.0);
    vec3 specular = vec3(texture(material.texture_specular1, texCoord)) * light.specular * pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.Kc + light.Kl * distance + light.Kq * distance * distance);
    attenuation = 1.0;
    return ambient + attenuation * (diffuse + specular);
}

