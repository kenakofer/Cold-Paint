#include <stdio.h>
#include <time.h>
#include <stdlib.h> //this is where rand lives
#include <stdbool.h>
#include <math.h>

#include "events.h"
#include "graphics.h"
#include "objects.h"
#include "objectlist.h"

#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 500

#define FPS 60
#define GAME_SPEED .85
#define DIFFICULTY .5

ObjectList objects;

void gamethread();

void start(){
	objects=objectList(200);
	add_object(&objects, water(objects.size,-10,WINDOW_HEIGHT-32));
	add_object(&objects, penguin(objects.size,arrows(),WINDOW_WIDTH/2,WINDOW_HEIGHT/2,color(255,255,255)));
	add_object(&objects, penguin(objects.size,wasd(),WINDOW_WIDTH/2,WINDOW_HEIGHT/2,color(0,255,255)));
	for (int i=0;i<WINDOW_WIDTH;i+=32)
		add_object(&objects, box(objects.size,i,WINDOW_HEIGHT-100));
//	add_object(&objects, crawler(objects.size,0,468));
/*	add_object(&objects, box(objects.size,232,364));
	add_object(&objects, box(objects.size,264,332));
	add_object(&objects, box(objects.size,200,364));
	add_object(&objects, box(objects.size,200,300));
	add_object(&objects, box(objects.size,200,240));*/
	gamethread();
}

void gamethread(){
	double time=GAME_SPEED;
	int key;
	int step=0;
	double difficulty=DIFFICULTY;
	printf("Starting new game\n");
	while (true){
//	while (penguin_alive(&objects)){
		GameObject o;
		GameObject* other;
		step++;

		if (rand()%(int)(30/time/difficulty)==0){
			o=bomb(objects.size,round(rand()%(WINDOW_WIDTH-32)/16.0)*16,-32);
			if ((other=is_touching_solid(&objects,&o))->classid==-1)
				add_object(&objects, o);
		}
		if (rand()%(int)(200/time/difficulty)==0){
			o=bonusbox(objects.size,round(rand()%(WINDOW_WIDTH-32)/16.0)*16,-32);
			if (is_touching_solid(&objects,&o)->classid==-1)
				add_object(&objects, o);
		}
		if (rand()%(int)(20/time/difficulty)==0){
			o=box(objects.size,round(rand()%(WINDOW_WIDTH-32)/16.0)*16,-32);
			if (is_touching_solid(&objects,&o)->classid==-1)
				add_object(&objects, o);
		}
		if (rand()%(int)(100/time/difficulty)==0){
			o=metal(objects.size,round(rand()%(WINDOW_WIDTH-32)/16.0)*16,-32);
			if (is_touching_solid(&objects,&o)->classid==-1)
				add_object(&objects, o);
		}
		if (rand()%(int)(140/time/difficulty)==0){
			o=missile(objects.size,round(rand()%(WINDOW_WIDTH-32)/16.0)*16,-32);
			if (is_touching_solid(&objects,&o)->classid==-1)
				add_object(&objects, o);
		}
		int prob=(int)(pow(145-objects.size,1.82)/time);
		if (prob<50) prob=50;
		if (rand()%prob==0 && object_count(&objects, 7)==0){
			o=crawler(objects.size,0,468);
			add_object(&objects, o);
		}

		//Get correct keyboard state, check for window closing (-1)
		if (handleEvents()==-1) return;
		//Do step for all objects
		int size=objects.size;
		for (int i=0;i<size;i++){
			step_object(&objects,get_object(&objects,i),time);
		}
		//Removing destroyed objects
		for (int i=0;i<objects.size;i++){
			if (get_object(&objects, i)->will_destroy){
				GameObject go = *get_object(&objects, i);
				remove_index(&objects,i);	
				destroy_object(&objects,&go);
			}
		}

		//Drawing
		//if (step%100==0){
		//SDL_Delay(100);
		clear_graphics();
		for (int i=1;i<objects.size;i++) if (!get_object(&objects,i)->effect){
			draw_object(&objects, get_object(&objects,i));
		}
		for (int i=1;i<objects.size;i++) if (get_object(&objects,i)->effect){
			draw_object(&objects, get_object(&objects,i));
		}
		draw_object(&objects, get_object(&objects,0));
		render_graphics();

		//Delay
		SDL_Delay(1000/FPS);

		//}
	}
	start();
}


int main( int argc, char *args[]){
	srand(0);
	init_graphics(0,0,WINDOW_WIDTH,WINDOW_HEIGHT+20,color(150,200,255));
	start();
	obliterate_graphics();
}
