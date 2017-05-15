#pragma once
#include <SDL.h>
#include "SceneManager.h"
#include <queue>
#include <functional>
#include <SDL_mixer.h>
#include <vector>
#include <SDL_ttf.h>

class RPGSAEMMOApp
{
public:
	void App(const int& sizeX, const int& sizeY,
		const char* WindowTitle, const uint32_t parameters, int fps,
		Scene* const& mainScene);
	static void ChangeFrameLock(const int& fps = 60);
	static int GetFrameLock();
	static int getWidth();
	static int getHeight();
	static SDL_Window* getWindow() { return window; }
	static SDL_Surface* getRenderSurface() { return renderSurface; }
	static SDL_Renderer* getRenderer() { return renderer;}
	static SDL_Texture* getRenderTexture() { return renderTexture; }
	static void PrintSurfacesToScreen(const std::vector<SDL_Surface*>& surfaces,
		const std::vector<SDL_Rect*>& srcRects, const std::vector<SDL_Rect*>& dstRects);
private:
	void Init();
	static int Draw(void *ptr);
	static void DrawStep();
	static SceneManager* sceneManager;
	static int windowSizeX;
	static int windowSizeY;
	static SDL_Window *window;
	static SDL_Renderer *renderer;
	static SDL_Texture *renderTexture;
	static SDL_Surface *renderSurface;
	SDL_Event event;
	uint32_t winParams;
	SDL_Thread *drawThread;
	static std::queue<std::function<void()>> drawQueue;
	static SDL_bool appIsClosing;
	static SDL_mutex* drawMutex;
	static int frameLock;
};

