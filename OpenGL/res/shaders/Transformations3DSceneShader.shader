#shader vertex
#version 330 core
        
layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;


void main()
{
    gl_Position = u_Proj * u_View * u_Model * a_Position;
    v_TexCoord = a_TexCoord;
};

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform sampler2D u_Texture2;

void main()
{
    color = mix(texture(u_Texture, vec2(1 - v_TexCoord.x, v_TexCoord.y)), texture(u_Texture2, v_TexCoord), 0.2f);
};