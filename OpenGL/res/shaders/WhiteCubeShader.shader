#shader vertex
#version 330 core
        
layout(location = 0) in vec3 a_Position;

layout (std140) uniform Matrices
{
    mat4 u_Proj;
    mat4 u_View;
    mat4 u_DirectionalLightSpaceMatrix;
    mat4 u_PointLightSpaceMatrices[6];
    mat4 u_SpotLightSpaceMatrix;
};

uniform mat4 u_Model;

void main()
{
    gl_Position = u_Proj * u_View * u_Model * vec4(a_Position, 1.0);
};

#shader fragment
#version 330 core
        
layout (location = 0) out vec4 color;

void main()
{
    color = vec4(1.0, 1.0, 1.0, 1.0);
};