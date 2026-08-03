#shader vertex
#version 330 core
        
layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Normal;
layout(location = 2) in vec2 a_TexCoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

out VS_OUT {
    vec2 TexCoords;
} vs_out;

void main()
{
     gl_Position = u_Proj * u_View * u_Model * a_Position;
     vs_out.TexCoords = a_TexCoords;
}

#shader geometry
#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 TexCoords;
} gs_in[];

out GS_OUT{
    vec2 TexCoords;
} gs_out;

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
} 

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 1.0;
    vec3 direction = normal * 1 * magnitude; 
    return position + vec4(direction, 0.0);
} 

void main() { 
    
    vec3 normal = GetNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    gs_out.TexCoords = gs_in[0].TexCoords;
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position, normal);
    gs_out.TexCoords = gs_in[1].TexCoords;
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position, normal);
    gs_out.TexCoords = gs_in[2].TexCoords;
    EmitVertex();
    EndPrimitive();
}  


#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

in GS_OUT {
    vec2 TexCoords;
} gs_in;

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
    color = texture(u_Material.texture_diffuse1,  gs_in.TexCoords);
};