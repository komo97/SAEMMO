#include "RPGSAEMMOApp.h"

std::queue<std::function<void()>> RPGSAEMMOApp::drawQueue;
SDL_bool RPGSAEMMOApp::appIsClosing = SDL_FALSE;
SDL_mutex* RPGSAEMMOApp::drawMutex = nullptr;
SceneManager* RPGSAEMMOApp::sceneManager;
int RPGSAEMMOApp::frameLock = 11;
int RPGSAEMMOApp::windowSizeX = 0;
int RPGSAEMMOApp::windowSizeY = 0;
SDL_Renderer* RPGSAEMMOApp::renderer = nullptr;
SDL_Surface* RPGSAEMMOApp::renderSurface = nullptr;
SDL_Window* RPGSAEMMOApp::window = nullptr;
SDL_Texture* RPGSAEMMOApp::renderTexture = nullptr;

void RPGSAEMMOApp::App(const int & sizeX, const int & sizeY,
	const char * WindowTitle, const uint32_t parameters, int fps,
	Scene* const& mainScene)
{
	windowSizeX = sizeX;
	windowSizeY = sizeY;
	winParams = parameters;
	ChangeFrameLock(fps);
	Init();
	SDL_SetWindowTitle(window,WindowTitle);
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
	SDL_FreeSurface(renderSurface);
	SDL_DestroyTexture(renderTexture);
	int a;
	Uint16 b;
	int c;
	for(int i = Mix_QuerySpec(&a,&b,&c); i--;)
		Mix_CloseAudio();
	while (Mix_Init(0))
		Mix_Quit();
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

int RPGSAEMMOApp::getWidth()
{
	return windowSizeX;
}

int RPGSAEMMOApp::getHeight()
{
	return windowSizeY;
}

void RPGSAEMMOApp::PrintSurfacesToScreen(const std::vector<SDL_Surface*>& surfaces,
	const std::vector<SDL_Rect*>& srcRects, const std::vector<SDL_Rect*>& dstRects)
{
	for (int i = 0; i < surfaces.size();++i)
	{
		renderTexture = SDL_CreateTextureFromSurface(renderer, surfaces.at(i));
		SDL_RenderCopy(renderer, renderTexture,
			srcRects.at(i), dstRects.at(i));
		SDL_DestroyTexture(renderTexture);
	}
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
	renderSurface = SDL_GetWindowSurface(RPGSAEMMOApp::getWindow());
	renderTexture = SDL_CreateTextureFromSurface(renderer, renderSurface);
	int flags = MIX_INIT_FLAC |
		MIX_INIT_MOD |
		MIX_INIT_MP3 |
		MIX_INIT_OGG;

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
		exit(2);
	}
	if (Mix_Init(flags) & flags != flags)
	{
		SDL_Log("Failed to initialize audio library.");
		exit(-3);
	}
	if (TTF_Init() == -1)
	{
		SDL_Log("TTF failed to initialize.");
		exit(-3);
	}
	Mix_VolumeMusic(100);

	SDL_DetachThread(drawThread);
}

int RPGSAEMMOApp::Draw(void * ptr)
{
	while (!appIsClosing) {
		if (drawQueue.size() <= 0) {
			SDL_Delay(10);
			continue;
		}
		SDL_RenderClear(renderer);
		SDL_LockMutex(drawMutex);
		std::function<void()> f = drawQueue.front();
		drawQueue.pop();
		SDL_UnlockMutex(drawMutex);
		f();
		SDL_RenderPresent(renderer);
		
	}
	return 0;
}

void RPGSAEMMOApp::DrawStep()
{
	if(sceneManager->GetCurrentScene() != nullptr)
		sceneManager->GetCurrentScene()->Draw();
}

