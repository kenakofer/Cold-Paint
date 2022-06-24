#include "structs.h"
#include "objects.h"
#include "objectlist.h"
#include "bitoperations.h"

#include "graphics.h"
#include "events.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <gsl/gsl_rng.h>

//#define go_in_objects{ (int i=0; i<objects->size; i++){ GameObject* go = get_object(objects, i);

GameProperties* game;
ObjectList* objects;
int global_timer=0;

int randcount=0;

gsl_rng *r1, *r2, *r3;

void setup_random(){
  const gsl_rng_type * T;
  gsl_rng_env_setup();
  T = gsl_rng_default;
  r1 = gsl_rng_alloc (T);
  r2 = gsl_rng_alloc (T);
  r3 = gsl_rng_alloc (T);
}

int random(int whichgen){
	int result;
	switch (whichgen){
		case (0):
			result=gsl_rng_uniform_int(r1,1000001);
			break;
		case (1):
			result=gsl_rng_uniform_int(r2,1000001);
			break;
		case (2):
			result=gsl_rng_uniform_int(r3,1000001);
			break;
		default:
			result= rand();
			break;
	}
	return result;
}
/*
int random(int marker){
	randcount++;
	int r=rand();
	return r;
}
*/

void slo_mo(){
	game->speed=game->slomo_speed;
	game->slomo_timer=game->slomo_time;
}

void gamethread(){
	while (true) {
		int start=SDL_GetTicks();

		//Keyboard events
		if (handleEvents()==-1) return;

		if (!P()){
			GameObject o;
			GameObject* other;

			//rain stuff
			double factor = game->speed * game->difficulty;
			probably_add_object(1/30.0*factor, bomb);
			probably_add_object(1/1000.0*factor, bonusbox);
			if (!object_count(objects, POW_ID))
				probably_add_object(1/200.0*factor, powerbox);
			probably_add_object(1/20.0*factor, box);
			probably_add_object(1/200.0*factor, smallbox);
			probably_add_object(1/100.0*factor, metal);
			probably_add_object(1/140.0*factor, missile);

			//Crawlers
			int prob=(int)(pow(145-objects->size,1.82)/game->speed);
			if (prob<50) prob=50;
			if (random(0)%prob==0 && object_count(objects,CRA_ID)==0){
				o=crawler(objects->size,0,468);
				add_object(objects, o);
			}


			int size=objects->size;

			//Slo_mo
			if (game->slomo_timer>0){
				game->slomo_timer--;
			}
			else{
				game->slomo_timer=0;
				if (game->speed < game->normal_speed)
					game->speed+= 0.05;
				if (game->speed > game->normal_speed)
					game->speed = game->normal_speed;
			}



			//Step
			for (int i=0;i<size;i++) step_object(get_object(objects,i));

			//Removal
			for (int i=0;i<objects->size;i++) if (get_object(objects, i)->will_destroy){
				GameObject go = *get_object(objects, i);
				remove_index(objects,i);
				destroy_object(&go);
			}

			//Recording/Replaying
			if (game->recording) for (int i=0; i<game->pen_num; i++){
				setbit(&(game->movements), game->movements.size, up(i));
				setbit(&(game->movements), game->movements.size, down(i));
				setbit(&(game->movements), game->movements.size, left(i));
				setbit(&(game->movements), game->movements.size, right(i));
			}
			else if (game->replaying){
				fetch_movements(&(game->movements), game->pen_num, game->step);
			}

			game->step++;
		}


		//Drawing
		//
		clear_graphics();
		for (int i=1;i<objects->size;i++) if (!get_object(objects,i)->effect) {
			draw_object(get_object(objects,i));
		}
		for (int i=1;i<objects->size;i++) if (get_object(objects,i)->effect){
			draw_object(get_object(objects,i));
		}
		draw_object(get_object(objects,0));
		render_graphics();

		//Delay
		int dur=SDL_GetTicks()-start;
		if (dur<1000/game->FPS) SDL_Delay(1000/game->FPS - dur);

	}
}

