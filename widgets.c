#include "widgets.h"
#include "render.h"
#include "stdlib.h"
#include "update.h"
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <corecrt_wconio.h>
#include <math.h>

Button createButton(float x, float y, float w, float h)
{
	Button b1 = (Button) malloc(sizeof(struct button));
	b1->r = (SDL_FRect){x-w/2,y-h/2,w,h};
	b1->currentState = false;
	b1->prevState = false;
	b1->clickedThisFrame = false;
	b1->releasedThisFrame = false;
	b1->mouseHovering = false;
	return b1;
}

void renderButton(Button b1, SDL_Renderer* renderer)
{
	if(b1->currentState) SDL_SetRenderDrawColor(renderer, 143, 143, 114, 255);
	else if(b1->mouseHovering) SDL_SetRenderDrawColor(renderer, 150, 150, 138, 255);
	else SDL_SetRenderDrawColor(renderer, 192, 193, 194, 255); //unpressed,normal state
	SDL_RenderFillRect(renderer,&b1->r);
}

void updateButtonState(SDL_FPoint* Mouse, Button b1, bool mouseState)
{
	b1->prevState = b1->currentState;
	b1->currentState = mouseState && SDL_PointInRectFloat(Mouse,&b1->r);
	b1->mouseHovering = !mouseState && SDL_PointInRectFloat(Mouse,&b1->r);
	b1->clickedThisFrame = !b1->prevState && b1->currentState;
	b1->releasedThisFrame = b1->prevState && !b1->currentState;
}

bool isButtonCurrentlyPressed(Button b1)
{
	return b1->currentState;
}

bool wasButtonClicked(Button b1)
{
	return b1->clickedThisFrame;
}
bool wasButtonReleased(Button b1)
{
	return b1->releasedThisFrame;
}

void destroyButton(Button b1)
{
	free(b1);
}



Slider createSlider(float x, float y, float w, float h)
{
	Slider s1 = (Slider) malloc(sizeof(struct slider));
	s1->r = (SDL_FRect) {x-w/2,y-h/2,w,h};
	s1->val = 51;
	s1->b = createButton(x,y,w/10,3*h);
	s1->changed = true;
	return s1;
}

void renderSlider(Slider s1, SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 192, 193, 194, 255); //unpressed,normal state
	SDL_RenderFillRect(renderer,&s1->r);
	renderSliderButton(s1->b, renderer);
}

void renderSliderButton(Button b1,SDL_Renderer* renderer)
{
	if(b1->currentState) SDL_SetRenderDrawColor(renderer, 142, 143, 144, 230);
	else if(b1->mouseHovering) SDL_SetRenderDrawColor(renderer, 152, 153, 154, 230);
	else SDL_SetRenderDrawColor(renderer, 192, 193, 194, 230); //unpressed,normal state
	SDL_RenderFillRect(renderer,&b1->r);
}

void updateSliderState(SDL_FPoint* mouse, Slider s1, bool mouseState)
{
	Button sliderButton = s1->b;
	updateButtonState(mouse, sliderButton, mouseState);
	float buttonwidth = sliderButton->r.w;
	float currx = mouse->x - s1->r.x;
	float sliderwidth = s1->r.w;
	if(isButtonCurrentlyPressed(sliderButton))
	{
		if(currx > buttonwidth/2 && currx < sliderwidth - buttonwidth/2.0)
		{
			sliderButton->r.x = mouse->x - buttonwidth/2.0;
			s1->val = 255 * (currx - buttonwidth/2.0) / (sliderwidth-buttonwidth);
			s1->changed = true;
		}
	}
}

void destroySlider(Slider s1)
{
	destroyButton(s1->b);
	free(s1);
}



Label createLabel(float x, float y, struct textsys TextSys, char* textlabel)
{
	Label l1 = (Label) malloc(sizeof(struct label));
	l1->pos = (SDL_FPoint) {x,y};
	l1->text = TTF_CreateText(TextSys.textengine,TextSys.font,textlabel, strlen(textlabel));
	TTF_SetTextColor(l1->text, 127, 127, 127, 255);
	return l1;
}

void renderLabel(Label l1)
{
	TTF_DrawRendererText(l1->text, l1->pos.x, l1->pos.y);
}

void updateLabelText(Label l1, char* textlabel)
{
	TTF_SetTextString(l1->text, textlabel, strlen(textlabel));
}

void destroyLabel(Label l1)
{
	TTF_DestroyText(l1->text);
	free(l1);
}



HollowPtr createHollowPtr(float x, float y, float r, SDL_Renderer* renderer)
{
	HollowPtr p1 = (HollowPtr) malloc(sizeof(struct hollowptr));
	p1->b1 = createButton(x, y, 2*r, 2*r);
	p1->texture = createHollowPtrTexture(r, renderer);
	p1->changeColour = false;
	return p1;
}

void destroyHollowPtr(HollowPtr p1)
{
	SDL_DestroyTexture(p1->texture);
	destroyButton(p1->b1);
	free(p1);
}

void renderHollowPtr(HollowPtr p1, SDL_Renderer * renderer)
{
	if(p1->changeColour)
	{
		if(p1->b1->currentState) 
		{
			SDL_SetTextureColorModFloat(p1->texture, 0.8438, 0.844, 0.845);
			SDL_RenderTexture(renderer, p1->texture, NULL, &p1->b1->r);
			SDL_SetTextureColorModFloat(p1->texture, 1.185, 1.1849, 1.1835);
		}
	}
	else SDL_RenderTexture(renderer, p1->texture, NULL, &p1->b1->r);
}

void updateHollowPtr(SDL_FPoint * mouse, HollowPtr p1, bool mouseState)
{
	if(getRadius(mouse->x-980, mouse->y-640)>=500) return;
	updateButtonState(mouse, p1->b1, mouseState);
	if(p1->b1->currentState /*|| p1->b1->mouseHovering*/) p1->changeColour = true;
	else p1->changeColour = false;
	if(isButtonCurrentlyPressed(p1->b1))
	{
		p1->b1->r.x = mouse->x - p1->b1->r.w/2.0;
		p1->b1->r.y = mouse->y - p1->b1->r.h/2.0;
	}
}