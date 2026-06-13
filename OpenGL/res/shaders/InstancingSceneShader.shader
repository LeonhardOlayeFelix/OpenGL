#shader vertex
#version 330 core
        
layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec4 a_Offset;

out vec4 v_Color;

void main()
{
    gl_Position = a_Position + a_Offset;
    v_Color = a_Color;
};

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

in vec4 v_Color;

void main()
{
    color = v_Color;
};