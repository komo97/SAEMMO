#pragma once
#include <SDL.h>
#include "SceneManager.h"
class RPGSAEMMOApp
{
public:
	void App(const int& sizeX, const int& sizeY,
		const char* WindowTitle, const uint32_t parameters,
		Scene* const& mainScene);
private:
	void Init();
	SceneManager* sceneManager = nullptr;
	SDL_Window *window = nullptr;
	SDL_Renderer *renderer = nullptr;
	SDL_Event event;
	int windowSizeX;
	int windowSizeY;
	uint32_t winParams;
};

