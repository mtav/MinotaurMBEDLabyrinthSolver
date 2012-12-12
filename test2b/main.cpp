#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#define MAZE_WIDTH 5
#define MAZE_HEIGHT 3

const char maze[MAZE_HEIGHT][MAZE_WIDTH] = {
                                    {2,3,0,0,1},
                                    {1,1,0,0,1},
                                    {0,0,0,0,1}
                                    };

void printMaze( const char maze_in[][MAZE_WIDTH])
{
    for(int y = 0; y < MAZE_HEIGHT; y++){
      for (int x = 0; x < MAZE_WIDTH; x++){
        cout<<int(maze_in[y][x]);
      }
      cout << endl;
    }
}

int main(void){
	printMaze(maze);
    return(0);
}
