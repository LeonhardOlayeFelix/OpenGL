#shader vertex
#version 330 core
        
layout(location = 0) in vec4 a_Position;

layout (std140) uniform Matrices
{
    mat4 u_Proj;
    mat4 u_View;
};

uniform mat4 u_Model;

void main()
{
    gl_Position = u_Proj * u_View * u_Model * a_Position;
};

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

void main()
{
    color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
};