#version 450 core

layout(location = 0) out vec4 o_Color;

layout(location = 0) in vec2 v_TextureCoords;
layout(location = 1) in flat uint v_TextureId;

layout(binding = 0) uniform sampler2D u_Textures[32];
// I didn't find any way to have spriv reflect a single uniform int, so this is it xd.
layout(std140, binding = 1) uniform RenderOptions
{
	int RenderWireframe;
};

void main()
{
	if (RenderWireframe == 1)
	{
		o_Color = vec4(1.0, 0.0, 1.0, 1.0);
	}
	else
	{
		o_Color = texture(u_Textures[v_TextureId], v_TextureCoords);
	}
}