#include "pch.h"
#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Rendering/OpenGLContext.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture2D.h"

namespace Cubeland
{
	struct CubeVertex
	{
		glm::vec3 Position;
		glm::vec2 TextureCoords;
		uint32_t TextureId;
	};

	struct RendererData
	{
		static constexpr uint32_t MAX_CUBES = 10000;
		static constexpr uint32_t MAX_VERTICES = MAX_CUBES * 36;
		static constexpr uint32_t MAX_INDICES = MAX_CUBES * 36;
		static constexpr uint32_t MAX_TEXTURE_SLOTS = 32;

		Ref<VertexArray> CubeVertexArray;
		Ref<VertexBuffer> CubeVertexBuffer;
		Ref<Shader> CubeShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t CubeIndexCount = 0;
		CubeVertex* CubeVertexBufferBase = nullptr;
		CubeVertex* CubeVertexBufferPtr = nullptr;

		Ref<Texture2D> TextureSlots[MAX_TEXTURE_SLOTS];
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		glm::vec4 CubeVertexPositions[36] = {

			// FRONT FACE
			{ -0.5f, -0.5f,   0.5f, 1.0f },		// LEFT DOWN FRONT		
			{  0.5f, -0.5f,   0.5f, 1.0f },		// RIGHT DOWN FRONT		
			{  0.5f,  0.5f,   0.5f, 1.0f },		// RIGHT UP FRONT		
			
			{  0.5f,  0.5f,   0.5f, 1.0f },		// RIGHT UP FRONT		
			{ -0.5f,  0.5f,   0.5f, 1.0f },		// LEFT UP FRONT		
			{ -0.5f, -0.5f,   0.5f, 1.0f },		// LEFT DOWN FRONT		
			
			// BACK FACE
			{ -0.5f, -0.5f,  -0.5f, 1.0f },		// LEFT DOWN BACK		
			{  0.5f, -0.5f,  -0.5f, 1.0f },		// RIGHT DOWN BACK		
			{  0.5f,  0.5f,  -0.5f, 1.0f },		// RIGHT UP BACK		
			
			{  0.5f,  0.5f,  -0.5f, 1.0f },		// RIGHT UP BACK		
			{ -0.5f,  0.5f,  -0.5f, 1.0f },		// LEFT UP BACK			
			{ -0.5f, -0.5f,  -0.5f, 1.0f },		// LEFT DOWN BACK		
								  
			// LEFT FACE		  
			{ -0.5f, -0.5f,  -0.5f, 1.0f },		// LEFT DOWN BACK		
			{ -0.5f, -0.5f,   0.5f, 1.0f },		// LEFT DOWN FRONT		
			{ -0.5f,  0.5f,   0.5f, 1.0f },		// LEFT UP FRONT		
								  
			{ -0.5f,  0.5f,   0.5f, 1.0f },		// LEFT UP FRONT		
			{ -0.5f,  0.5f,  -0.5f, 1.0f },		// LEFT UP BACK			
			{ -0.5f, -0.5f,  -0.5f, 1.0f },		// LEFT DOWN BACK		
								  
			// RIGHT FACE		  
			{  0.5f, -0.5f,   0.5f, 1.0f },		// RIGHT DOWN FRONT		
			{  0.5f, -0.5f,  -0.5f, 1.0f },		// RIGHT DOWN BACK		
			{  0.5f,  0.5f,  -0.5f, 1.0f },		// RIGHT UP BACK		
								  
			{  0.5f,  0.5f,  -0.5f, 1.0f },		// RIGHT UP BACK		
			{  0.5f,  0.5f,   0.5f, 1.0f },		// RIGHT UP FRONT		
			{  0.5f, -0.5f,   0.5f, 1.0f },		// RIGHT DOWN FRONT		
								  
			// UP FACE			  
			{ -0.5f,  0.5f,   0.5f, 1.0f },		// LEFT UP FRONT		
			{  0.5f,  0.5f,   0.5f, 1.0f },		// RIGHT UP FRONT		
			{  0.5f,  0.5f,  -0.5f, 1.0f },		// RIGHT UP BACK

			{  0.5f,  0.5f,  -0.5f, 1.0f },		// RIGHT UP BACK		
			{ -0.5f,  0.5f,  -0.5f, 1.0f },		// LEFT UP BACK			
			{ -0.5f,  0.5f,   0.5f, 1.0f },		// LEFT UP FRONT		
								  
			// DOWN FACE		  
			{ -0.5f, -0.5f,   0.5f, 1.0f },		// LEFT DOWN FRONT		
			{  0.5f, -0.5f,   0.5f, 1.0f },		// RIGHT DOWN FRONT		
			{  0.5f, -0.5f,  -0.5f, 1.0f },		// RIGHT DOWN BACK

			{  0.5f, -0.5f,  -0.5f, 1.0f },		// RIGHT DOWN BACK		
			{ -0.5f, -0.5f,  -0.5f, 1.0f },		// LEFT DOWN BACK		
			{ -0.5f, -0.5f,   0.5f, 1.0f },		// LEFT DOWN FRONT		
		};

