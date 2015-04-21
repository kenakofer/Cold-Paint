#include "structs.h"
#include <stdlib.h>

MovementList movementList(int c){
	MovementList ml;
	ml.size=0;
	ml.capacity=c;
	ml.bitarray=(char *)malloc(sizeof(char)*8*ml.capacity/8);
	return ml;
}

void setbit(MovementList* ml, int which, bool what){
	
	int byte=which/8;
	int bit=which-byte*8;

	while (which >= ml->capacity){	
		ml->capacity*=2;
		ml->bitarray=(char*)realloc(ml->bitarray,sizeof(char)*8*ml->capacity);
	}
	if (what) ml->bitarray[byte]  |= 1<<bit;
	else ml->bitarray[byte]  &= ~(1<<bit);

	if (which>=ml->size) ml->size=which+1;
}

bool getbit(MovementList* ml, int which){
	int byte=which/8;
	int bit=which-byte*8;
	if (ml->bitarray[byte] & 1<<bit) return true;
	else return false;
}


void write_init_data(char* filename, GameProperties* gp){
	gp->objects.size=0;

	FILE *f = fopen(filename, "wb");
	fwrite(gp, sizeof(GameProperties), 1, f);
	fclose(f);
}
void write_movement_data(char* filename, GameProperties* gp){
	FILE *f = fopen(filename, "ab");
	fwrite(&(gp->movements), sizeof(MovementList), 1, f);
	fwrite(gp->movements.bitarray, sizeof(char), gp->movements.size/8+1, f);
	fclose(f);
}
void write_all_data(char* filename, GameProperties* gp){
	FILE *f = fopen(filename, "wb");
	gp->objects.size=0;
	fwrite(gp, sizeof(GameProperties), 1, f);
	fwrite(gp->movements.bitarray, sizeof(char), gp->movements.size/8+1, f);
	fclose(f);
}

void read_data(char* filename, GameProperties* gp){
	FILE *f = fopen(filename, "rb");
	fread(gp, sizeof(GameProperties), 1, f);
	fread(&(gp->movements), sizeof(MovementList), 1, f);
	int size=gp->movements.size;
	gp->movements=movementList(gp->movements.size);
	gp->movements.size=size;
	fread(gp->movements.bitarray, sizeof(char), gp->movements.size/8+1, f);
	fclose(f);
}

