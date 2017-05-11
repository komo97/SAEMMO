//Main
#include <SDL.h>
#include "RPGSAEMMOApp.h"
#include "MainScene.h"
#define INITIALPARAMS SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_INPUT_GRABBED

#ifdef __cplusplus
extern "C"
#endif 

int main(int argc, char *argv[])
{
	RPGSAEMMOApp app;
	app.App(800, 800, "TEST", INITIALPARAMS, new MainScene());
	return 0;
}

//!Main