		struct CameraData
		{
			glm::mat4 View;
			glm::mat4 Projection;
		};

		CameraData CameraBuffer{};

		Renderer::Statistics Stats;
	};

	static RendererData* g_Data = nullptr;

	void Renderer::Init()
	{
		CL_LOG_DEBUG("Initializing Renderer.");
		g_Data = new RendererData;

		g_Data->CubeVertexArray = VertexArray::Create();

		g_Data->CubeVertexBuffer = VertexBuffer::Create(RendererData::MAX_VERTICES * sizeof(CubeVertex));
		g_Data->CubeVertexBuffer->SetLayout({
			{ "a_Position",			ShaderDataType::Float3 },
			{ "a_TextureCoords",	ShaderDataType::Float2 },
			{ "a_TextureId",		ShaderDataType::Int	   },
		});
		g_Data->CubeVertexBuffer->Reflect();
		g_Data->CubeVertexArray->SetVertexBuffer(g_Data->CubeVertexBuffer);
		g_Data->CubeVertexBufferBase = new CubeVertex[RendererData::MAX_VERTICES];

		auto* cubeIndices = new uint32_t[RendererData::MAX_INDICES];
		for (uint32_t i = 0; i < RendererData::MAX_INDICES; i += 36)
		{
			// FRONT FACE
			cubeIndices[i +  0] = i + 0;
			cubeIndices[i +  1] = i + 1;
			cubeIndices[i +  2] = i + 2;
										 
			cubeIndices[i +  3] = i + 3;
			cubeIndices[i +  4] = i + 4;
			cubeIndices[i +  5] = i + 5;
										 
			// BACK FACE				 
			cubeIndices[i +  6] = i + 6;
			cubeIndices[i +  7] = i + 7;
			cubeIndices[i +  8] = i + 8;
										 
			cubeIndices[i +  9] = i + 9;
			cubeIndices[i + 10] = i + 10;
			cubeIndices[i + 11] = i + 11;
										 
			// LEFT FACE				 
			cubeIndices[i + 12] = i + 12;
			cubeIndices[i + 13] = i + 13;
			cubeIndices[i + 14] = i + 14;
										 
			cubeIndices[i + 15] = i + 15;
			cubeIndices[i + 16] = i + 16;
			cubeIndices[i + 17] = i + 17;
										 
			// RIGHT FACE				 
			cubeIndices[i + 18] = i + 18;
			cubeIndices[i + 19] = i + 19;
			cubeIndices[i + 20] = i + 20;
										 
			cubeIndices[i + 21] = i + 21;
			cubeIndices[i + 22] = i + 22;
			cubeIndices[i + 23] = i + 23;
										 
			// UP FACE					 
			cubeIndices[i + 24] = i + 24;
			cubeIndices[i + 25] = i + 25;
			cubeIndices[i + 26] = i + 26;
										 
			cubeIndices[i + 27] = i + 27;
			cubeIndices[i + 28] = i + 28;
			cubeIndices[i + 29] = i + 29;
										 
			// DOWN FACE				 
			cubeIndices[i + 30] = i + 30;
			cubeIndices[i + 31] = i + 31;
			cubeIndices[i + 32] = i + 32;
										 
			cubeIndices[i + 33] = i + 33;
			cubeIndices[i + 34] = i + 34;
			cubeIndices[i + 35] = i + 35;
		}

		const auto cubeIBO = IndexBuffer::Create(cubeIndices, RendererData::MAX_INDICES);
		g_Data->CubeVertexArray->SetIndexBuffer(cubeIBO);
		delete[] cubeIndices;

		g_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		g_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		g_Data->TextureSlots[0] = g_Data->WhiteTexture;

		g_Data->CubeShader = CreateRef<Shader>("CubeShader", ShaderFilesMap {
			{ ShaderType::Vertex,	"assets/shaders/CubeShader_Vert.glsl"	},
			{ ShaderType::Fragment, "assets/shaders/CubeShader_Frag.glsl"	}
		});

		CL_LOG_DEBUG("Renderer start up successful.");
	}

