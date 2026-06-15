#ifndef APP_H
#define APP_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>
#include "canvas.h"

struct textsys
{
	SDL_Renderer* renderer;
	TTF_TextEngine* textengine;
	TTF_Font* font;
};

struct app
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	struct textsys TextSys;
	SDL_FPoint Mouse;
	Canvas c;
	int fps;
	bool running;
	bool mousedown;
	bool fulls;
	Uint64 modmask;
	Uint64 clockrate;
	Uint64 freq;
	Uint64 elapsedNanoSeconds;
	Uint64 last_render_time;
};
typedef struct app* Application;

Application AppInit(int);
void WindowAndRendererInit(Application);
void TextInit(Application);
void AppQuit(Application);

#endif