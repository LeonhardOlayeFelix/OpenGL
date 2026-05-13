#shader vertex
#version 330 core
        
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 color;

out vec2 v_TexCoord;
out vec4 v_Color;

uniform mat4 u_MVP;

void main()
{
    v_Color = color;
    v_TexCoord = texCoord;
    gl_Position = u_MVP * position ;
};


#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

uniform sampler2D u_Texture;

in vec2 v_TexCoord;
in vec4 v_Color;

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = v_Color * texColor;
};