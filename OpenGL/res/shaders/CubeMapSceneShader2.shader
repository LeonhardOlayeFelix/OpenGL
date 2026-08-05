#shader vertex
#version 330 core
        
layout(location = 0) in vec4 a_Position;

layout (std140) uniform Matrices
{
    mat4 u_Proj;
    mat4 u_View;
};

out vec3 v_TexCoords;

void main()
{
    v_TexCoords = a_Position.xyz;
    gl_Position = u_Proj * u_View * a_Position;
};

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

in vec3 v_TexCoords;

uniform samplerCube u_Skybox;

void main()
{
    color = texture(u_Skybox, v_TexCoords);
    //color = vec4(1.0, 1.0, 1.0, 1.0);
};