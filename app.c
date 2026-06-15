#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>
#include <stdlib.h>
#include "app.h"
#include <stdio.h>
#include "canvas.h"

Application AppInit(int fps)
{
	Application App = (Application) malloc(sizeof(struct app));
	App->fps = fps;
	App->clockrate = 1000000000.0/(App->fps);
	App->modmask = 0;
	App->elapsedNanoSeconds = 0;
	App->last_render_time = 0;
	App->running = true;
	App->mousedown = false;
	App->fulls = false;
	App->freq = SDL_GetPerformanceFrequency();
	WindowAndRendererInit(App);
	TextInit(App);
	App->c = CanvasInit(App->window,App->renderer);
	App->Mouse.x = 0.0;
	App->Mouse.y = 0.0;
	App->TextSys.renderer = App->renderer;
	return App;
}

void WindowAndRendererInit(Application A1)
{
	A1->window = SDL_CreateWindow("gui", 255, 255, SDL_WINDOW_RESIZABLE
		|SDL_WINDOW_HIDDEN|SDL_WINDOW_HIGH_PIXEL_DENSITY);
	SDL_MaximizeWindow(A1->window);
	A1->renderer = SDL_CreateRenderer(A1->window,NULL);
	SDL_SetRenderDrawBlendMode(A1->renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderClear(A1->renderer);
	SDL_RenderPresent(A1->renderer);
	SDL_ShowWindow(A1->window);
	//FullScreenDisplayModeChange(window);
}

void TextInit(Application A1)
{
	A1->TextSys.font = TTF_OpenFont("ARIAL.ttf", 48);
	if (!A1->TextSys.font) 
	{
		printf("font not found");
		return;
	}
	A1->TextSys.textengine = TTF_CreateRendererTextEngine(A1->renderer);
}

void AppQuit(Application App)
{
	destroyCanvas(App->c);
	TTF_CloseFont(App->TextSys.font);
	TTF_DestroyRendererTextEngine(App->TextSys.textengine);
	SDL_DestroyRenderer(App->renderer);
	SDL_DestroyWindow(App->window);
	free(App);
}