#shader vertex
#version 330 core
        
layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;
uniform mat3 u_Normal;

out vec3 v_Normal;
out vec3 v_FragPosition;

void main()
{
    gl_Position = u_Proj * u_View * u_Model * a_Position;
    v_FragPosition = vec3(u_Model * a_Position);
    v_Normal = u_Normal * vec3(a_Normal);
};

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

struct Material 
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material u_Material;
uniform Light u_Light;

uniform vec3 u_ViewPosition;

uniform float u_Kc;
uniform float u_Kl;
uniform float u_Kq;

in vec3 v_Normal;
in vec3 v_FragPosition;

void main()
{

    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(u_Light.position - v_FragPosition);
    vec3 viewDir = normalize(u_ViewPosition - v_FragPosition);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 ambient  = u_Material.ambient  * u_Light.ambient;
    vec3 diffuse  = u_Material.diffuse  * u_Light.diffuse  * max(dot(normal, lightDir), 0.0);
    vec3 specular = u_Material.specular * u_Light.specular * pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    float distance = length(u_Light.position - v_FragPosition);
    float attenuation = 1.0 / (u_Kc + u_Kl * distance + u_Kq * distance * distance);

    vec3 result = ambient + attenuation * (diffuse + specular);

    color = vec4(result, 1.0f);
};

