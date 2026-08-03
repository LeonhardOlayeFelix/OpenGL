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
} vs_out;

void main()
{
     gl_Position = u_Proj * u_View * u_Model * a_Position;
     vs_out.TexCoords = a_TexCoords;
}


#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

in VS_OUT {
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