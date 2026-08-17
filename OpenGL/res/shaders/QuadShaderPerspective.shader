#shader vertex
#version 330 core
        
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_Proj;


void main()
{
    gl_Position = u_Proj * u_Model * vec4(a_Position, 1.0);
    v_TexCoord = a_TexCoord;
}

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

uniform sampler2D u_DepthTexture;
uniform float u_NearPlane;
uniform float u_FarPlane;
uniform bool u_Linearize;

in vec2 v_TexCoord;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * u_NearPlane * u_FarPlane) / (u_FarPlane + u_NearPlane - z * (u_FarPlane - u_NearPlane));
}

void main()
{
    float depth = texture(u_DepthTexture, v_TexCoord).r;
    if (u_Linearize)
    {
        color = vec4(vec3(LinearizeDepth(depth) / u_FarPlane), 1.0);
    }
    else
    {
        color = vec4(vec3(depth), 1.0);
    }
    //color = vec4(1.0, 0.0, 0.0, 1.0);
};