#include <stdio.h>
#include <time.h>
#include <stdlib.h> //this is where rand lives
//#include <stdbool.h>
//#include <math.h>

//#include "events.h"
//#include "graphics.h"
#include "objects.h"
#include "objectlist.h"

void init_game(GameProperties * game) {
	game->width=		512;
	game->height=		500;
	game->resolution=	16;		//how wide/tall are the cells
	game->background=	color(100,133,200);

	game->pen_jump=		-10.0;
	game->pen_move=		3.02;
	game->difficulty=	.5;
	game->FPS=			60.0;
	game->gravity=		0.35;
	game->box_fall=		2.0;

	game->bomb_time=	120;
	game->ghost_time=	500;
	game->powerup_time=	500;

	game->step=			0;

	game->normal_speed=	.7;
	game->slomo_speed=	.08;
	game->speed=		game->normal_speed;

	game->slomo_time=	50;
	game->slomo_timer=	0;

	game->objects=		objectList(50);
}

int main( int argc, char *args[]){
	GameProperties game;
	init_game(&game);
	srand(0);
	start_game(&game);
	obliterate_graphics();
}
