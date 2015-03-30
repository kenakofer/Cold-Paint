#include "graphics.h"
#include <stdbool.h>
#include "objects.h"

#define ARROWS 0
#define WASD 1

bool up_key;
bool down_key;
bool left_key;
bool right_key;

bool W_key;
bool A_key;
bool S_key;
bool D_key;

int arrows(){
	return ARROWS;
}
int wasd(){
	return WASD;
}

bool up(GameObject* go){
	if (go->control==WASD) return W_key;
	return up_key;
}
bool down(GameObject* go){
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
}

int handleEvents(){
	SDL_Event e;
	while (SDL_PollEvent(&e) !=0){
		if (e.type == SDL_QUIT){
			return -1;
		}

		if (e.type == SDL_KEYDOWN){
			switch (e.key.keysym.scancode){
				case 79:
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
					break;
			}
			//printf("%i\n",e.key.keysym.scancode);
		}
		if (e.type == SDL_KEYUP){
			switch (e.key.keysym.scancode){
				case 79:
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
					break;
			}
		}
	}
}
