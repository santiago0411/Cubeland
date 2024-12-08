#pragma once

namespace Cubeland
{
	class ScriptableEntity;

	// This manager class is needed because Components.h cannot import ScriptableEntity.h since it includes Components.h through Entity.h,
	// so to avoid the circular import we need this manager class in which we can import ScriptableEntity.h in the cpp file to have access to the fully typed class
	class ScriptableEntityManager
	{
	public:
		template<typename T>
		static ScriptableEntity* CreateInstance()
		{
			static_assert(std::is_base_of_v<ScriptableEntity, T>, "Cannot CreateInstance() of a class that doesn't inherit from ScriptableEntity");
			return new T();
		}

		static void DestroyInstance(const ScriptableEntity* instance);
	};
}