	void Renderer::Shutdown()
	{
		CL_LOG_DEBUG("Shutting down Renderer.");
		delete[] g_Data->CubeVertexBufferBase;
		delete g_Data;
	}

	void Renderer::BeginScene(const Camera& camera, const glm::mat4& viewMatrix)
	{
		OpenGLContext::Clear();

		g_Data->CameraBuffer.View = viewMatrix;
		g_Data->CameraBuffer.Projection = camera.GetProjection();
		g_Data->CubeShader->GetUniformBuffer("CameraData")->SetData(&g_Data->CameraBuffer);

		StartBatch();
	}

	void Renderer::EndScene()
	{
		Flush();
	}

	void Renderer::Flush()
	{
		if (g_Data->CubeIndexCount > 0)
		{
			const auto dataSize = (uint32_t)((uint8_t*)g_Data->CubeVertexBufferPtr - (uint8_t*)g_Data->CubeVertexBufferBase);
			g_Data->CubeVertexBuffer->SetData(g_Data->CubeVertexBufferBase, dataSize);

			for (uint32_t i = 0; i < g_Data->TextureSlotIndex; i++)
				g_Data->TextureSlots[i]->Bind(i);

			g_Data->CubeShader->Bind();
			OpenGLContext::DrawIndexed(g_Data->CubeVertexArray, g_Data->CubeIndexCount);
			g_Data->Stats.DrawCalls++;
		}
	}

	void Renderer::DrawCube(const glm::vec3& pos, const Ref<SubTexture2D>& subTexture)
	{
		if (g_Data->CubeIndexCount >= RendererData::MAX_INDICES)
			NextBatch();

		const uint32_t textureIndex = FindTextureIndex(subTexture->GetTexture());
		const auto texCoords = subTexture->GetTexCoords();

		static uint32_t texIndices[]{ 0, 1, 2, 2, 3, 0 };

		for (uint32_t i = 0; i < std::size(g_Data->CubeVertexPositions); i++)
		{
			// TODO top texture - if (someMappingArray[i] == CubeMesh::TopFace) use topTexCoords instead

			g_Data->CubeVertexBufferPtr->Position = glm::translate(glm::mat4(1.0f), pos) * g_Data->CubeVertexPositions[i];
			g_Data->CubeVertexBufferPtr->TextureCoords = texCoords[texIndices[i % 6]];
			g_Data->CubeVertexBufferPtr->TextureId = textureIndex;
			g_Data->CubeVertexBufferPtr++;
		}

		g_Data->CubeIndexCount += 36;
		g_Data->Stats.CubeCount++;
	}

	void Renderer::StartBatch()
	{
		g_Data->CubeIndexCount = 0;
		g_Data->CubeVertexBufferPtr = g_Data->CubeVertexBufferBase;
		g_Data->TextureSlotIndex = 1;
	}

	void Renderer::NextBatch()
	{
		Flush();
		StartBatch();
	}

	uint32_t Renderer::FindTextureIndex(const Ref<Texture2D>& texture)
	{
		uint32_t textureIndex = 0;

		for (uint32_t i = 0; i < g_Data->TextureSlotIndex; i++)
		{
			if (*g_Data->TextureSlots[i] == *texture)
			{
				textureIndex = i;
				break;
			}
		}

		if (textureIndex == 0)
		{
			if (g_Data->TextureSlotIndex >= RendererData::MAX_TEXTURE_SLOTS)
				NextBatch();

			textureIndex = g_Data->TextureSlotIndex;
			g_Data->TextureSlots[g_Data->TextureSlotIndex] = texture;
			g_Data->TextureSlotIndex++;
		}

		return textureIndex;
	}

	Renderer::Statistics Renderer::GetStats()
	{
		return g_Data->Stats;
	}

	void Renderer::ResetStats()
	{
		memset(&g_Data->Stats, 0, sizeof(Statistics));
	}
}
