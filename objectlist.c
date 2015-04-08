
#include "structs.h"
#include <stdbool.h>


ObjectList objectList(int capacity){
	ObjectList ol;
	ol.size=0;
	ol.capacity=capacity;
	ol.array=(GameObject*)malloc(sizeof(GameObject)*ol.capacity);
	return ol;
}

void add_object(ObjectList* ol, GameObject go){
	if (ol->size >= ol->capacity){
		ol->capacity*=2;
		ol->array=(GameObject*)realloc(ol->array,sizeof(GameObject)*ol->capacity);
	}
	ol->array[ol->size++]=go;
}

void add_object_at_position(ObjectList* ol, GameObject go, int n){
	if (ol->size >= ol->capacity){
		ol->capacity*=2;
		ol->array=(GameObject*)realloc(ol->array,sizeof(GameObject)*ol->capacity);
	}
	//Shift the item at n to the back
	ol->array[ol->size]=ol->array[n];
	//Set the id of the shifted item
	ol->array[ol->size].id=ol->size++;
	//Insert the new item at n
	ol->array[n]=go;
}
GameObject* get_object(ObjectList* ol, int index){
	return &(ol->array[index]);
}
void remove_index(ObjectList* ol, int index){
	ol->array[index] = ol->array[--ol->size];
	ol->array[index].id=index;
}

bool penguin_alive(ObjectList* ol){
	for (int i=0; i<ol->size; i++) if (get_object(ol,i)->classid==PEN_ID) return true;
	return false;
}

int object_count(ObjectList* ol, int classid){
	int count=0;
	for (int i=0; i<ol->size; i++) if (get_object(ol,i)->classid==classid) count++;
	return count;

}


