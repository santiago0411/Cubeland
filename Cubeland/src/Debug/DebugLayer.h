#pragma once

#include "Core/Layer.h"

#include "Rendering/Buffer.h"
#include "Rendering/Shader.h"


namespace Cubeland
{
	class DebugLayer : public Layer
	{
	public:
		DebugLayer() = default;
		~DebugLayer() = default;

		void OnAttach() override;
		void OnUpdate(float ts) override;

	private:
		Ref<VertexArray> m_VertexArray;
		Scope<Shader> m_Shader;
	};
}
