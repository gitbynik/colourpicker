#ifndef RENDER_H
#define RENDER_H

#include <SDL3/SDL.h>
#include "canvas.h"
#include "widgets.h"

SDL_Texture* iniFrameTexture(SDL_Renderer*,Canvas);
SDL_Texture* iniBGTexture(SDL_Renderer*,Canvas);
void FullScreenDisplayModeChange(SDL_Window*);
void polartocart(double radius, double theta, double* xptr, double *yptr);
void HSVtoRGB(double Hue, double Saturation, double Value, double* Redptr, double* Greenptr, double* Blueptr);
Uint32 RGBtoHex(double Red, double Green, double Blue); //change to uint8
void renderWidgets(SDL_Renderer*,Button * ,int);
SDL_Texture* createHollowPtrTexture(float r,SDL_Renderer* renderer);

#endif