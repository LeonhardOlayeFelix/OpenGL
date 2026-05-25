#shader vertex
#version 330 core
        
layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;
uniform mat3 u_Normal;

uniform vec3  u_Albedo;

uniform vec3  u_LightIntensity;
uniform vec3  u_LightPosition;

uniform vec3 u_ViewPosition;

uniform float u_Ka;
uniform float u_Kd;
uniform float u_Ks;
uniform float u_Shininess;

out vec4 v_Color;


void main()
{
    gl_Position = u_Proj * u_View * u_Model * a_Position;
    vec3 normal = u_Normal * a_Normal.xyz;

    vec3 I_a = u_LightIntensity;
    vec3 I_p = u_LightIntensity;
    vec3 I_s = u_LightIntensity;

    vec3 lightDir = (u_LightPosition - a_Position.xyz);
    vec3 N = normalize(normal);
    vec3 L = normalize(lightDir);

    vec3 viewDir = normalize(u_ViewPosition - a_Position.xyz);
    vec3 reflectDir = reflect(-L, N);

    vec3 ambient = u_Ka * I_a;
    vec3 diffuse = u_Kd * I_p * max(dot(N, L), 0.0);
    vec3 specular = u_Ks * I_s * pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess * 200);

    vec3 result = (ambient + diffuse + specular) * u_Albedo;

    v_Color = vec4(result, 1.0f);

};

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

in vec4 v_Color;

void main()
{

    color = v_Color;
};