#include <SDL2/SDL.h>
#include "graphics.h"

static Graphics g;

void init_graphics(int x, int y, int width, int height, Color c){
    SDL_Init(SDL_INIT_VIDEO);
    g.win = SDL_CreateWindow("Cold Waters in C", x, y, width, height, (Uint32)0);
    g.renderer = SDL_CreateRenderer(g.win, -1, SDL_RENDERER_ACCELERATED);
    g.background = c;
	SDL_SetRenderDrawBlendMode(g.renderer,SDL_BLENDMODE_BLEND);
    clear_graphics();
    render_graphics();
}

void obliterate_graphics(){
    SDL_DestroyRenderer(g.renderer);
    SDL_DestroyWindow(g.win);
    SDL_Quit();
}

void fillRect(Color c, int x, int y, int width, int height){
    SDL_SetRenderDrawColor(g.renderer, c.red, c.green, c.blue, c.alpha);
    SDL_Rect r = {x, y, width, height};
    SDL_RenderFillRect(g.renderer, &r);
} 
void outlineRect(Color c, int x, int y, int width, int height){
    SDL_SetRenderDrawColor(g.renderer, c.red, c.green, c.blue, c.alpha);
    SDL_Rect r = {x, y, width, height};
    SDL_RenderDrawRect(g.renderer, &r);
} 

void drawNum(Color c, int n, int x, int y, int width, int height){
	SDL_SetRenderDrawColor(g.renderer, c.red, c.green, c.blue, c.alpha);
	int x2=x+width;
	int y2=y+height/2;
	int y3=y+height;
	char str[15];
	sprintf(str, "%d", n);
	for (int i=0; str[i]!='\0';i++) {
		switch (str[i]){
			case ('-'):
				SDL_RenderDrawLine(g.renderer, x,y2,x2,y2);
				break;
			case ('0'):
				SDL_RenderDrawLine(g.renderer, x,y,x2,y);
				SDL_RenderDrawLine(g.renderer, x,y,x,y3);
				SDL_RenderDrawLine(g.renderer, x2,y,x2,y3);
				SDL_RenderDrawLine(g.renderer, x,y3,x2,y3);
				break;
			case ('1'):
				SDL_RenderDrawLine(g.renderer, x2,y,x2,y3);
				SDL_RenderDrawLine(g.renderer, x2,y,x+width/2,y+height/4);
				break;
			case ('2'):
				SDL_RenderDrawLine(g.renderer, x,y,x2,y);
				SDL_RenderDrawLine(g.renderer, x2,y,x2,y2);
				SDL_RenderDrawLine(g.renderer, x2,y2,x,y2);
				SDL_RenderDrawLine(g.renderer, x,y2,x,y3);
				SDL_RenderDrawLine(g.renderer, x,y3,x2,y3);
				break;
			case ('3'):
				SDL_RenderDrawLine(g.renderer, x2,y,x2,y3);
				SDL_RenderDrawLine(g.renderer, x,y,x2,y);
				SDL_RenderDrawLine(g.renderer, x,y2,x2,y2);
				SDL_RenderDrawLine(g.renderer, x,y3,x2,y3);
				break;
			case ('4'):
				SDL_RenderDrawLine(g.renderer, x2,y,x2,y3);
				SDL_RenderDrawLine(g.renderer, x,y2,x2,y2);
				SDL_RenderDrawLine(g.renderer, x,y,x,y2);
				break;
			case ('5'):
				SDL_RenderDrawLine(g.renderer, x,y,x2,y);
				SDL_RenderDrawLine(g.renderer, x,y,x,y2);
				SDL_RenderDrawLine(g.renderer, x,y2,x2,y2);
				SDL_RenderDrawLine(g.renderer, x2,y2,x2,y3);
				SDL_RenderDrawLine(g.renderer, x2,y3,x,y3);
				break;
			case ('6'):
				//Horizontal bars
				SDL_RenderDrawLine(g.renderer, x,y,x2,y);
				SDL_RenderDrawLine(g.renderer, x,y2,x2,y2);
				SDL_RenderDrawLine(g.renderer, x,y3,x2,y3);

				//Full vertical
				SDL_RenderDrawLine(g.renderer, x,y,x,y3);
				//SDL_RenderDrawLine(g.renderer, x2,y,x2,y3);

				//Half vertical
				//SDL_RenderDrawLine(g.renderer, x,y,x,y2);
				//SDL_RenderDrawLine(g.renderer, x,y2,x,y3);
				//SDL_RenderDrawLine(g.renderer, x2,y,x2,y2);
				SDL_RenderDrawLine(g.renderer, x2,y2,x2,y3);
				break;
			case ('7'):
				//Horizontal bars
				SDL_RenderDrawLine(g.renderer, x,y,x2,y);
				//SDL_RenderDrawLine(g.renderer, x,y2,x2,y2);
				//SDL_RenderDrawLine(g.renderer, x,y3,x2,y3);

				//Full vertical
				//SDL_RenderDrawLine(g.renderer, x,y,x,y3);
				SDL_RenderDrawLine(g.renderer, x2,y,x2,y3);

				//Half vertical
				//SDL_RenderDrawLine(g.renderer, x,y,x,y2);
				//SDL_RenderDrawLine(g.renderer, x,y2,x,y3);
				//SDL_RenderDrawLine(g.renderer, x2,y,x2,y2);
				//SDL_RenderDrawLine(g.renderer, x2,y2,x2,y3);
				break;
			case ('8'):
				//Horizontal bars
				SDL_RenderDrawLine(g.renderer, x,y,x2,y);
				SDL_RenderDrawLine(g.renderer, x,y2,x2,y2);
				SDL_RenderDrawLine(g.renderer, x,y3,x2,y3);

				//Full vertical
				SDL_RenderDrawLine(g.renderer, x,y,x,y3);
				SDL_RenderDrawLine(g.renderer, x2,y,x2,y3);

				//Half vertical
				//SDL_RenderDrawLine(g.renderer, x,y,x,y2);
				//SDL_RenderDrawLine(g.renderer, x,y2,x,y3);
				//SDL_RenderDrawLine(g.renderer, x2,y,x2,y2);
				//SDL_RenderDrawLine(g.renderer, x2,y2,x2,y3);
				break;
			case ('9'):
				//Horizontal bars
				SDL_RenderDrawLine(g.renderer, x,y,x2,y);
				SDL_RenderDrawLine(g.renderer, x,y2,x2,y2);
				SDL_RenderDrawLine(g.renderer, x,y3,x2,y3);

				//Full vertical
				//SDL_RenderDrawLine(g.renderer, x,y,x,y3);
				SDL_RenderDrawLine(g.renderer, x2,y,x2,y3);

				//Half vertical
				SDL_RenderDrawLine(g.renderer, x,y,x,y2);
				//SDL_RenderDrawLine(g.renderer, x,y2,x,y3);
				//SDL_RenderDrawLine(g.renderer, x2,y,x2,y2);
				//SDL_RenderDrawLine(g.renderer, x2,y2,x2,y3);
				break;
		}

		x2+=width+4;
		x+=width+4;
	}
}

