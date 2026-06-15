#include "update.h"
#include "input.h"
#include "canvas.h"
#include "widgets.h"
#include "stdio.h"
#include "app.h"
#include "math.h"
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

extern int brushsize;

int dtod(int x, int y, Canvas c)	{return x+y*c->width;}
double getRadius(double x, double y)	{return sqrt(x*x+y*y);}

void poller(SDL_Event e, Application App)
{
	switch(e.type)
	{
		case SDL_EVENT_QUIT:
			App->running = false;
			break;
		case SDL_EVENT_KEY_DOWN:
			App->modmask = SDL_GetModState();
			keydowncontrols(e,App);
			break;
		case SDL_EVENT_KEY_UP:
			App->modmask = SDL_GetModState(); //this is for modifier keys, 
			//maybe do this in key down as well?
			break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			App->mousedown=true;
			break;		
		case SDL_EVENT_MOUSE_BUTTON_UP:
			App->mousedown=false;
			break;
		case SDL_EVENT_MOUSE_MOTION:
			SDL_GetMouseState(&App->Mouse.x,&App->Mouse.y);
			if(App->mousedown) //this is for drawing
			{
				//drawCircle(Mouse->x, Mouse->y, brushsize, c);
			}
			break;
	}
}

void handlefulls(SDL_Window *window,bool* fulls)
{
	if(!*fulls) SDL_SetWindowFullscreen(window,*fulls = true);
	else
	{
		SDL_HideWindow(window);
		SDL_SetWindowFullscreen(window,*fulls = false);
		SDL_MaximizeWindow(window);
		SDL_ShowWindow(window);
	}
}

void saveCurrentFrame(Canvas c)
{
	SDL_Surface* FrameScreenshot = SDL_CreateSurfaceFrom(c->width, c->height, SDL_PIXELFORMAT_RGBA8888, c->CanvasBuffer, c->pitch);
	IMG_SavePNG(FrameScreenshot, "image.png");
	SDL_DestroySurface(FrameScreenshot);
}

void updateWidgets(SDL_FPoint* Mouse, bool mousedown, Button* buttonarray, int buttoncount)
{
	for(int i = 0; i<buttoncount;i++)
	{
		updateButtonState(Mouse,buttonarray[i],mousedown);
	}
}

void checkWidgets(Button * buttonarray, int buttoncount, Canvas c)
{
	/*for(int i = 0; i<buttoncount;i++)
	{
		Button b = buttonarray[i];
		if(wasButtonClicked(b)) printf("Button %d was pressed\n",i+1);
		if(wasButtonReleased(b)) printf("Button %d was released\n",i+1);
	}*/
	//float step = (1.0f - HSVcolorValue/300.0f)*(1.0f - HSVcolorValue/300.0f)*0.08;
	double step = 10;	
	if(wasButtonClicked(buttonarray[0]) && c->alpha>step) 
	{
		c->alpha-=step;
		c->alphaChanged = true;
	}
	if(wasButtonClicked(buttonarray[1]) && c->alpha+step<255) 
	{
		c->alpha+=step;
		c->alphaChanged = true;
	}
}