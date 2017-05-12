#pragma once
#include <SDL.h>
#include "SceneManager.h"
#include <queue>
#include <functional>

class RPGSAEMMOApp
{
public:
	void App(const int& sizeX, const int& sizeY,
		const char* WindowTitle, const uint32_t parameters, int fps,
		Scene* const& mainScene);
	static void ChangeFrameLock(const int& fps = 60);
	static int GetFrameLock();
private:
	void Init();
	static int Draw(void *ptr);
	static void DrawStep();
	static SceneManager* sceneManager;
	SDL_Window *window = nullptr;
	SDL_Renderer *renderer = nullptr;
	SDL_Event event;
	int windowSizeX;
	int windowSizeY;
	uint32_t winParams;
	SDL_Thread *drawThread;
	static std::queue<std::function<void()>> drawQueue;
	static SDL_bool appIsClosing;
	static SDL_mutex* drawMutex;
	static int frameLock;
};

