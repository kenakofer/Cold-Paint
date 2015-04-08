#include "structs.h"

MovementList movementList(int capacity){
	MovementList ml;
	ml.size=0;
	ml.capacity=capacity;
	ml.bitarray=(char*)malloc(sizeof(char)*8*ol.capacity/8);
	return ml;
}

void add_object(ObjectList* ol, GameObject go){
	if (ol->size >= ol->capacity){
		ol->capacity*=2;
		ol->array=(GameObject*)realloc(ol->array,sizeof(GameObject)*ol->capacity);
	}
	ol->array[ol->size++]=go;
}

void setbit(MovementList* ml, int which, bool what){
	
	int byte=which/8;
	int bit=which-byte*8;

	if (which >= ml->capacity){	
		ml->capacity*=2;
		ml->bitarray=(char*)realloc(ol->bitarray,sizeof(char)*8*ml->capacity);
	}
	if (what) ml->bitarray[byte]  |= 1<<bit;
	else ml->bitarray[byte]  &= ~(1<<bit);
	}
}

bool getbit(MovementList* ml, bool which){
	int byte=which/8;
	int bit=which-byte*8;
	return ml->bitarray[byte] & 1<<bit;
}



int main( int argc, char *args[]){
	MovementList ml = movementList(8);

	printf("%i\n",rand());
}
