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

in vec2 v_TexCoords;

void main()
{
    color = texture(u_Texture, v_TexCoords);
};