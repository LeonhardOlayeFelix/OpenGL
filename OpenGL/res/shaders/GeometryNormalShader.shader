#shader vertex
#version 330 core
        
layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Normal;
layout(location = 2) in vec2 a_TexCoords;

layout (std140) uniform Matrices
{
    mat4 u_Proj;
    mat4 u_View;
};
uniform mat4 u_Model;

out VS_OUT {
    vec3 normal;
} vs_out;

void main()
{
    gl_Position = u_View * u_Model * a_Position;
    mat3 normalMatrix = mat3(transpose(inverse(u_View * u_Model)));
    vs_out.normal = normalize(normalMatrix * vec3(a_Normal));
}

#shader geometry
#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

layout (std140) uniform Matrices
{
    mat4 u_Proj;
    mat4 u_View;
};

uniform float u_Magnitude;

void GenerateLine(int index)
{
    gl_Position = u_Proj * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = u_Proj * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * u_Magnitude);
    EmitVertex();
    EndPrimitive();
}

void main() { 
    GenerateLine(0);
    GenerateLine(1);
    GenerateLine(2);
}  

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

void main()
{
    color = vec4(1.0, 1.0, 0.0, 1.0);
};