#ifndef OBJECTS
#define OBJECTS

#include "graphics.h"
#include <stdbool.h>

typedef enum {NONE=0, SAFE_WATER=1, SAFE_EXPLODE=1<<1, FLY=1<<2, BOX_COLOR=1<<3, METALIZE=1<<4, BOMBIZE=1<<5, IS_SOLID=1<<6} powerups;

typedef enum {NULL_ID=0,PEN_ID,BOX_ID,BOM_ID,MET_ID,WAT_ID,EXP_ID,SPL_ID,CRA_ID,MIS_ID,DRI_ID,GHO_ID,NUM_ID,BON_ID,POW_ID,SMA_ID} classids;


typedef struct {
	int id;
	int classid;
	int control;	//arrows or wasd
	double x;		//Need finer adjustment than int
	double y;
	int width;
	int height;
	double timer;		//used for "bomb" class
	bool will_destroy;
	bool on_ground;
	bool solid;
	bool explodable;
	bool effect;
	bool floats;
	bool no_leave_screen;
	int marked;
	double score;
	int pow;
	Color color;

	//Penguin stuff
	double yspeed;
	bool ducking;

	//May not be useful
	double xspeed;

} GameObject;

typedef struct {
	GameObject* array;
	int size;
	int capacity;
} ObjectList;

typedef struct {
	int width;			//of window
	int height;
	int resolution;		//how wide/tall are the cells
	Color background;

	double pen_jump;
	double pen_move;
	double difficulty;
	double game_speed;
	double FPS;
	double gravity;
	double box_fall;

	double bomb_time;
	double ghost_time;
	double powerup_time;

	int step;

	ObjectList objects;

} GameProperties;

void start_game(GameProperties * gp);

//Constructors
GameObject penguin(int id, int control, double x, double y, Color c);
GameObject bomb(int id, double x, double y);
GameObject box(int id, double x, double y);
GameObject smallbox(int id, double x, double y);
GameObject metal(int id, double x, double y);
GameObject water(int id, double x, double y);
GameObject explosion(int id, double x, double y, int radius);
GameObject splinter(int id, Color c, double x, double y);
GameObject crawler(int id, double x, double y);
GameObject missile(int id, double x, double y);
GameObject bonusbox(int id, double x, double y);
GameObject powerbox(int id, double x, double y);
GameObject* null();



void start_game(GameProperties * gp);

bool probably_add_object(ObjectList* objects, double probability, GameObject (*f)(int i, double j, double k));

int sign(double a);
//Performs the changes to a game object as they would happen in <time> amount of time. The time parameter allows easy speeding and slowing of the game.
void step_object(ObjectList* objects, GameObject* go);

void destroy_object(ObjectList* objects, GameObject* go);

//Draws the gameobject
void draw_object(ObjectList* objects, GameObject* go);

//
bool is_touching(GameObject* go1, GameObject* go2);

GameObject* is_touching_solid(ObjectList* objects, GameObject* go);
GameObject* is_touching_any(ObjectList* objects, GameObject* go);
bool is_touching_water(ObjectList* objects, GameObject* go);

void set_score(GameObject * go,int s);
void add_score(GameObject * go,int s);
int get_score(GameObject * go);


#endif
