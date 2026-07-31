#shader vertex
#version 330 core
        
layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec2 a_TexCoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

out vec2 v_TexCoords;

void main()
{
    v_TexCoords = a_TexCoords;
    gl_Position = u_Proj * u_View * u_Model * a_Position;
};

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

uniform sampler2D u_Texture;
uniform float u_Near;
uniform float u_Far;

in vec2 v_TexCoords;

void main()
{
    color = vec4(0.04, 0.28, 0.26, 1.0);
};