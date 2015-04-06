#ifndef FOTUS_GRAPHICS
#define FOTUS_GRAPHICS

#include <SDL2/SDL.h>

typedef struct {
    Uint8 red;
    Uint8 green;
    Uint8 blue;
    Uint8 alpha;
} Color;

typedef struct {
    SDL_Window *win;
    SDL_Renderer *renderer;
    Color background;
} Graphics;

typedef struct {
    int start;
    int lastTick;
    int frames;
    int fps;
    int tpf;
} Framerate;



void init_graphics(int x, int y, int width, int height, Color c);

void obliterate_graphics();
void render_graphics();

void fillRect(Color c, int x, int y, int width, int height); 
void outlineRect(Color c, int x, int y, int width, int height);
void drawLine(Color c, int x1, int y1, int x2, int y2);

void drawNum(Color c, int n, int x, int y, int w, int h);

void clear_graphics();

Color color(int r, int g, int b);
Color colora(int r, int g, int b, int a);
Color vary(Color c, int vary);
Color lighten(Color c, int vary);

int getKeyB();
int getKey();

Framerate initFramerate(int fps);
Framerate capFrame(Framerate f);
float avgFPS(Framerate f);

#endif
