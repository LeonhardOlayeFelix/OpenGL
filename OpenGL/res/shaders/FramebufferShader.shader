#shader vertex
#version 330 core
        
layout(location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
    TexCoords = aTexCoords;
}  

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float u_Offset;
uniform float u_Kernel[9];

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-u_Offset,  u_Offset), // top-left
        vec2( 0.0f,    u_Offset), // top-center
        vec2( u_Offset,  u_Offset), // top-right
        vec2(-u_Offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( u_Offset,  0.0f),   // center-right
        vec2(-u_Offset, -u_Offset), // bottom-left
        vec2( 0.0f,   -u_Offset), // bottom-center
        vec2( u_Offset, -u_Offset)  // bottom-right    
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));

    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * u_Kernel[i];

    color = vec4(col, 1.0);

};