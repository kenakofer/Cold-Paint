#ifndef OBJECTLIST
#define OBJECTLIST


#include "structs.h"
#include <stdbool.h>



ObjectList objectList(int capacity);
void add_object(ObjectList* ol, GameObject go);
void add_object_at_position(ObjectList* ol, GameObject go, int n);
GameObject* get_object(ObjectList* ol, int index);
void remove_index(ObjectList* ol, int index);
bool penguin_alive(ObjectList* ol);
int object_count(ObjectList* ol, int classid);

#endif
