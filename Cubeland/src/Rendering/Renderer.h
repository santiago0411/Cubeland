#pragma once

#include "Rendering/Camera.h"
#include "Rendering/Buffer.h"
#include "Rendering/StaticMesh.h"
#include "Rendering/Texture2D.h"

namespace Cubeland
{
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera);
		static void EndScene();
		static void Flush();

		static void DrawCube(const glm::vec3& pos, const Ref<SubTexture2D>& subTexture);

		static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0);
		static void DrawStaticMesh(const Ref<StaticMesh>& staticMesh);

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t CubeCount = 0;

			uint32_t GetTotalVertexCount() const { return CubeCount * 8; }
			uint32_t GetTotalIndexCount() const { return CubeCount * 36; }
		};

		static Statistics GetStats();
		static void ResetStats();

	private:
		static uint32_t FindTextureIndex(const Ref<Texture2D>& texture);

		static void StartBatch();
		static void NextBatch();
	};
}
