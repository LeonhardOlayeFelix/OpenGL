#shader vertex
#version 330 core
        
layout(location = 0) in vec4 a_Position;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

out vec2 v_TexCoord;

void main()
{
    gl_Position = u_Proj * u_View * u_Model * a_Position;
    v_TexCoord = a_TexCoord;
};

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

uniform sampler2D u_Texture;

in vec2 v_TexCoord;

void main()
{
    color = texture(u_Texture, v_TexCoord);
};