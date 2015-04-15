#include <stdio.h>
#include "objects.h"
#include "objectlist.h"
#include "bitoperations.h"

#include "string.h"


void init_game(GameProperties * game) {
	game->width=		512;
	game->height=		500;
	game->resolution=	16;		//how wide/tall are the cells
	game->background=	color(100,133,200);

	game->pen_num=		1;
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

	game->slomo_time=	60;
	game->slomo_timer=	0;

	game->movements=	movementList(800);

	game->objects=		objectList(50);
}

int main( int argc, char *args[]){
	GameProperties game;
	init_game(&game);
	if (argc>2){
		if (strcmp(args[1],"record")==0){
			game.recording=true;
		}
		else if (strcmp(args[1],"replay")==0){
			read_data(args[2], &game);
			game.replaying=true;
			game.recording=false;
			game.step=0;
		}
		strcpy(game.recordfile,args[2]);
	}
	printf("%i\n",rand());
	start_game(&game);
	obliterate_graphics();
}