void start_game(GameProperties * g){
	game=g;
	objects=&(g->objects);
	//Open window
	init_graphics(0,0,512,500,game->background);
	srand(0);
	setup_random();

	//Initial objects
	*objects=objectList(200);
	add_object(objects, water(objects->size,-10,game->height-32));
	if (game->pen_num>0) add_object(objects, penguin(objects->size,arrows(),game->width/2,game->height/2,color(255,255,255)));
	if (game->pen_num>1) add_object(objects, penguin(objects->size,wasd(),game->width/2,game->height/2,color(0,255,255)));
	for (int i=0;i<game->width;i+=32)
		add_object(objects, box(objects->size,i,game->height-100));

	gamethread();
	gsl_rng_free (r1);
	gsl_rng_free (r2);
	gsl_rng_free (r3);
	if (game->recording) write_movement_data(game->recordfile, game);
}

void set_score(GameObject * go,int s){
	go->score=s;
}
void add_score(GameObject * go,int s){
	go->score+=s;
	game->difficulty+= .002*s;
}
int get_score(GameObject * go){
	return go->score;
}

GameObject penguin(int id, int control, double x, double y, Color c) {
	GameObject p;
	p.id=id;
	p.control=control;
	p.classid=PEN_ID;
	p.x=x;
	p.y=y;
	p.width=game->resolution;
	p.height=game->resolution*2;
	p.will_destroy=false;
	p.on_ground=false;
	p.xspeed=0;
	p.yspeed=0;
	p.ducking=false;
	p.color=c;
	p.solid=false;
	p.explodable=true;
	p.effect=false;
	p.floats=false;
	p.marked=-1;
	p.score=0;
	p.no_leave_screen=true;
	p.pow=NONE;
	p.timer=0;
	p.blacken=false;
	return p;
}
GameObject penguin_from_ghost(int id, GameObject* g) {
	GameObject p;
	p.id=id;
	p.control=g->control;
	p.classid=PEN_ID;
	p.x=g->x;
	p.y=g->y;
	p.width=game->resolution;
	p.height=game->resolution*2;
	p.will_destroy=false;
	p.on_ground=false;
	p.xspeed=0;
	p.yspeed=0;
	p.ducking=false;
	Color c=g->color;
	p.color=color(c.red,c.green,c.blue);
	p.solid=false;
	p.explodable=true;
	p.effect=false;
	p.floats=false;
	p.marked=-1;
	p.score=g->score;
	p.no_leave_screen=true;
	p.pow=NONE;
	p.timer=0;
	p.blacken=false;
	return p;
}

GameObject ghost(int id, GameObject* pen){
	GameObject o;
	o.id=id;
	o.control=pen->control;
	o.classid=GHO_ID;
	o.x=pen->x;
	o.y=pen->y;
	if (pen->height==game->resolution) o.y-=game->resolution;
	o.width=game->resolution;
	o.height=game->resolution*2;
	o.will_destroy=false;
	o.on_ground=false;
	o.xspeed=0;
	o.yspeed=0;
	o.ducking=false;
	Color c=pen->color;
	o.color=colora(c.red,c.green,c.blue,150);
	o.solid=false;
	o.explodable=false;
	o.effect=true;
	o.floats=false;
	o.marked=-1;
	o.score=pen->score;
	o.timer=game->ghost_time;
	o.no_leave_screen=true;
	o.blacken=false;
	return o;
}

GameObject bomb(int id, double x, double y){
	GameObject o;
	o.id=id;
	o.classid=BOM_ID;
	o.x=x;
	o.y=y;
	o.width=game->resolution*2;
	o.height=game->resolution*2;
	o.will_destroy=false;
	o.on_ground=false;
	o.xspeed=0;
	o.yspeed=0;
	o.timer=game->bomb_time;
	o.solid=true;
	o.color=color(200+random(3)%30,100+random(3)%10,100+random(3)%10);
	o.explodable=true;
	o.effect=false;
	o.floats=true;
	o.marked=-1;
	o.score=1;
	o.no_leave_screen=false;
	o.blacken=false;
	return o;
}
GameObject box(int id, double x, double y){
	GameObject o;
	o.id=id;
	o.classid=BOX_ID;
	o.x=x;
	o.y=y;
	o.width=game->resolution*2;
	o.height=game->resolution*2;
	o.will_destroy=false;
	o.on_ground=false;
	o.xspeed=0;
	o.yspeed=0;
	o.solid=true;
	o.color=color(200+random(6)%30,100+random(7)%10,100+random(8)%10);
	o.explodable=true;
	o.effect=false;
	o.floats=true;
	o.marked=-1;
	o.score=1;
	o.blacken=false;
	o.no_leave_screen=false;
	return o;
}
GameObject smallbox(int id, double x, double y){
	GameObject go=box(id,x,y);
	go.classid=SMA_ID;
	go.width=game->resolution;
	go.height=game->resolution;
	return go;
}

