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
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light {
    vec3 direction;
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float u_Kc;
    float u_Kl;
    float u_Kq;

    float cutOff;
    float outerCutOff;
};

uniform Material u_Material;
uniform Light u_Light;

uniform vec3 u_ViewPosition;

in vec3 v_Normal;
in vec3 v_FragPosition;
in vec2 v_TexCoords;

void main()
{

    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(u_Light.position - v_FragPosition);
    vec3 viewDir = normalize(u_ViewPosition - v_FragPosition);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 ambient  = vec3(texture(u_Material.diffuse, v_TexCoords))  * u_Light.ambient;
    vec3 diffuse  = vec3(texture(u_Material.diffuse, v_TexCoords))  * u_Light.diffuse  * max(dot(normal, lightDir), 0.0);
    vec3 specular = vec3(texture(u_Material.specular, v_TexCoords)) * u_Light.specular * pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    float distance = length(u_Light.position - v_FragPosition);
    float attenuation = 1.0 / (u_Light.u_Kc + u_Light.u_Kl * distance + u_Light.u_Kq * distance * distance);
    float theta = dot(lightDir, normalize(-u_Light.direction));
    float epsilon   = u_Light.cutOff - u_Light.outerCutOff;
    float intensity = clamp((theta - u_Light.outerCutOff) / epsilon, 0.0, 1.0);    

    vec3 result = ambient + attenuation * intensity * (diffuse + specular);

    


    color = vec4(result, 1.0f);
};

