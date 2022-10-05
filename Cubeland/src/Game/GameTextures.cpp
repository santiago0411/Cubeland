#include "pch.h"
#include "GameTextures.h"

namespace Cubeland
{
	Ref<Texture2D> GameTextures::BlocksAtlasTexture = nullptr;
	Ref<SubTexture2D> GameTextures::StoneSubTexture = nullptr;
	Ref<SubTexture2D> GameTextures::DirtSubTexture = nullptr;

	void GameTextures::InitTextures()
	{
		CL_LOG_DEBUG("Initializing atlas texture and subtextures.");
		BlocksAtlasTexture = Texture2D::Create("assets/textures/atlas.png");
		StoneSubTexture = SubTexture2D::CreateFromCoords(BlocksAtlasTexture, { 1, 0 }, { 16, 16 });
		DirtSubTexture = SubTexture2D::CreateFromCoords(BlocksAtlasTexture, { 2, 0 }, { 16, 16 });
	}
}