GameObject bonusbox(int id, double x, double y){
	GameObject o;
	o.id=id;
	o.classid=BON_ID;
	o.x=x;
	o.y=y;
	o.width=game->resolution*2;
	o.height=game->resolution*2;
	o.will_destroy=false;
	o.on_ground=false;
	o.xspeed=0;
	o.yspeed=0;
	o.solid=true;
	o.color=color(200+random(9)%30,100+random(10)%10,100+random(11)%10);
	o.explodable=true;
	o.effect=false;
	o.floats=true;
	o.marked=-1;
	o.score=0;
	o.no_leave_screen=false;
	o.blacken=false;
	return o;
}
GameObject powerbox(int id, double x, double y){
	GameObject o;
	o.id=id;
	o.classid=POW_ID;
	o.x=x;
	o.y=y;
	o.width=game->resolution*2;
	o.height=game->resolution*2;
	o.will_destroy=false;
	o.on_ground=false;
	o.xspeed=0;
	o.yspeed=0;
	o.solid=true;
	o.color=color(200+random(12)%30,150+random(12)%10,120+random(12)%10);
	o.explodable=true;
	o.effect=false;
	o.floats=true;
	o.marked=-1;
	o.score=0;
	o.pow=random(13)%2-3;
	o.no_leave_screen=false;
	o.blacken=false;
	return o;
}
GameObject metal(int id, double x, double y){
	GameObject o;
	o.id=id;
	o.classid=MET_ID;
	o.x=x;
	o.y=y;
	o.width=game->resolution*2;
	o.height=game->resolution*2;
	o.will_destroy=false;
	o.on_ground=false;
	o.xspeed=0;
	o.yspeed=0;
	o.solid=true;
	o.color=color(150+random(14)%30,150+random(15)%10,150+random(16)%10);
	o.explodable=false;
	o.effect=false;
	o.floats=false;
	o.marked=-1;
	o.score=3;
	o.no_leave_screen=false;
	o.blacken=false;
	return o;
}
GameObject water(int id, double x, double y){
	GameObject o;
	o.id=id;
	o.classid=WAT_ID;
	o.x=x;
	o.y=y;
	o.width=1000;
	o.height=1000;
	o.will_destroy=false;
	o.on_ground=false;
	o.color=colora(230,230,230,200);
	o.xspeed=0;
	o.yspeed=0;
	o.solid=false;
	o.explodable=false;
	o.effect=false;
	o.marked=-1;
	o.no_leave_screen=false;
	o.blacken=false;
	return o;
}
GameObject explosion(int id, double x, double y, int radius){
	GameObject o;
	o.id=id;
	o.classid=EXP_ID;
	o.x=x-radius;
	o.y=y-radius;
	o.width=2*radius;
	o.height=2*radius;
	o.will_destroy=false;
	o.on_ground=false;
	o.xspeed=0;
	o.yspeed=0;
	o.solid=false;
	o.color=color(255,100,100);
	o.explodable=false;
	o.effect=false;
	o.marked=-1;
	o.no_leave_screen=false;
	o.blacken=false;
	return o;
}
GameObject splinter(int id, Color c, double x, double y){
	GameObject o;
	o.id=id;
	o.classid=SPL_ID;
	int radius=random(17)%5+1;
	o.x=x-radius;
	o.y=y-radius;
	o.width=2*radius;
	o.height=2*radius;
	o.will_destroy=false;
	o.on_ground=false;
	o.xspeed=random(18)%20-10;
	o.yspeed=random(19)%20-10;
	o.solid=false;
	o.explodable=false;
	o.effect=true;
	o.color=c;
	o.marked=-1;
	o.no_leave_screen=false;
	o.blacken=false;
	return o;
}
GameObject crawler(int id, double x, double y){
	GameObject o;
	o.id=id;
	o.classid=CRA_ID;
	int radius=12;
	o.x=x-radius;
	o.y=y-radius;
	o.width=2*radius;
	o.height=2*radius;
	o.will_destroy=false;
	o.on_ground=false;
	o.xspeed=1;
	o.solid=false;
	o.explodable=false;
	o.effect=false;
	o.color=color(50,50,100);
	o.marked=-1;
	o.no_leave_screen=false;
	o.score=5;
	o.blacken=false;
	return o;
}
GameObject missile(int id, double x, double y){
	GameObject o;
	o.id=id;
	o.classid=MIS_ID;
	o.x=x;
	o.y=y;
	o.width=game->resolution;
	o.height=25;
	o.will_destroy=false;
	o.on_ground=false;
	o.yspeed=1;
	o.solid=false;
	o.explodable=true;
	o.effect=false;
	o.color=color(0,0,0);
	o.marked=-1;
	o.no_leave_screen=false;
	o.score=5;
	o.blacken=false;
	return o;
}
GameObject drip(int id, Color c, double x, double y){
	GameObject o;
	o.id=id;
	o.classid=DRI_ID;
	o.x=x;
	o.y=y;
	o.width=2;
	o.height=3;
	o.will_destroy=false;
	o.on_ground=false;
	o.yspeed=0;
	o.solid=false;
	o.explodable=false;
	o.effect=true;
	o.color=c;
	o.marked=-1;
	o.no_leave_screen=false;
	o.blacken=false;
	return o;

}

