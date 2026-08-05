#shader vertex
#version 330 core
        
layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec2 a_TexCoords;

layout (std140) uniform Matrices
{
    mat4 u_Proj;
    mat4 u_View;
};

uniform mat4 u_Model;

out vec2 v_TexCoords;

void main()
{
    v_TexCoords = a_TexCoords;
    gl_Position = u_Proj * u_View * u_Model * a_Position;
};

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

in vec2 v_TexCoords;

uniform sampler2D u_Texture;

void main()
{
    color = texture(u_Texture, v_TexCoords);
    color = vec4(1.0, 1.0, 1.0, 1.0);
};