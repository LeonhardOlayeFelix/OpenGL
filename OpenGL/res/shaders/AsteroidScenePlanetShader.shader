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
     
struct Material 
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D emission;
    float shininess;
};

uniform Material u_Material;

layout(location = 0) out vec4 color;

in vec3 v_Normal;
in vec3 v_FragPosition;
in vec2 v_TexCoords;

void main()
{
    color = texture(u_Material.texture_diffuse1, v_TexCoords);
};