GameObject number(int id, int n, Color c, double x, double y){
	GameObject o;
	o.id=id;
	o.classid=NUM_ID;
	o.x=x;
	o.y=y;
	o.width=10;
	o.height=20;
	o.will_destroy=false;
	o.on_ground=false;
	o.xspeed=random(20)%6-3;
	o.yspeed=random(21)%5-10;
	o.solid=false;
	o.explodable=false;
	o.effect=true;
	o.color=c;
	o.marked=-1;
	o.no_leave_screen=false;
	o.score=n;
	o.blacken=false;
	return o;

}
GameObject wipeout(int id, double x, double y){
	GameObject o;
	o.id=id;
	o.classid=WIP_ID;
	if (random(22)%2==0){
		o.x=0;
		o.xspeed=.5;
	} else {
		o.x=game->width;
		o.xspeed=-.5;
	}
	o.y=0;
	o.width=1;
	o.height=1000;
	o.will_destroy=false;
	o.on_ground=false;
	o.solid=false;
	o.explodable=false;
	o.effect=true;
	o.color=color(255,0,0);
	o.marked=-1;
	o.no_leave_screen=false;
	o.blacken=false;
	o.score=0;
	return o;

}

GameObject null_object;
GameObject* null(){
	null_object.classid=NULL_ID;
	return &null_object;
}

bool probably_add_object( double probability, GameObject (*f)(int i, double j, double k)){
	int r=random(0);
	if (r%100000<probability*100000){
		GameObject o= f(objects->size,round(random(0)%(game->width-32)/16.0)*16,-32);
		if (is_touching_solid(&o)->classid==NULL_ID)
			add_object(objects, o);
			return true;
	}
	return false;
}

int sign(double a){
	return (a>=0)*2-1;
}
bool is_touching(GameObject* go1, GameObject* go2){
	if (go1->id!=go2->id){
		double ax1=go1->x;
		double ax2=go1->x+go1->width;
		double ay1=go1->y;
		double ay2=go1->y+go1->height;
		double bx1=go2->x;
		double bx2=go2->x+go2->width;
		double by1=go2->y;
		double by2=go2->y+go2->height;
		if (ax1 < bx2 && ax2 > bx1 && ay1 < by2 && ay2 > by1) return true;
	}
	return false;
}
GameObject* is_touching_class(GameObject* go, int obj_id){
	for (int i=0;i<objects->size;i++){
		GameObject* other=get_object(objects,i);
		if (other->classid==obj_id && is_touching(go,other)) return other;
	}
	return null();
}

bool is_neighboring(GameObject * go1, GameObject* go2){
	GameObject bigger = *go1;
	bigger.x--;
	bigger.y--;
	bigger.width+=2;
	bigger.height+=2;
	return is_touching(&bigger, go2);
}

