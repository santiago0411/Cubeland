#version 450 core

layout(location = 0) out vec4 o_Color;

layout(location = 0) in vec2 v_TextureCoords;
layout(location = 1) in flat uint v_TextureId;

layout(binding = 0) uniform sampler2D u_Textures[32];

void main()
{
	o_Color = texture(u_Textures[v_TextureId], v_TextureCoords);
}