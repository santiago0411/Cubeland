#pragma once

#include "Rendering/Texture2D.h"

namespace Cubeland
{
	struct GameTextures
	{
		static inline Ref<Texture2D> BlocksAtlasTexture = nullptr;
		static inline Ref<SubTexture2D> StoneSubTexture = nullptr;
		static inline Ref<SubTexture2D> DirtSubTexture = nullptr;

		static void InitTextures();
	};
};