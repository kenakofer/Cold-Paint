#include <stdio.h>
#include "graphics.h"
#include <time.h>
#include <stdlib.h> //this is where rand lives

#define CELL_SIZE 40 
#define X_DIM 15
#define Y_DIM 15
#define WALL 0
#define FLOOR 1
#define PELLET 2
#define HERO 3
#define END 4

#define HERO_COLOR color(0,255,35)
#define WALL_COLOR color(0,0,0)
#define FLOOR_COLOR color(255,0,0)
#define PELLET_COLOR color(255,0,255)
#define END_COLOR color(255,255,255)

int map[Y_DIM][X_DIM]={[1][1]=1};
/*		{0,0,0,0,0,0,0,0,0,0,0},
		{0,1,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0}
	};
*/
int isOpen(int x, int y){
	return(y<Y_DIM && y>=0 && x<X_DIM && x>=0 && map[y][x]);
}
int isWall(int x, int y){
	return(y<Y_DIM && y>=0 && x<X_DIM && x>=0 && !map[y][x]);
}

int recursiveBuild(int x, int y){
	int r = rand()%4;
	for (int i=0;i<4;i++){
		switch ((r+i)%4){
			case 0:
				if (isWall(x+2,y)) {
					map[y][x+1]=1; map[y][x+2]=2;
					recursiveBuild(x+2,y);
				} break;
			case 1:
				if (isWall(x-2,y)) {
					map[y][x-1]=1; map[y][x-2]=2;
					recursiveBuild(x-2,y);
				} break;
			case 2:
				if (isWall(x,y+2)) {
					map[y+1][x]=1; map[y+2][x]=2;
					recursiveBuild(x,y+2);
				} break;
			case 3:
				if (isWall(x,y-2)) {
					map[y-1][x]=1; map[y-2][x]=2;
					recursiveBuild(x,y-2);
				} break;
		}
	}
}

int main(int argc, char *argv[]){
    printf("Start test...\n");
   
    int key;

	//Create the maze
	//
	srand (time(NULL));
	recursiveBuild(1,1);
	map[Y_DIM-2][X_DIM-2]=END;

	int score=0;




	int herox=1;
	int heroy=1;
	map[heroy][herox]=HERO;

    init_graphics(150, 150, X_DIM*CELL_SIZE, Y_DIM*CELL_SIZE, color(255,255,255));
    int quit = 0;
	Color colors[5]={WALL_COLOR,FLOOR_COLOR,PELLET_COLOR,HERO_COLOR,END_COLOR};

    while (!quit) {
        clear_graphics();

		for(int i=0; i<X_DIM; i++){
			for (int j=0; j<Y_DIM; j++){
				drawRect(colors[map[j][i]],CELL_SIZE*i,CELL_SIZE*j,CELL_SIZE,CELL_SIZE);
			}
		}
                            

		render_graphics();

		key = getKeyB();
//      printf("Key Pressed: %i\n", key);
        if(key == -1 ){
            quit = 1;
        }
		else if (key == 79){ //RIGHT
			if (map[heroy][herox+1]!=WALL){
				map[heroy][herox]=1;
				herox++;
			}
		}
		else if (key == 80){ //LEFT
			
			if (map[heroy][herox-1]!=WALL){
				map[heroy][herox]=1;
				herox--;
			}
		}
		else if (key == 81){ //DOWN
			
			if (map[heroy+1][herox]!=WALL){
				map[heroy][herox]=1;
				heroy++;
			}
		}
		else if (key == 82){ //UP
			
			if (map[heroy-1][herox]!=WALL){
				map[heroy][herox]=1;
				heroy--;
			}
		}
		switch (map[heroy][herox]){
			case PELLET:
				score++;
				break;
			case END:
				quit=1;
				break;
		}
		map[heroy][herox]=HERO;
    }

    printf("Pellets collected: %i\n",score);
    obliterate_graphics();

    return 0;
}
