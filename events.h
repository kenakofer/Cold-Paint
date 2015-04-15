#ifndef EVENTS
#define EVENTS

#include "objects.h"
#include "structs.h"

int arrows();
int wasd();

bool up(int i);
bool down(int i);
bool left(int i);
bool right(int i);
bool F();
bool P();
int handleEvents();

void fetch_movements(MovementList* ml, int pen_num, int step);
#endif
