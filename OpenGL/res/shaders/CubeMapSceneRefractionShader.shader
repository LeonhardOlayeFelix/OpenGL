#shader vertex
#version 330 core
        
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;

layout (std140) uniform Matrices
{
    mat4 u_Proj;
    mat4 u_View;
};

out vec3 v_Normal;
out vec3 v_WorldPosition;

uniform mat4 u_Model;

void main()
{
    v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
    v_WorldPosition = vec3(u_Model * vec4(a_Position, 1.0)); 
    gl_Position = u_Proj * u_View * vec4(v_WorldPosition, 1.0);
};

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

in vec3 v_Normal;
in vec3 v_WorldPosition;

uniform vec3 u_CameraPosition;
uniform samplerCube u_Skybox;


void main()
{
    float ratio = 1.00 / 1.52;
    vec3 dir = normalize(v_WorldPosition - u_CameraPosition);
    vec3 refracted = refract(dir, normalize(v_Normal), ratio);
    color = vec4(texture(u_Skybox, refracted).rgb, 1.0);
};