#ifndef BITOP
#define BITOP

#include "structs.h"

MovementList movementList(int c);

void setbit(MovementList * ml, int which, bool what);

bool getbit(MovementList * ml, int which);

void read_data(char* filename, GameProperties* gp);

void write_init_data(char* filename, GameProperties* gp);
void write_movement_data(char* filename, GameProperties* gp);


#endif
