#ifndef STRUCTS
#define STRUCTS

#include "graphics.h"

#include <stdbool.h>

//For some reason, these structs cause massive compilation issues when I try to separate them into separate files

typedef enum {BLACKEN=-3, WIPEOUT=-2, RAIN=-1, NONE=0, SAFE_WATER=1, SAFE_EXPLODE=1<<1, FLY=1<<2, BOX_COLOR=1<<3, METALIZE=1<<4, BOMBIZE=1<<5, IS_SOLID=1<<6} powerups;

typedef enum {NULL_ID=0,PEN_ID,BOX_ID,BOM_ID,MET_ID,WAT_ID,EXP_ID,SPL_ID,CRA_ID,MIS_ID,DRI_ID,GHO_ID,NUM_ID,BON_ID,POW_ID,SMA_ID,WIP_ID} classids;



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
	bool blacken;
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
	char* bitarray; //Actually storing bits of info
	int size;	   //In bits. Should be multiple of 8
	int capacity;  //In bits.
} MovementList;

typedef struct {
	int width;			//of window
	int height;
	int resolution;		//how wide/tall are the cells
	Color background;

	double pen_num;
	double pen_jump;
	double pen_move;
	double difficulty;
	double normal_speed;
	double FPS;
	double gravity;
	double box_fall;

	double bomb_time;
	double ghost_time;
	double powerup_time;

	int step;

	double slomo_time;
	double slomo_timer;
	double slomo_speed;
	double speed;

	int seed;
	bool replaying;
	bool recording;
	char recordfile[50];

	MovementList movements;

	ObjectList objects;

} GameProperties;

#endif
