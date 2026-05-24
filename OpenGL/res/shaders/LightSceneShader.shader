#shader vertex
#version 330 core
        
layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

out vec3 v_Normal;
out vec3 v_FragPosition;

void main()
{
    gl_Position = u_Proj * u_View * u_Model * a_Position;
    v_FragPosition = vec3(u_Model * a_Position);
    v_Normal = vec3(a_Normal);
};

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

uniform vec3  u_Albedo;
uniform vec3  m_LightIntensity;
uniform vec3  u_LightPosition;

in vec3 v_Normal;
in vec3 v_FragPosition;

void main()
{
    float k_a = 0.1;
    float k_d = 0.8;

    vec3 I_a = m_LightIntensity;
    vec3 I_p = m_LightIntensity;

    vec3 lightDir = (u_LightPosition - v_FragPosition);
    vec3 N = normalize(v_Normal);
    vec3 L = normalize(lightDir);

    vec3 ambient = k_a * I_a;
    vec3 diffuse = I_p * k_d * max(dot(N, L), 0.0);

    vec3 result = (ambient + diffuse) * u_Albedo;

    color = vec4(result, 1.0f);
};