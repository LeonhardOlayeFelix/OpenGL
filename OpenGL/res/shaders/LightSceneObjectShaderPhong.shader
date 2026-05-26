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

uniform vec3  u_Albedo;

uniform vec3  u_LightIntensity;
uniform vec3  u_LightPosition;

uniform vec3 u_ViewPosition;

uniform float u_Ka;
uniform float u_Kd;
uniform float u_Ks;
uniform float u_Kc;
uniform float u_Kl;
uniform float u_Kq;
uniform float u_Shininess;

in vec3 v_Normal;
in vec3 v_FragPosition;

void main()
{

    vec3 I_a = u_LightIntensity;
    vec3 I_p = u_LightIntensity;
    vec3 I_s = u_LightIntensity;

    vec3 lightDir = (u_LightPosition - v_FragPosition);
    vec3 N = normalize(v_Normal);
    vec3 L = normalize(lightDir);

    vec3 viewDir = normalize(u_ViewPosition - v_FragPosition);
    vec3 reflectDir = reflect(-L, N);

    vec3 ambient = u_Ka * I_a;
    vec3 diffuse = u_Kd * I_p * max(dot(N, L), 0.0);
    vec3 specular = u_Ks * I_s * pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess * 200);

    float distance = length(u_LightPosition - v_FragPosition);
    float attenuation = 1.0 / (u_Kc + u_Kl * distance + u_Kq * distance * distance);

    vec3 result = (ambient + attenuation * (diffuse + specular)) * u_Albedo;

    color = vec4(result, 1.0f);
};