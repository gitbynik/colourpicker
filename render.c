#include "render.h"
#include "math.h"
#include "widgets.h"
#include "stdio.h"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <stdlib.h>

extern int brushsize;
extern Uint32 paintcolor,bgcolor;

void polartocart(double r, double theta, double* x, double *y)
{
	*x = r * cos(theta);
	*y = r * sin(theta);
}

void HSVtoRGB(double H, double S, double V, double* R, double* G, double* B) 
{
	V = V*255;
	if (S == 0.0f) 
	{
		*R = *G = *B = V;
		return;
	}
	double h = H / 60.0f;
	int i = (int)floor(h);
	double f = h - i;
	double p = V * (1.0f - S);
	double q = V * (1.0f - S * f);
	double t = V * (1.0f - S * (1.0f - f));

	switch (i % 6) 
	{
		case 0: *R = V, *G = t, *B = p; break;
		case 1: *R = q, *G = V, *B = p; break;
		case 2: *R = p, *G = V, *B = t; break;
		case 3: *R = p, *G = q, *B = V; break;
		case 4: *R = t, *G = p, *B = V; break;
		case 5: *R = V, *G = p, *B = q; break;
	}
}

Uint32 RGBtoHex(double r, double g, double b)
{
	Uint32 color = (Uint8) r;
	color = color<<8;
	color = color + g;
	color = color<<8;
	color = color + b;
	color = color<<8;
	color = color+0xff;
	return color;
}

SDL_Texture* iniBGTexture(SDL_Renderer * renderer,Canvas c)
{
	SDL_Texture* bgtexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET, c->width, c->height);
	SDL_SetRenderTarget(renderer, bgtexture);
	SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
	SDL_FRect r1 = {0,0,c->width,c->height};
	SDL_RenderFillRect(renderer,&r1);
	c->canvasRequiresUpdate = true;
	SDL_SetRenderTarget(renderer, NULL);
	return bgtexture;
}

SDL_Texture* iniFrameTexture(SDL_Renderer* renderer,Canvas c)
{
	SDL_Texture* frametexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STREAMING, c->width, c->height);
	drawHSVCircle(c);
	SDL_UpdateTexture(frametexture, NULL, c->CanvasBuffer, c->pitch);
	return frametexture;
}

void FullScreenDisplayModeChange(SDL_Window * window)
{
	int* displaymodecount = NULL; //the number of resolutions available
	int* displaycount = NULL; //the number of gpus/displays i guess??;
	SDL_DisplayID * displayID = SDL_GetDisplays(displaycount);
	SDL_DisplayMode ** modeptr = SDL_GetFullscreenDisplayModes(
	displayID[0],displaymodecount); //use 0 for default graphics 1 for secondary gpu
	SDL_SetWindowFullscreenMode(window,modeptr[0]); //pass this to setfullscreen
}

void renderWidgets(SDL_Renderer* renderer,Button * buttonarray,int buttoncount)
{
	for(int i=0; i<buttoncount; i++)
	{
		renderButton(buttonarray[i],renderer);
	}
}

SDL_Texture* createHollowPtrTexture(float r, SDL_Renderer* renderer)
{
	Uint32 * pixels = (Uint32*) malloc(4*r*r*sizeof(Uint32));
	for(int i=0;i<2*r;i++)
	{
		for(int j=0;j<2*r;j++)
		{
			float rcalc = sqrt((i-r)*(i-r) + (j-r)*(j-r));
			if(rcalc <= r && rcalc >= r-6)
				{pixels[i+j*(int)(2*r)] = 0xc0c1c2ff;}
			else
				{pixels[i+j*(int)(2*r)] = 0x00000000;}
		}
	}
	SDL_Surface* surface = SDL_CreateSurfaceFrom(2*r, 2*r, SDL_PIXELFORMAT_RGBA8888,pixels, 2*r*sizeof(Uint32));
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_DestroySurface(surface);
	free(pixels);
	return texture;
}
