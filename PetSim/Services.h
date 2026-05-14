#pragma once
#include "NeedsSystem.h"
#include "AnimationManager.h"
#include "FontManager.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include "DialogManager.h"
#include "ItemManager.h"

struct Services
{
	NeedsSystem* needsSystem = nullptr;
	AnimationManager* animationManager = nullptr;
	FontManager* fontManager = nullptr;
	SoundManager* soundManager = nullptr;
	TextureManager* textureManager = nullptr;
	DialogManager* dialogManager = nullptr;
	ItemManager* itemManager = nullptr;
};