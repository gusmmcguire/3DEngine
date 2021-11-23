#version 430 core
in vec3 fs_color;
in vec2 fs_texcoord;
    
out vec4 outColor;

uniform vec3 tint;
uniform sampler2D textureSampler;

void main()
{
    //outColor = vec4(fs_color * tint, 1.0) * texture(textureSampler, fs_texcoord);
    outColor = texture(textureSampler, fs_texcoord);
}