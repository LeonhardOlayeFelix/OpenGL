#shader vertex
#version 330 core
        
layout (location = 0) in vec2 a_Pos;
layout (location = 1) in vec3 a_Color;

out VS_OUT {
    vec3 color;
} vs_out;

void main()
{
    gl_Position = vec4(a_Pos.x, a_Pos.y, 0.0, 1.0); 
    vs_out.color = a_Color;
}

#shader geometry
#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
    vec3 color;
} gs_in[];

out GS_OUT {
    vec3 color;
} gs_out;

void build_house(vec4 position)
{    
    gs_out.color = gs_in[0].color;
    gl_Position = position + vec4(-0.1, -0.1, 0.0, 0.0);    // 1:bottom-left
    EmitVertex();   
    gl_Position = position + vec4( 0.1, -0.1, 0.0, 0.0);    // 2:bottom-right
    EmitVertex();
    gl_Position = position + vec4(-0.1,  0.1, 0.0, 0.0);    // 3:top-left
    EmitVertex();
    gl_Position = position + vec4( 0.1,  0.1, 0.0, 0.0);    // 4:top-right
    EmitVertex();
    gl_Position = position + vec4( 0.0,  0.2, 0.0, 0.0);    // 5:top
    gs_out.color = vec3(1.0, 1.0, 1.0);
    EmitVertex();
    EndPrimitive();
}

void main() {     
    build_house(gl_in[0].gl_Position);
}  


#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

in GS_OUT {
    vec3 color;
} gs_in;

void main()
{
    color = vec4(gs_in.color, 1.0f);
};