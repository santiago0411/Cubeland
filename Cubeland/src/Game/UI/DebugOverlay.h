#pragma once

#include "Game/Entity.h"

namespace Cubeland
{
	class DebugOverlay
	{
	public:
		DebugOverlay(Entity playerEntity)
			: m_PlayerEntity(playerEntity) {}

		void OnImGuiRender();

	private:
		Entity m_PlayerEntity;
	};
}
