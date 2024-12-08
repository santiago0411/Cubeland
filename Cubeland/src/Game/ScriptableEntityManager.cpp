#include "pch.h"
#include "ScriptableEntityManager.h"

#include "ScriptableEntity.h"

namespace Cubeland
{
	void ScriptableEntityManager::DestroyInstance(const ScriptableEntity* instance)
	{
		delete instance;
	}
}
