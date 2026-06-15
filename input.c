#include "input.h"
#include "update.h"
#include "canvas.h"
#include "app.h"
#include <stdio.h>

extern Uint32 bgcolor;

void keydowncontrols(SDL_Event event,Application App)
{
	if(event.key.key == SDLK_F11) handlefulls(App->window,&App->fulls);
	switch(event.key.key)
	{
		case SDLK_Q:
		{
			for(int i=0; i<App->c->width;i++)
				for (int j=0; j<App->c->height; j++)
					App->c->CanvasBuffer[dtod(i,j,App->c)] = bgcolor;
			App->c->canvasRequiresUpdate = true;
			break;
		}
		case SDLK_S:
			if(App->modmask & SDL_KMOD_LCTRL) App->c->saveRequested = true;
			break;
	}
}