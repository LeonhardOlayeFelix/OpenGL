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
    vec2 TexCoords;
    vec3 WorldPos;
} vs_out;

void main()
{
    vs_out.WorldPos = vec3(u_Model * a_Position);
    vs_out.TexCoords = a_TexCoords;
    gl_Position = u_Proj * u_View * u_Model * a_Position;
}

#shader geometry
#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 TexCoords;
    vec3 WorldPos;
} gs_in[];

out GS_OUT{
    vec2 TexCoords;
} gs_out;

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
    
    vec3 normal = GetNormal();

    for (int i = 0; i < 3; i++)
    {
        vec3 exploded = explode(gs_in[i].WorldPos, normal);
        gl_Position = u_Proj * u_View * vec4(exploded, 1.0);
        gs_out.TexCoords = gs_in[i].TexCoords;
        EmitVertex();
    }
    EndPrimitive();
}  


#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

in GS_OUT {
    vec2 TexCoords;
} fs_in;

struct Material 
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D emission;
    float shininess;
};

uniform Material u_Material;

void main()
{
    color = texture(u_Material.texture_diffuse1,  fs_in.TexCoords);
};