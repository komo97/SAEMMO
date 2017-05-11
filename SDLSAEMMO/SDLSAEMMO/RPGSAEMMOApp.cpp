#include "RPGSAEMMOApp.h"


void RPGSAEMMOApp::App(const int & sizeX, const int & sizeY,
	const char * WindowTitle, const uint32_t parameters,
	Scene* const& mainScene)
{
	windowSizeX = sizeX;
	windowSizeY = sizeY;
	winParams = parameters;
	Init();
	sceneManager->ChangeScene(mainScene);
	while (!SDL_QuitRequested())
	{
		sceneManager->GetCurrentScene()->Draw();
		sceneManager->GetCurrentScene()->Update();
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void RPGSAEMMOApp::Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		exit(-1);
	}
	if (SDL_CreateWindowAndRenderer(windowSizeX, windowSizeY, winParams, &window, &renderer) == -1) 
	{
		SDL_Log("Faled to initialize Window: %s", SDL_GetError());
		exit(-2);
	}

	sceneManager = SceneManager::GetSceneManager();
}
