#pragma once

#include "ScriptableEntity.h"

namespace Cubeland
{
	class PlayerComponent : public ScriptableEntity
	{
	protected:
		void OnCreate() override;
		void OnDestroy() override;
		void OnUpdate(float ts) override;

	private:
		void MoveAndRotateCamera(float ts);

	private:
		glm::vec2 m_LastMousePosition{0};
	};
}