void clear_graphics(){
    SDL_SetRenderDrawColor(g.renderer, g.background.red, 
                           g.background.green, g.background.blue, g.background.alpha);
    SDL_RenderClear(g.renderer);
}

void render_graphics(){
    SDL_RenderPresent(g.renderer);
}

Color color(int r, int g, int b){
    return (Color) { (Uint8)r, (Uint8)g, (Uint8)b, (Uint8)255};
}
Color colora(int r, int g, int b, int a){
    return (Color) { (Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a};
}

int getKeyB(){
    SDL_Event e;

    while(1){
        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT){
                return -1;
            }

            if(e.type == SDL_KEYDOWN){
                return e.key.keysym.scancode;
            }
        }
    }
}

int getKey(){
    SDL_Event e;

    while(SDL_PollEvent(&e) != 0){
        if(e.type == SDL_QUIT){
            return -1;
        }

        if(e.type == SDL_KEYDOWN){
            return e.key.keysym.scancode;
        }
    }

    return 0;
}

Framerate initFramerate(int fps){
    Framerate f;
    f.start = SDL_GetTicks();
    f.lastTick = SDL_GetTicks();
    f.frames = 0;
    f.fps = fps;
    f.tpf = 100 / fps;
    return f;
}

Framerate capFrame(Framerate f){
    int ticksSince = SDL_GetTicks() - f.lastTick;
    if( ticksSince < f.tpf){
        SDL_Delay( f.tpf - ticksSince); 
    }
    f.frames++;
    f.lastTick = SDL_GetTicks();
    return f;
}

float avgFPS(Framerate f){
    return f.frames / (SDL_GetTicks() - f.start) / 1000.f;
}
