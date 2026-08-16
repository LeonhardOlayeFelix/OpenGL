#shader vertex
#version 330 core
        
layout(location = 0) in vec3 a_Position;

out vec3 v_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_Proj;
uniform mat4 u_View;

void main()
{
    v_TexCoord = a_Position;
    gl_Position = u_Proj * u_View * u_Model * vec4(a_Position, 1.0);
}

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

uniform samplerCube u_DepthTexture3D;

in vec3 v_TexCoord;

void main()
{
    float depth = texture(u_DepthTexture3D, v_TexCoord).r;
    color = vec4(vec3(depth), 1.0);
    //color = vec4(1.0, 1.0, 1.0, 1.0);
};