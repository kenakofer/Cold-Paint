#ifndef OBJECTS
#define OBJECTS

#include "structs.h"
#include <stdbool.h>


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
GameObject wipeout(int id, double x, double y);
GameObject missile(int id, double x, double y);
GameObject bonusbox(int id, double x, double y);
GameObject powerbox(int id, double x, double y);
GameObject* null();



void start_game(GameProperties * gp);

bool probably_add_object(double probability, GameObject (*f)(int i, double j, double k));

int sign(double a);
//Performs the changes to a game object as they would happen in <time> amount of time. The time parameter allows easy speeding and slowing of the game.
void step_object(GameObject* go);

void destroy_object(GameObject* go);

//Draws the gameobject
void draw_object(GameObject* go);

//
bool is_touching(GameObject* go1, GameObject* go2);

GameObject* is_touching_solid(GameObject* go);
GameObject* is_touching_any(GameObject* go);
bool is_touching_water(GameObject* go);

void set_score(GameObject * go,int s);
void add_score(GameObject * go,int s);
int get_score(GameObject * go);


#endif
