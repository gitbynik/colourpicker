#ifndef WIDGETS_H
#define WIDGETS_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "app.h"

//conaider changing all layout rects to centre or top left?? for uniformity

struct button
{
	SDL_FRect r;//topleft	
	bool currentState;
	bool prevState;
	bool clickedThisFrame;
	bool releasedThisFrame;
	bool mouseHovering; //maybe combine to do bitmasking
	/*
	improve button collison behaviour
	when mousedown and moves out of button, it no longer registers
	when mousedown and moves into button, it registers
	*/
};
typedef struct button* Button;

struct slider
{
	Button b;
	bool changed;
	int val; //0 to 100
	SDL_FRect r;//topleft
};
typedef struct slider* Slider;

struct label
{
	SDL_FPoint pos;//topleft
	TTF_Text* text;
};
typedef struct label* Label;

struct hollowptr
{
	Button b1;
	SDL_Texture* texture;
	bool changeColour;
};
typedef struct hollowptr* HollowPtr;

Button createButton(float x, float y, float w, float h); //x,y is center
void renderButton(Button, SDL_Renderer*);
void updateButtonState(SDL_FPoint* ,Button,bool mouseState);
bool isButtonCurrentlyPressed(Button);
bool wasButtonClicked(Button);
bool wasButtonReleased(Button);
void destroyButton(Button);
void renderSliderButton(Button,SDL_Renderer*);

Slider createSlider(float x, float y, float w, float h); //x,y is center
void renderSlider(Slider, SDL_Renderer*);
void updateSliderState(SDL_FPoint* ,Slider,bool mouseState);
void destroySlider(Slider);

Label createLabel(float x, float y, struct textsys TextSys, char* textlabel); //x,y is topleft
void renderLabel(Label);
void updateLabelText(Label l1, char* textlabel);
void destroyLabel(Label);

HollowPtr createHollowPtr(float x, float y, float r, SDL_Renderer*);
void destroyHollowPtr(HollowPtr);
void renderHollowPtr(HollowPtr, SDL_Renderer*); //first mouseclick inconsistent
void updateHollowPtr(SDL_FPoint* ,HollowPtr,bool mouseState);


#endif