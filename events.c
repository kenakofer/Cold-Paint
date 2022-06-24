#include "graphics.h"
#include "structs.h"
#include "objects.h"
#include "bitoperations.h"

#include <stdbool.h>

#define ARROWS 0
#define WASD 1

#define UP_K 82
#define DOWN_K 81
#define LEFT_K 80
#define RIGHT_K 79

#define W_K 26
#define S_K 22
#define A_K 4
#define D_K 7

#define F_K 9
#define P_K 19

typedef enum {UP, DOWN, LEFT, RIGHT} directions;

int controls[2][4]={{UP_K,DOWN_K,LEFT_K,RIGHT_K}, {W_K,S_K,A_K,D_K}};

bool up_key;
bool down_key;
bool left_key;
bool right_key;

bool W_key;
bool A_key;
bool S_key;
bool D_key;

bool P_key;
bool F_key;

bool keys[10][4]={false};

int arrows(){
	return ARROWS;
}
int wasd(){
	return WASD;
}
/*bool up(int i){
//	return keys[i][UP];
	if (go->control==WASD) return W_key;
	return up_key;
}
bool down(int i){
	if (go->control==WASD) return S_key;
	return down_key;
}
bool left(GameObject* go){
	if (go->control==WASD) return A_key;
	return left_key;
}
bool right(GameObject* go){
	if (go->control==WASD) return D_key;
	return right_key;
}*/

bool up(int i)   { return keys[i][UP]; }
bool down(int i) { return keys[i][DOWN]; }
bool left(int i) { return keys[i][LEFT]; }
bool right(int i){ return keys[i][RIGHT]; }

bool F(){return F_key;}
bool P(){return P_key;}

int fetch_movements(MovementList* ml, int pen_num, int which_frame){
	for (int i=0;i<pen_num;i++){
		for (int dir=0; dir<4; dir++){
			keys[i][dir]=getbit(ml,which_frame*pen_num*4 + i*4 + dir);
		}
	}
}

int handleEvents(){
	SDL_Event e;
	while (SDL_PollEvent(&e) !=0){
		if (e.type == SDL_QUIT){
			return -1;
		}

		if (e.type == SDL_KEYDOWN){
			for (int i=0; i<2; i++) for (int j=0; j<4; j++){
				if (e.key.keysym.scancode==controls[i][j])  keys[i][j]=true;
			}
			switch (e.key.keysym.scancode){
		/*		case 79:
					right_key=true;
					break;
				case 80:
					left_key=true;
					break;
				case 81:
					down_key=true;
					break;
				case 82:
					up_key=true;
					break;
				case 26:
					W_key=true;
					break;
				case 4:
					A_key=true;
					break;
				case 22:
					S_key=true;
					break;
				case 7:
					D_key=true;
					break;*/
				case 9:
					F_key=true;
					break;
				case 19:
					P_key=true;
					break;
			}
		}
		if (e.type == SDL_KEYUP){
			for (int i=0; i<2; i++) for (int j=0; j<4; j++){
				if (e.key.keysym.scancode==controls[i][j])  keys[i][j]=false;
			}
			switch (e.key.keysym.scancode){
			/*	case 79:
					right_key=false;
					break;
				case 80:
					left_key=false;
					break;
				case 81:
					down_key=false;
					break;
				case 82:
					up_key=false;
					break;
				case 26:
					W_key=false;
					break;
				case 4:
					A_key=false;
					break;
				case 22:
					S_key=false;
					break;
				case 7:
					D_key=false;
					break;*/
				case 9:
					F_key=false;
					break;
				case 19:
					P_key=false;
					break;
			}
		}
	}
}
