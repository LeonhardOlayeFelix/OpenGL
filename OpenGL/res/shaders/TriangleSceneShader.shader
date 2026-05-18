#shader vertex
#version 330 core
        
layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Color;

out vec4 v_VertexPos;

uniform float u_XOffset;
uniform float u_YOffset;
uniform float u_Opacity;

void main()
{
    v_VertexPos = vec4(a_Position.x + u_XOffset, a_Position.y + u_YOffset, a_Position.z, u_Opacity);
    gl_Position = vec4(a_Position.x + u_XOffset, a_Position.y + u_YOffset, a_Position.z, a_Position.w);
};

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

in vec4 v_VertexPos;

uniform float u_Opacity;

void main()
{
    color = v_VertexPos;
};