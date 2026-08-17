#shader vertex
#version 330 core
        
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

layout (std140) uniform Matrices
{
    mat4 u_Proj;
    mat4 u_View;
    mat4 u_DirectionalLightSpaceMatrix;
    mat4 u_PointLightSpaceMatrices[6];
    mat4 u_SpotLightSpaceMatrix;
};

uniform mat4 u_Model;

out VS_OUT 
{
    vec3 Normal;
    vec3 FragPosWorld;
    vec2 TexCoord;
    vec4 FragPosLightDirectional;
    vec4 FragPosLightSpot;
} vs_out;

void main()
{
    gl_Position = u_Proj * u_View * u_Model * vec4(a_Position, 1.0);
    vs_out.Normal = transpose(inverse(mat3(u_Model))) * a_Normal;
    vs_out.FragPosWorld = vec3(u_Model * vec4(a_Position, 1.0));
    vs_out.TexCoord = a_TexCoord;
    vs_out.FragPosLightDirectional = u_DirectionalLightSpaceMatrix * vec4(vs_out.FragPosWorld, 1.0);
    vs_out.FragPosLightSpot = u_SpotLightSpaceMatrix * vec4(vs_out.FragPosWorld, 1.0);
};

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

struct PointLight
{
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float Near;
    float Far;
    vec3 Position;
    float Kc;
    float Kl;
    float Kq;
};
struct DirectionalLight
{
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float Near;
    float Far;
    vec3 Direction;
};
struct SpotLight
{
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float Near;
    float Far;
    vec3 Position;
    vec3 Direction;
    float CutOff;
    float OuterCutOff;
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
    bool blinn;
};

uniform PointLight u_PointLight;
uniform PointLight u_PointLight2;
uniform PointLight u_PointLight3;
uniform PointLight u_PointLight4;
uniform DirectionalLight u_DirectionalLight;
uniform SpotLight u_SpotLight;

uniform Material u_WoodMaterial;

uniform vec3 u_ViewPosition;

uniform sampler2D u_DepthTexture1;
uniform sampler2D u_DepthTexture2;
uniform samplerCube u_DepthTexture3D;

in VS_OUT 
{
    vec3 Normal;
    vec3 FragPosWorld;
    vec2 TexCoord;
    vec4 FragPosLightDirectional;
    vec4 FragPosLightSpot;
} fs_in;

float CalcShadow2D(vec4 fragPosLightSpace, float bias, sampler2D shadowMap);
float CalcShadow3D(vec3 fragPosWorldSpace, vec3 lightPos, float farPlane, float bias);
vec3 CalcDirectionalLight(DirectionalLight light, Material material, vec3 normal, vec3 viewDir, vec2 texCoord, vec4 fragPosLight);
vec3 CalcPointLight(PointLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoord);
vec3 CalcSpotLight(SpotLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoord, vec4 fragPosLight);

void main()
{
   vec3 viewDir = normalize(u_ViewPosition - fs_in.FragPosWorld);

   color = vec4(CalcDirectionalLight(u_DirectionalLight, u_WoodMaterial, fs_in.Normal, viewDir, fs_in.TexCoord, fs_in.FragPosLightDirectional), 1.0);
   color += vec4(CalcPointLight(u_PointLight, u_WoodMaterial, fs_in.Normal, fs_in.FragPosWorld, viewDir, fs_in.TexCoord), 1.0);
   color += vec4(CalcSpotLight(u_SpotLight, u_WoodMaterial, fs_in.Normal, fs_in.FragPosWorld, viewDir, fs_in.TexCoord, fs_in.FragPosLightSpot), 1.0);

   color.rgb = pow(color.rgb, vec3(1.0/2.2));
};

float CalcShadow2D(vec4 fragPosLightSpace, float bias, sampler2D shadowMap)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    
    if(projCoords.z > 1.0)
        return 0.0;
    
    float currentDepth = projCoords.z;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    return shadow / 9.0;
}
float CalcShadow3D(vec3 fragPosWorldSpace, vec3 lightPos, float farPlane, float bias)
{
    vec3 fragToLight = fragPosWorldSpace - lightPos;
    float currentDepth = length(fragToLight);

    vec3 sampleOffsetDirections[20] = vec3[]
    (
        vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1),
        vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
        vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
        vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
        vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
    );

    float shadow = 0.0;
    int samples = 20;

    float viewDistance = length(u_ViewPosition - fragPosWorldSpace);
    float diskRadius = (1.0 + (viewDistance / farPlane)) / 25.0;

    for (int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(u_DepthTexture3D, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= farPlane;
        if (currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);

    return shadow;
}
vec3 CalcDirectionalLight(DirectionalLight light, Material material, vec3 normal, vec3 viewDir, vec2 texCoord, vec4 fragPosLightSpace){
    vec3 lightDir   = normalize(-light.Direction);
    normal = normalize(normal);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 ambient  = vec3(texture(material.texture_diffuse1,  texCoord)) * light.Ambient;
    vec3 diffuse  = vec3(texture(material.texture_diffuse1,  texCoord)) * light.Diffuse  * max(dot(normal, lightDir), 0.0);
    vec3 specular = vec3(texture(material.texture_specular1, texCoord)) * light.Specular * pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float shadow = CalcShadow2D(fragPosLightSpace, max(0.0005 * (1.0 - dot(normal, lightDir)), 0.00005), u_DepthTexture1);
    return ambient + (1.0 - shadow) * (diffuse + specular);
}

vec3 CalcPointLight(PointLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoord){
    vec3 lightDir   = normalize(light.Position - fragPos);
    normal = normalize(normal);
    float specularDot = 0.0;

    if (material.blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        specularDot = dot(normal, halfwayDir);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        specularDot = dot(viewDir, reflectDir);
    }

    vec3 ambient  = vec3(texture(material.texture_diffuse1,  texCoord)) * light.Ambient;
    vec3 diffuse  = vec3(texture(material.texture_diffuse1,  texCoord)) * light.Diffuse  * max(dot(normal, lightDir), 0.0);
    vec3 specular = vec3(texture(material.texture_specular1, texCoord)) * light.Specular * pow(max(specularDot, 0.0), material.shininess);

    float shadow = CalcShadow3D(fragPos, light.Position, light.Far, max(0.05 * (1.0 - dot(normal, lightDir)), 0.005));;
    float distance    = length(light.Position - fragPos);
    float attenuation = 1.0 / (light.Kc + light.Kl * distance + light.Kq * distance * distance);
    
    return attenuation * (ambient + (1.0 - shadow) * (diffuse + specular));
}

vec3 CalcSpotLight(SpotLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoord, vec4 fragPosLightSpace){
    vec3 lightDir = normalize(light.Position - fragPos);
    normal = normalize(normal);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 ambient  = vec3(texture(material.texture_diffuse1,  texCoord)) * light.Ambient;
    vec3 diffuse  = vec3(texture(material.texture_diffuse1,  texCoord)) * light.Diffuse  * max(dot(normal, lightDir), 0.0);
    vec3 specular = vec3(texture(material.texture_specular1, texCoord)) * light.Specular * pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance    = length(light.Position - fragPos);
    float attenuation = 1.0 / (light.Kc + light.Kl * distance + light.Kq * distance * distance);

    float shadow    = CalcShadow2D(fragPosLightSpace, max(0.0005 * (1.0 - dot(normal, lightDir)), 0.00005), u_DepthTexture2);
    float theta     = dot(lightDir, normalize(-light.Direction));
    float epsilon   = light.CutOff - light.OuterCutOff;
    float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);

    return attenuation * (ambient + intensity * (1.0 - shadow) * (diffuse + specular));
}

