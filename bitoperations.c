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
//	printf("%i",ml->bitarray[byte] & 1<<bit);
	if (ml->bitarray[byte] & 1<<bit) return true;
	else return false;
}


/*GameProperties read_data(char* filename){
	GameProperties gp
}*/

void write_data(char* filename, GameProperties* gp){
	gp->objects.size=0;

	FILE *f = fopen(filename, "wb");
	fwrite(gp, sizeof(GameProperties), 1, f);
	fwrite(gp->movements.bitarray, sizeof(char), gp->movements.size, f);
	fclose(f);
}

void read_data(char* filename, GameProperties* gp){
	FILE *f = fopen(filename, "rb");
	fread(gp, sizeof(GameProperties), 1, f);
	gp->movements=movementList(gp->step/8+1);
	fread(gp->movements.bitarray, sizeof(char), gp->movements.size, f);
	fclose(f);
}

/*int main( int argc, char *args[]){
	srand(0);
	MovementList ml = movementList(8);
	for (int i=0;i<100;i++){
		setbit(&ml, i, !(rand()%2));
	}
	for (int i=0;i<ml.capacity;i++){
		setbit(&ml, i, getbit(&ml,i) && !(rand()%2));
	}
	for (int i=0;i<ml.capacity;i++){
		//printf("%i/n",i);
		//putchar(getbit(&ml, i));
		if(getbit(&ml, i)) putchar('1');
		else putchar('0');
	}

	printf("\n");
}*/