GameObject* is_touching_solid(GameObject* go){
	for (int i=0;i<objects->size;i++){
		GameObject* other=get_object(objects,i);
		if (is_touching(go,other) && (other->solid || (go->floats && other->classid==WAT_ID)
					|| (go->classid==PEN_ID && other->classid==PEN_ID))) {
			return other;
		}
	}
	return null();
}
GameObject* is_touching_any(GameObject* go){
	for (int i=0;i<objects->size;i++){
		GameObject* other=get_object(objects,i);
		if (!other->effect && is_touching(go,other)) return other;
	}
	return null();
}
bool is_touching_penguin(GameObject* go){
	int i=1;
	GameObject * other;
	while ((other=get_object(objects,i))->classid==PEN_ID || other->classid==GHO_ID){
		if (is_touching(go,other)) return true;
	}
	return false;
}
bool is_touching_water(GameObject* go){
	return(is_touching(go,get_object(objects,0)));
}
bool is_on_ground(GameObject* go){
	bool result;
	GameObject o=*go;
	o.y=go->y+go->height;
	o.height=1;
	result=is_touching_solid(&o)->classid!=NULL_ID;
	return result;
}
bool is_on_water(GameObject* go){
	bool result;
	go->y++;
	result=is_touching_water(go);
	go->y--;
	return result;
}

double settle_down(GameObject* go){
	GameObject* other;
	GameObject o=*go;
	o.y=go->y+go->height;
	o.height=1;
	other=is_touching_solid(&o);
	go->y=other->y-go->height;
}


double resolve_left(GameObject* go){
	GameObject* other;
	double old=go->x;
	while ((other=is_touching_solid(go))->classid!=NULL_ID){
		go->x=other->x-go->width;
	}
	return old-go->x;
}
double resolve_right(GameObject* go){
	GameObject* other;
	double old=go->x;
	while ((other=is_touching_solid(go))->classid!=NULL_ID){
		go->x=other->x+other->width;
	}
	return go->x-old;
}
double resolve_up(GameObject* go){
	GameObject* other;
	double old=go->y;
	while ((other=is_touching_solid(go))->classid!=NULL_ID){
		if (go->y!=other->y-go->height)
			go->y=other->y-go->height;
		else go->y--;
	}
	return old-go->y;
}
double resolve_down(GameObject* go){
	GameObject* other;
	double old=go->y;
	while ((other=is_touching_solid(go))->classid!=NULL_ID){
		go->y=other->y+other->height;
	}
	return go->y-old;
}



void add_negative(int which){
	switch (which){
		case (WIPEOUT):
			add_object(objects, wipeout(objects->size,0,0));
			break;
		case (BLACKEN):
			for (int i=0; i<objects->size; i++){ GameObject* go = get_object(objects, i);
//			for go_in_objects{ TODO fix
				if (go->classid!=WAT_ID && go->effect==false)
					go->blacken=true;
			}
			break;

	}
}

void add_powerup(GameObject* go){
	int which=random(1)%3;
	switch (which){
		case (0):
			go->floats=true;
			go->pow=go->pow | SAFE_WATER;
			break;
		case (1):
			go->explodable=false;
			go->pow=go->pow | SAFE_EXPLODE;
			break;
		case (2):
			go->solid=true;
			go->pow=go->pow | IS_SOLID;
			break;
	}
	go->timer=game->powerup_time;
}

void reset_powerups(GameObject* go){
	go->floats=false;
	go->explodable=true;
	go->solid=false;
	go->pow=NONE;
}

