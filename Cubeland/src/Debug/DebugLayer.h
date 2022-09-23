#pragma once

#include "Core/Layer.h"

#include "Rendering/Camera.h"
#include "Rendering/Buffer.h"
#include "Rendering/Shader.h"
#include "Rendering/StaticMesh.h"

namespace Cubeland
{
	class DebugLayer : public Layer
	{
	public:
		DebugLayer() = default;
		~DebugLayer() override = default;

		void OnAttach() override;
		void OnUpdate(float ts) override;
		void OnEvent(Event& e) override;

	private:
		Camera m_Camera;
		Ref<StaticMesh> m_CubeMesh;

		Ref<UniformBuffer> m_CameraUniformBuffer;
		Scope<Shader> m_Shader;
	};
}
