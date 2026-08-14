#shader vertex
#version 330 core
        
layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

void main()
{
    gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
    v_TexCoord = a_TexCoord;
}

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

uniform sampler2D u_DepthTexture;

in vec2 v_TexCoord;

void main()
{
    float depth = texture(u_DepthTexture, v_TexCoord).r;
    color = vec4(vec3(depth), 1.0);
    //color = vec4(1.0, 0.0, 0.0, 1.0);
};