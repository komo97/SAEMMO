#include "RPGSAEMMOApp.h"

std::queue<std::function<void()>> RPGSAEMMOApp::drawQueue;
SDL_bool RPGSAEMMOApp::appIsClosing = SDL_FALSE;
SDL_mutex* RPGSAEMMOApp::drawMutex = nullptr;
SceneManager* RPGSAEMMOApp::sceneManager;
int RPGSAEMMOApp::frameLock = 11;

void RPGSAEMMOApp::App(const int & sizeX, const int & sizeY,
	const char * WindowTitle, const uint32_t parameters, int fps,
	Scene* const& mainScene)
{
	windowSizeX = sizeX;
	windowSizeY = sizeY;
	winParams = parameters;
	ChangeFrameLock(fps);
	Init();
	sceneManager = SceneManager::GetSceneManager();
	sceneManager->ChangeScene(mainScene);
	while (1)
	{
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
		{
			appIsClosing = SDL_TRUE;
			break;
		}
		SDL_LockMutex(drawMutex);
		drawQueue.push(&DrawStep);
		SDL_UnlockMutex(drawMutex);
		sceneManager->GetCurrentScene()->Update();
		SDL_Delay(frameLock);
	}
	delete sceneManager;
	int threadStatus;
	if(SDL_GetThreadID(drawThread) == 0)
		SDL_WaitThread(drawThread, &threadStatus);
	SDL_DestroyMutex(drawMutex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void RPGSAEMMOApp::ChangeFrameLock(const int & fps)
{
	frameLock = (1000 / fps);
}

int RPGSAEMMOApp::GetFrameLock()
{
	return (1000 / frameLock);
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

	drawThread = SDL_CreateThread((Draw), "Draw", (void*)NULL);
	drawMutex = SDL_CreateMutex();
	if(drawThread == NULL)
	{
		SDL_Log("Faled to initialize DrawThread: %s", SDL_GetError());
		exit(-3);
	}
	if(drawMutex == NULL)
	{
		SDL_Log("Faled to initialize DrawQueue: %s", SDL_GetError());
		exit(-3);
	}
	SDL_DetachThread(drawThread);
}

int RPGSAEMMOApp::Draw(void * ptr)
{
	while (!appIsClosing) {
		if (drawQueue.size() <= 0) {
			SDL_Delay(10);
			continue;
		}
		SDL_LockMutex(drawMutex);
		std::function<void()> f = drawQueue.front();
		drawQueue.pop();
		SDL_UnlockMutex(drawMutex);
		f();
	}
	return 0;
}

void RPGSAEMMOApp::DrawStep()
{
	if(sceneManager->GetCurrentScene() != nullptr)
		sceneManager->GetCurrentScene()->Draw();
}

