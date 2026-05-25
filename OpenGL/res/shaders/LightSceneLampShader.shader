#shader vertex
#version 330 core
        
layout(location = 0) in vec4 a_Position;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
    gl_Position = u_Proj * u_View * u_Model * a_Position;
};

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

uniform vec3 u_LightColor;

void main()
{
    color = vec4(u_LightColor, 1.0);
};