#ifndef CANVAS_H
#define CANVAS_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>

struct canvas
{
	int width,height,pitch;
	Uint32 * CanvasBuffer;
	int alpha;
	bool canvasRequiresUpdate,saveRequested, alphaChanged;
	SDL_Texture* frameTexture, *bgTexture;
};
typedef struct canvas* Canvas;

Canvas CanvasInit(SDL_Window*, SDL_Renderer*);
void drawCircle(float centrex, float centrey,int radius,Canvas);
void drawSquare(int xcord, int ycord, int sidelength,Canvas);
void drawHSVCircle(Canvas c);
void createBGDisk(int r, Canvas c);
void destroyCanvas(Canvas);
void canvasGetRGB(float x, float y, int val, float centrex, float centrey,
 					float radius, double* R, double* G, double* B);


#endif