void step_object(GameObject* go){
	GameObject* other; //Useful in cases of collisions
	double offset;
	bool old;

	//Universal step, checks for things outside useful bounds
	if (go->x < -50 || go->y < -50 || go->y > 550 || go-> x > 550){
		go->will_destroy=true;
		return;
	}
	//Check to move certain things (penguin, ghost) back into useful bounds
	if (go->no_leave_screen){
		if (go->x < 0) go->x=0;
		if (go->y < 0) {
			go->y=0;
			go->yspeed*=-1;
		}
		if (go->x+go->width > 512) go->x=512-go->width;
		if (go->y+go->height > 500) go->y=500-go->height;
	}


	switch (go->classid){
		case (PEN_ID):

			//Check for initial collisions, which would indicate that some other object moved onto the penguin, from above
			if (is_touching_solid(go)->classid!=NULL_ID){
				if (is_on_ground(go)){
					if (go->ducking) {
						go->will_destroy=true;
						return;
					}
					else {
						go->ducking=true;
						go->height=game->resolution;
						double sy=go->y;
						resolve_down(go);
						if (go->y-sy>32){
							go->y=sy;
							resolve_up(go);
						}
						else{
							go->yspeed=4+abs(go->yspeed)/2*game->speed;
						}
					}
				}
				else {
					resolve_down(go);
				}
			}

			//Gravity and vertical speed enaction
			go->yspeed+=game->gravity*game->speed;
			go->y+=go->yspeed*game->speed;
			//Moving down...
			if (go->yspeed>0) {
				double sy=go->y;
				go->on_ground=(resolve_up(go));
				if (sy-go->y>16) {
					go->y=sy;

				}
				if (go->on_ground) go->yspeed=0;
			}
			else {
				if (!up(go->control)) go->yspeed*=.94;
				if (resolve_down(go)!=0) go->yspeed*=-1;

			}

			//Ninja jump off missile
			if (up(go->control) && (other=is_touching_class(go,MIS_ID))->classid!=NULL_ID){
				go->yspeed = game->pen_jump;
				go->y+= go->yspeed-5;
				if (!is_touching(go,other)){

					other->yspeed += 16;
					other->marked = go->id;
					other->color = vary(go->color,100);

					slo_mo();
				}
			}

			//Controls
			if (down(go->control)) {
				go->ducking=true;
				go->height=game->resolution;
			}
			else if (go->ducking) {
				//Handles standing up from a duck
				go->height=game->resolution*2;
				double sy=go->y;
				double r = resolve_up(go);
				if (r>16){
					go->y=sy;
					go->height=game->resolution;
				}
				else go->ducking=false;
			}
			if (left(go->control) && is_touching_solid(go)->classid==NULL_ID) {
				go->x-=game->pen_move*game->speed;
				resolve_right(go);
			}
			if (right(go->control) && is_touching_solid(go)->classid==NULL_ID) {
				go->x+=game->pen_move*game->speed;
				resolve_left(go);
			}
			if (!left(go->control) && !right(go->control) && fmod(round(go->x),8)!=0){
				go->x=round(go->x);
				if (fmod(go->x,8.0)<4) {
					go->x--;
					resolve_right(go);
				}
				else {
					go->x++;
					resolve_left(go);
				}

			}
			if (up(go->control) && go->on_ground) {
				go->yspeed=game->pen_jump;
				go->on_ground=false;
			}

			//Check to see if the penguin is chillin in the water
			if (is_touching_water(go)) go->will_destroy=true;

			//Check for touching boxes
			for (int i=0;i<objects->size;i++){
				GameObject* other=get_object(objects,i);
				if (is_neighboring(go,other) && other->solid
						&& other->marked!=go->id && other->marked>-2
						&& !(other->marked>-1 && is_neighboring(other,get_object(objects,other->marked)))
						&& other->classid!=PEN_ID
				   ){


					other->marked=go->id;
					other->color=lighten(go->color,-20/*-(random(26*go->id)%15)*/); //TODO
					if (other->classid==BON_ID) other->score+=2;

				}
			}

			//Drip effect
			//TODO
			if (random(27*go->id)%(int)(20/game->speed+2)==0){
				add_object(objects, drip(objects->size,lighten(go->color,-50),go->x+(random(28)%go->width),go->y+(random(29)%go->height)));
			}

			//POwerup timer
			if (go->timer>0){
				go->timer-=1*game->speed;
				if (go->timer<=0){
					reset_powerups(go);
					go->timer=0;
				}
			}


			break;
		case (GHO_ID):
			if (up(go->control)) go->y-=1*game->speed;
			if (down(go->control)) go->y+=1*game->speed;
			if (left(go->control)) go->x-=1*game->speed;
			if (right(go->control)) go->x+=1*game->speed;
			//Position rounding
			if (!left(go->control) && !right(go->control) && fmod(round(go->x),8)!=0){
				go->x=round(go->x);
				if (fmod(go->x,8.0)<4) go->x-=1;
				else go->x++;
			}
			go->timer-=1*game->speed;
			if (go->timer<=0) go->will_destroy=true;
			break;

		case (MET_ID):
			if (is_touching_water(go))	{
				go->y+=game->box_fall*game->speed*.25;
				if (go->marked>-1){
					add_score(get_object(objects, go->marked),go->score);
					add_object(objects, number(objects->size,go->score,go->color,go->x+go->width/2,go->y+go->height-5));
					go->marked=-2;
					go->color=color(150+random(30)%30,150+random(31)%10,150+random(32)%10); //TODO
				}


			}
			else go->y+=game->box_fall*game->speed;
			go->on_ground=(resolve_up(go)!=0);
			break;
		case (BOX_ID):
		case (SMA_ID):
		case (BON_ID):
		case (POW_ID):
			go->y+=game->box_fall*game->speed;
			go->on_ground=(resolve_up(go)!=0);
			break;
		case (BOM_ID):
			go->y+=game->box_fall*game->speed;
			go->on_ground=(resolve_up(go)!=0);
			if (go->on_ground) go->timer-=game->speed;
			if (go->timer<=0) go->will_destroy=true;
			break;
		case (EXP_ID):
			for (int i=0; i<objects->size; i++){
				other=get_object(objects,i);
				if (other->explodable && is_touching(go,other)){
					other->will_destroy=true;
				}
			}
			if (go->width<5) go->will_destroy=true;
			else {
				go->width-=(int)(6*game->speed);
				go->height-=(int)(6*game->speed);
				go->x+=3*game->speed;
				go->y+=3*game->speed;
			}

			break;
		case (CRA_ID):
			go->x+=go->xspeed*game->speed;
			for (int i=0; i<objects->size; i++){
				other=get_object(objects,i);
				if (other->explodable && is_touching(go,other)){
					other=get_object(objects,i);
					other->will_destroy=true;
					if (other->classid==BOM_ID || other->classid==BOX_ID){
						add_object(objects,metal(objects->size,other->x,other->y));//New object shoud wait to be processed.
					}

				}
			}
			break;
		case (WIP_ID):
			go->x+=go->xspeed*game->speed;
			for (int i=0; i<objects->size; i++){
				other=get_object(objects,i);
				if ((other->classid==SMA_ID) && is_touching(other,go)){
					other->will_destroy=true;
				}
				if ((other->classid==BOM_ID || other->classid==BOX_ID || other->classid==BON_ID) && is_touching(other,go)){
					other->will_destroy=true;
					add_object(objects,metal(objects->size,other->x,other->y));//New object shoud wait to be processed.
				}
			}
			break;
		case (MIS_ID):
			go->y+=go->yspeed*game->speed;
			go->yspeed+=.02*game->speed;
			if (is_touching_any(go)->classid!=NULL_ID) go->will_destroy=true;
			break;
		case (SPL_ID):
			go->x+=go->xspeed*game->speed;
			go->y+=go->yspeed*game->speed;
			go->yspeed+=game->gravity*game->speed;
			break;
		case (DRI_ID):
			go->yspeed+=game->gravity;
			if (is_touching(go,get_object(objects,1)) || is_touching(go,get_object(objects,2))){ //TODO better check
				go->yspeed=0.1;
			}
			go->y+=go->yspeed*game->speed;
			break;
		case (NUM_ID):
			go->y+=go->yspeed*game->speed;
			go->x+=go->xspeed*game->speed;
			go->yspeed+=game->gravity*game->speed*game->speed;
			break;
	}

}

