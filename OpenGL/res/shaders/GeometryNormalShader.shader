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
    vec3 WorldPos;
    vec3 normal;
} vs_out;

void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(u_Model)));

    vs_out.WorldPos = vec3(u_Model * a_Position);
    gl_Position = u_Proj * u_View * u_Model * a_Position;
    vs_out.normal = normalize(normalMatrix * vec3(a_Normal));
}
#shader geometry
#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 WorldPos;
    vec3 normal;
} gs_in[];

uniform float u_Magnitude;

layout (std140) uniform Matrices
{
    mat4 u_Proj;
    mat4 u_View;
};

vec3 GetNormal()
{
   vec3 a = gs_in[0].WorldPos - gs_in[1].WorldPos;
   vec3 b = gs_in[2].WorldPos - gs_in[1].WorldPos;
   return normalize(cross(a, b));
} 

vec3 explode(vec3 worldPos, vec3 normal)
{
    return worldPos + normal * u_Magnitude;
} 

void main() { 
    vec3 faceNormal = GetNormal();

    for (int i = 0; i < 3; i++)
    {
        vec3 exploded = explode(gs_in[i].WorldPos, faceNormal);

        gl_Position = u_Proj * u_View * vec4(exploded, 1.0);
        EmitVertex();

        gl_Position = u_Proj * u_View * vec4(exploded - 0.1 * gs_in[i].normal, 1.0);
        EmitVertex();

        EndPrimitive();
    }
}

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

void main()
{
    color = vec4(1.0, 1.0, 0.0, 1.0);
};