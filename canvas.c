#include "canvas.h"
#include "render.h"
#include "stdlib.h"
#include "math.h"
#include "update.h"
#include "stdio.h"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>

extern Uint64 paintcolor;

Canvas CanvasInit(SDL_Window* window,SDL_Renderer * renderer)
{
	Canvas c = malloc(sizeof(struct canvas));
	SDL_GetWindowSizeInPixels(window, &c->width, &c->height);
	//printf("%d,%d",c->width,c->height);
	c->CanvasBuffer = (Uint32*) calloc(c->width*c->height,sizeof(Uint32));
	c->pitch = c->width*sizeof(Uint32);
	c->frameTexture = iniFrameTexture(renderer,c);
	SDL_SetTextureBlendMode(c->frameTexture, SDL_BLENDMODE_BLEND);
	c->bgTexture = iniBGTexture(renderer,c);
	createBGDisk(499,c);
	c->canvasRequiresUpdate = false;
	c->saveRequested = false;
	c->alphaChanged = false;
	c->alpha = 255;
	return c;
}

void destroyCanvas(Canvas c)
{
	SDL_DestroyTexture(c->bgTexture);
	SDL_DestroyTexture(c->frameTexture);
	free(c->CanvasBuffer);
	free(c);
}

void drawCircle(float x, float y, int r, Canvas c)
{
	for(int i=x-r;i<x+r;i++)
	{
		if(i<0||i>=c->width) continue;
		for(int j=y-r;j<y+r;j++)
		{
			if(j<0||j>=c->height) continue;
			if(sqrt((i-x)*(i-x) + (j-y)*(j-y)) <=r) c->CanvasBuffer[dtod(i,j,c)] = paintcolor;
		}
	}
	c->canvasRequiresUpdate = true;
}

void drawSquare(int x, int y, int r, Canvas c)
{
	for(int i=x-r;i<x+r;i++)
	{
		if(i<0||i>=c->width) continue;
		for(int j=y-r;j<y+r;j++)
		{
			if(j<0||j>=c->height) continue;
			c->CanvasBuffer[dtod(i,j,c)] = paintcolor;
		}
	}
	c->canvasRequiresUpdate = true;
}

void drawHSVCircle(Canvas c)
{
	int Radius = 500;
	for(double radius = 0.1; radius < Radius; radius++)
	{
		double radnormal = radius/Radius;
		double pixeldensity = 22;
		for(double theta = 0; theta<2*M_PI; theta=theta+M_PI/(180.0*radnormal*pixeldensity)) //22 is sufficiently large so that enough theta for r=500
																				   //21 suffices but heh pi=22/7
		{
			double i,j,r,g,b;
			polartocart(radius,theta,&i,&j);
			HSVtoRGB(180*theta/M_PI,radius/Radius,1.0,&r,&g,&b);
			c->CanvasBuffer[dtod(i+c->width/2.0-300,j+c->height/2.0,c)]=RGBtoHex(r,g,b);
		}
	}
}

void createBGDisk(int r, Canvas c)
{
	Uint32 * pixels = (Uint32*) malloc(4*r*r*sizeof(Uint32));
	for(int i=0;i<2*r;i++)
	{
		for(int j=0;j<2*r;j++)
		{
			if(sqrt( (i-r)*(i-r) + (j-r)*(j-r)) <= r ) 
				{pixels[i+2*j*r] = 0x000000ff;}
			else
				{pixels[i+2*j*r] = 0x191919ff;}
		}
	}
	SDL_Rect re = (SDL_Rect){c->width/2.0-300-r,c->height/2.0-r,2*r,2*r};
	SDL_UpdateTexture(c->bgTexture, &re, pixels, 2*r*sizeof(Uint32));
	free(pixels);
}

void canvasGetRGB(float mousex, float mousey, int val, float centrex, float centrey,
 				float radius, double *R, double *G, double *B)
{
	float Value = val/255.0;
	float rcord,thetacord; //thetacord in rad
	rcord = getRadius(mousex-centrex, mousey-centrey);
	thetacord = SDL_atan2(centrey-mousey, centrex-mousex) +M_PI;
	HSVtoRGB(180*thetacord/M_PI, rcord/radius, Value, R,G,B);
}