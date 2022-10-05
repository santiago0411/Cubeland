#pragma once

#include "Rendering/Texture2D.h"

namespace Cubeland
{
	struct GameTextures
	{
		static Ref<Texture2D> BlocksAtlasTexture;
		static Ref<SubTexture2D> StoneSubTexture;
		static Ref<SubTexture2D> DirtSubTexture;

		static void InitTextures();
	};
};