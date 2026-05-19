#shader vertex
#version 330 core
        
layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;

out vec2 v_TexCoord;
out vec4 v_Color;

void main()
{
    gl_Position = vec4(a_Position.x, a_Position.y, a_Position.z, 1.0f);
    v_TexCoord = a_TexCoord;
    v_Color = a_Color;
};

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;

uniform sampler2D u_Texture;
uniform sampler2D u_Texture2;

void main()
{
    color = mix(texture(u_Texture, vec2(1-v_TexCoord.x, v_TexCoord.y)), texture(u_Texture2, v_TexCoord), 0.2);
};