#shader vertex
#version 330 core
        
layout(location = 0) in vec4 a_Position;

layout (std140) uniform Matrices
{
    mat4 u_Proj;
    mat4 u_View;
};

uniform mat4 u_ViewNoTranslation;

out vec3 v_TexCoords;

void main()
{
    v_TexCoords = a_Position.xyz;
    vec4 pos = u_Proj * u_ViewNoTranslation * a_Position;
    gl_Position = pos.xyww;
};

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

in vec3 v_TexCoords;

uniform samplerCube u_Skybox;

void main()
{
    color = texture(u_Skybox, v_TexCoords);
};