#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TextureCoords;
layout(location = 2) in uint a_TextureId;

layout(std140, binding = 0) uniform CameraData
{
	mat4 u_View;
	mat4 u_Projection;
};

layout (location = 0) out vec2 v_TextureCoords;
layout (location = 1) out flat uint v_TextureId;

void main()
{
	v_TextureCoords = a_TextureCoords;
	v_TextureId = a_TextureId;
	gl_Position = u_Projection * u_View * vec4(a_Position, 1.0f);
}