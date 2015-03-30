#include <stdio.h>
#include <stdbool.h>
#include "graphics.h"

//Board dimensions
#define MAXX 8
#define MAXY 8

//Size constants
#define SQSZ 50
#define PSZ 40
#define PILLSZ 20
#define PILLOFFSET (SQSZ - PILLSZ) / 2
#define OFFSET (SQSZ - PSZ) / 2
#define BORDERSZ 0

//Color constants
#define SCLR1 color(255, 0, 255)
#define SCLR2 color(0, 255, 255)
#define BACKCLR color(255,255,255)
#define PILLCLR color(0,0,255)
#define ENDCLR color(0,255,0)
#define WALLCLR color(0,0,0)
#define PORTCLR color(100,100,100)
#define PCLR  color(255, 255, 0)

//Key constants
#define UP 82
#define DOWN 81
#define LEFT 80
#define RIGHT 79

//Board Symbols
#define S 1
#define P 2
#define W 3
#define E 4
#define X 5
#define Y 6

int gridItems[8][8] = {
    {S, W, P, P, P, X, W, Y},
    {P, W, P, W, W, W, W, P},
    {P, W, P, P, P, P, W, P},
    {P, W, W, W, W, P, W, P},
    {P, P, P, P, W, P, W, P},
    {P, W, W, P, P, P, W, P},
    {P, W, W, W, W, W, W, P},
    {P, W, E, P, P, P, P, P}
};

//Gloabal Variables
int x, y;//coords of our player piece
int endy, endx; //location of final position
int row, col, key; //allocate some ints to hold for looping vars and key output
int pillsEaten = 0;
int endx, endy; //location of end sequence
int GRIDY = sizeof(gridItems) / sizeof(gridItems[0]); //calculate Y edge val
int GRIDX = sizeof(gridItems[0]) / sizeof(gridItems[0][0]); // calculate X edge val 

//function prototypes. Since I am defining all of them before I use them, I don't actually need these
bool move_player(int, int);
void interact();
void render_board();
bool process_input();
bool run_game();

/*
 * After ever move, a player has a chance to interact with whatever space they landed on
 *  this function manages this logic. At the moment that only includes pills and the 
 *  teleporter space
 */
void interact(){
    if(gridItems[y][x] == P){
        pillsEaten++;
        gridItems[y][x] = 0;
    }

    if(gridItems[y][x] == X){ //X represents the enterance to the portal, Y the exit
        int i, j;
        for(i=0; i<GRIDX; i++){
            for(j=0; j<GRIDY; j++){
                if(gridItems[j][i] == Y){
                    x = i;
                    y = j;
                }
            }
        }
    }
}

/*
 * Move the player to an arbitrary location on the board
 */
bool move_player(int newx, int newy){
    //check if space is on the board
    if(newx < 0 || newy < 0 || newy >= GRIDY || newx >= GRIDX){
        printf("That's off the board!\n");
        return false;
    }

    //test for wall collision
    if(gridItems[newy][newx] == W){
        printf("That's a wall!\n");
        return false;
    }

    //if passed muster, update player location
    x = newx;
    y = newy;

    interact();
    return true;
}

/*
 * Parse out the meaning of what was pressed on the keyboard
 *
 * a false return from this implies that the player wishes to terminate the game 
 */
bool process_input(){
    int key = getKeyB();

    if(key==-1) { 
        return false;
    }

    if(key == RIGHT) {
        move_player(x + 1, y);
    } else if(key == LEFT)        {
        move_player(x - 1, y);
    } else if(key == DOWN) {
        move_player(x, y + 1);
    } else if(key == UP)        {
        move_player(x, y - 1);
    }

    return true;
}

//A false return indicates premature exite, a true indicates the player completed the level
bool run_game(){
    render_board();

    while (process_input()) {
        render_board();
        
        if(x == endx && y == endy)
            return true;
    }
    return false;

}

void render_board(){
    clear_graphics();
    Color c;

    //nested loops for drawing the NxN grid
    for(row = 0; row < GRIDX; row++) {
        for(col = 0; col < GRIDY; col++) {
            if(gridItems[row][col] == W) {
                c = WALLCLR;
            } else if(row == endy && col == endx) {
                c = ENDCLR;
            } else if( gridItems[row][col] == X || gridItems[row][col] == Y){
                c = PORTCLR;
            } else if( (row + col) % 2 == 0) {
                c = SCLR1;
            } else {
                c = SCLR2;
            }
            drawRect(c, (col * SQSZ) + BORDERSZ,
                     (row * SQSZ) + BORDERSZ,
                     SQSZ, SQSZ);
            if(gridItems[row][col] == 2) {
                drawRect(PILLCLR, (col * SQSZ) + BORDERSZ + PILLOFFSET,
                         (row * SQSZ) + BORDERSZ + PILLOFFSET,
                         PILLSZ, PILLSZ);

            }
        }
    }

    drawRect(PCLR, BORDERSZ + x * SQSZ,
             BORDERSZ + y * SQSZ,
             SQSZ, SQSZ);

    render_graphics();

}


int main(int argc, char *args[]) {

    //make the grid large enough for our board plus a boarder
    init_graphics(50, 50, (2*BORDERSZ) + (GRIDX * SQSZ),
                  (2*BORDERSZ) + (GRIDY * SQSZ),
                  BACKCLR);


    //initialize starting space
    for(row = 0; row < GRIDY; row++) {
        for(col = 0; col < GRIDX; col++) {
            if(gridItems[row][col] == S) {
                x = row;
                y = col;
            } else if(gridItems[row][col] == E) {
                endy = row;
                endx = col;
            }
        }
    }


    if(run_game())
        printf("You ate %i pills!\n", pillsEaten);

    printf("Closing program...\n");
    obliterate_graphics();

    return 0;
}