void draw_outlined_rect(Color c1, Color c2, int x, int y, int w, int h){
	fillRect(c2,x+1,y+1,w-2,h-2);
	outlineRect(c1,x,y,w,h);
}

//Draws the gameobject
void draw_object(GameObject* go){
	int x=(int)go->x;
	int y=(int)go->y;
	int w=go->width;
	int h=go->height;
	int height;

	if (go->blacken){
		fillRect(color(0,0,0),x,y,w,h);
	}
	else
	switch (go->classid){
		case (EXP_ID):
		case (MIS_ID):
		case (SPL_ID):
		case (BOX_ID):
		case (SMA_ID):
		case (CRA_ID):
			draw_outlined_rect(color(0,0,0),go->color,x,y,w,h);
			break;
		case (WIP_ID):
			drawLine(go->color,x,y,x,game->height);
			break;
		case (PEN_ID):
			draw_outlined_rect(color(0,0,0),go->color,x,y,w,h);
			if (go->timer!=0){
				height=(int)((h-4)*(go->timer/game->powerup_time));
				fillRect(color(20,100,20),x+12,y+h-height-2,3,height);
			}
			if (go->pow & SAFE_EXPLODE)	fillRect(color(255,0,0),x+2,y+2,8,2);
			if (go->pow & SAFE_WATER) fillRect(color(0,0,255),x+2,y+6,8,2);
			if (go->pow & IS_SOLID) fillRect(color(180,80,120),x+2,y+10,8,2);
			break;
		case (MET_ID):
			draw_outlined_rect(color(0,0,0),go->color,x,y,w,h);
			Color c=game->background;
			Color b=color(0,0,0);
			draw_outlined_rect(b,c,x+4,y+4,10,10);
			draw_outlined_rect(b,c,x+18,y+4,10,10);
			draw_outlined_rect(b,c,x+4,y+18,10,10);
			draw_outlined_rect(b,c,x+18,y+18,10,10);
			break;
		case (BOM_ID):
			draw_outlined_rect(color(0,0,0),go->color,x,y,w,h);
			fillRect(color(255*(game->bomb_time-go->timer)/game->bomb_time,0,0),x+10,y+10,w-20,h-20);
			break;
		case (WAT_ID):
			fillRect(go->color,x,y-10,w,h);
			x+=5;
		 	for (int i=0; i<objects->size; i++){
				GameObject* go=get_object(objects,i);
				//drawing scores
				if (go->classid==PEN_ID || go->classid==GHO_ID){
					drawNum(color(0,0,0),(int)go->score,x+10,y,16,32);
					drawNum(go->color,(int)go->score,x+11,y-1,16,32);
					x+=80;
				}
			}
			break;
		case (GHO_ID):
			height=(int)(h*((game->ghost_time-go->timer)/game->ghost_time));
			fillRect(go->color,x,y+h-height,w,height);
			outlineRect(color(0,0,0),x,y,w,h);
			break;
		case (NUM_ID):
			drawNum(color(0,0,0),(int)go->score,x,y,w,h);
			x++;y++;
			if (go->score>2){
				drawNum(color(0,255,0),(int)go->score,x,y,w,h);
				x++; y++;
			}
			if (go->score>4){
				drawNum(color(0,0,0),(int)go->score,x,y,w,h);
				x++; y++;
			}
			drawNum(go->color,(int)go->score,x,y,w,h);
			break;
		case (BON_ID):
			draw_outlined_rect(color(0,0,0),go->color,x,y,w,h);
			drawNum(color(0,0,0),(int)go->score,x+3,y+3,w/2-5,h-6);
			drawNum(color(0,255,0),(int)go->score,x+2,y+2,w/2-5,h-6);
			break;
		case (POW_ID):
			draw_outlined_rect(color(0,0,0),go->color,x,y,w,h);
			drawLine(color(0,0,0),x,y+10,x+10,y);
			drawLine(color(0,0,0),x,y+h-11,x+10,y+h-1);
			drawLine(color(0,0,0),x+w-1,y+h-11,x+w-11,y+h-1);
			drawLine(color(0,0,0),x+w-11,y,x+w-1,y+10);
			break;
		case (DRI_ID):
			drawLine(color(0,0,0),x-1,y+1,x-1,y+3);
			drawLine(go->color,x,y,x,y+2);
			break;
	}
}
void destroy_object(GameObject* go){


	if (!go->effect && go->marked>-1){
		add_score(get_object(objects, go->marked),go->score);
		add_object(objects, number(objects->size,go->score,go->color,go->x+go->width/2,go->y+go->height-5));
	}

	switch (go->classid){
		case (MIS_ID):
			add_object(objects, explosion(objects->size,go->x+go->width/2,go->y+go->height-5,40));
			break;
		case (BOM_ID):
			add_object(objects, explosion(objects->size,go->x+go->width/2,go->y+go->height/2,32));
			for (int i=0;i<5;i++)add_object(objects, splinter(objects->size,go->color,go->x+go->width/2,go->y+go->height/2));
			break;
		case (POW_ID):
			if (go->marked<=-1){
				add_negative(go->pow);
			} else if (get_object(objects,go->marked)->classid==PEN_ID)
				add_powerup(get_object(objects,go->marked));

		case (BOX_ID):
		case (BON_ID):
		case (SMA_ID):
			for (int i=0;i<5;i++)add_object(objects, splinter(objects->size,go->color,go->x+go->width/2,go->y+go->height/2));
			break;
		case (CRA_ID):
			add_object(objects, explosion(objects->size,go->x+go->width/2,go->y+go->height/2,16));
			break;
		case (PEN_ID):
			add_object_at_position(objects, ghost(go->id, go),go->id);
			game->difficulty*=.9;
			for (int i=0;i<10;i++)add_object(objects, splinter(objects->size,go->color,go->x+go->width/2,go->y+go->height/2));
			break;
		case (GHO_ID):
			add_object_at_position(objects, penguin_from_ghost(go->id, go), go->id);
			break;

	}
}
