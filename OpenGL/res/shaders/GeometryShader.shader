#shader vertex
#version 330 core
        
layout (location = 0) in vec2 a_Pos;

void main()
{
    gl_Position = vec4(a_Pos.x, a_Pos.y, 0.0, 1.0); 
}

#shader geometry
#version 330 core

layout (points) in;
layout (line_strip, max_vertices = 2) out;

void main() {     
    gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0); 
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4( 0.1, 0.0, 0.0, 0.0);
    EmitVertex();
    
    EndPrimitive();
}  


#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

void main()
{
    color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
};