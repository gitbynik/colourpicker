#include <SDL3/SDL.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>
#include "app.h"
#include "update.h"
#include "canvas.h"
#include "stdio.h"
#include "widgets.h"

Uint64 paintcolor = 0x2d4061ff;
Uint64 bgcolor = 0x191919ff;
int brushsize = 10;
int framecount = 0;
int ButtonCount = 2;

int main(int argc, char *argv[])
{
	SDL_Event e;
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS);
	TTF_Init();
	Application App = AppInit(60);
	Uint64 current_render_time = 0;
	double ms = 0;
	char frametime[20];

	Label fpscounterlabel = createLabel(100, 100, App->TextSys, "0");
	Slider s1 = createSlider(App->c->width/2.0+300, App->c->height/2.0+400, 400, 30);
	Label SliderLabel = createLabel(App->c->width/2.0+550, App->c->height/2.0+375, App->TextSys, "0");
	char sliderText[20];
	double refr,refg,refb;
	SDL_FRect refrect = {1800,350,400,400};
	Label ColorLabel = createLabel(App->c->width/2.0+525, App->c->height/2.0+150, App->TextSys, "0");
	char colortext[20];
	HollowPtr p1 = createHollowPtr(600, 800, 25, App->renderer);

	App->last_render_time = SDL_GetPerformanceCounter();

	while(App->running)
	{
		while(SDL_PollEvent(&e)) poller(e, App);
		if(App->c->alpha != s1->val)
		{
			App->c->alpha = s1->val;
			SDL_SetTextureAlphaMod(App->c->frameTexture, App->c->alpha);
			//SDL_SetTextureAlphaMod(p1->texture, App->c->alpha);
		}
		if(App->c->canvasRequiresUpdate) 
		{
			SDL_UpdateTexture(App->c->frameTexture, NULL, App->c->CanvasBuffer, App->c->pitch);
			App->c->canvasRequiresUpdate = false;
		}
		if(App->c->saveRequested) 
		{
			saveCurrentFrame(App->c);
			App->c->saveRequested = false;
		}

		ms = App->elapsedNanoSeconds/1000000.0;
		snprintf(frametime, sizeof(frametime), "%0.4lf",ms);
		updateLabelText(fpscounterlabel, frametime);
		updateSliderState(&App->Mouse, s1, App->mousedown);
		snprintf(sliderText, sizeof(sliderText), "%d",s1->val);
		updateHollowPtr(&App->Mouse, p1, App->mousedown);
		canvasGetRGB(p1->b1->r.x+p1->b1->r.w/2.0,p1->b1->r.y+p1->b1->r.h/2.0,App->c->alpha,
			App->c->width/2.0-300, App->c->height/2.0,499,&refr,&refg,&refb);
		snprintf(colortext, sizeof(colortext), "0x%02x%02x%02x",(Uint8)refr,(Uint8)refg,(Uint8)refb);

		if(s1->changed)	
		{
			updateLabelText(SliderLabel, sliderText);
			s1->changed = false;
		}
		current_render_time = SDL_GetPerformanceCounter();
		App->elapsedNanoSeconds = (current_render_time - App->last_render_time) * 1000000000ULL / App->freq;
		updateLabelText(ColorLabel,colortext);

		if(App->elapsedNanoSeconds>=App->clockrate)
		{
			SDL_RenderClear(App->renderer);
			SDL_RenderTexture(App->renderer,App->c->bgTexture,NULL,NULL);
			SDL_RenderTexture(App->renderer,App->c->frameTexture,NULL,NULL);
			renderLabel(fpscounterlabel);
			renderSlider(s1, App->renderer);
			renderLabel(SliderLabel);
			renderHollowPtr(p1, App->renderer);
			SDL_SetRenderDrawColor(App->renderer, refr, refg, refb, 255);
			SDL_RenderFillRect(App->renderer,&refrect);
			renderLabel(ColorLabel);
			SDL_RenderPresent(App->renderer);
			App->last_render_time = current_render_time;
		}
	}
	destroyHollowPtr(p1);
	destroyLabel(SliderLabel);
	destroySlider(s1);
	destroyLabel(fpscounterlabel);
	AppQuit(App);
	TTF_Quit();
	SDL_Quit();
	return 0;
}