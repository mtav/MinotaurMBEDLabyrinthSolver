//#include "mbed.h"
//#include "Servo.h"

#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#define MAZE_WIDTH 16
#define MAZE_HEIGHT 13
#define MAZE_LEFT 1
#define MAZE_RIGHT 2
#define MAZE_UP 3
#define MAZE_DOWN 4


void GetSmallestFscore(int * cPointX, int * cPointY);
void CalculateSurroundingSquareValues(int cPointX, int cPointY, int endPointX, int endPointY);
void zeroMaze();

//InterruptIn buttonPressed(p6);
//DigitalIn zeroButton(p6);
//DigitalOut myled(LED1);
//Serial pc(USBTX,USBRX);
//Servo xServo(p21);
//Servo yServo(p22);
//Timer timer;

struct SolvePath {
    unsigned short gScore;
    unsigned short fScore;
    unsigned short hScore;
    char used;
    char setValues;
    char parentX;
    char parentY;
};

SolvePath path[MAZE_HEIGHT][MAZE_WIDTH];

char maze[MAZE_HEIGHT][MAZE_WIDTH] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,3,1,0,0,0,0,1,1,1,1,0,0,0,1,1},
    {1,0,1,0,1,1,1,1,0,0,0,0,0,0,1,1},
    {1,0,0,0,0,0,1,1,0,1,1,0,1,0,0,1},
    {1,1,1,1,1,0,1,1,0,0,1,0,1,0,1,1},
    {1,0,1,0,0,0,1,1,1,0,1,0,1,0,0,1},
    {1,0,1,0,1,1,0,0,1,0,1,1,1,0,1,1},
    {1,0,0,0,0,1,0,1,1,0,1,1,0,0,0,1},
    {1,0,1,0,1,1,0,1,0,0,0,1,0,1,1,1},
    {1,0,1,0,0,0,0,0,0,0,1,1,0,0,0,1},
    {1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,1},
    {1,0,0,1,0,0,0,0,0,0,0,1,0,0,2,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};


/*
char maze[MAZE_HEIGHT][MAZE_WIDTH] = {
                                    {2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                    {0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
                                    {0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
                                    {0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0},
                                    {0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1,0},
                                    {0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,0,1,0,1,0},
                                    {0,1,0,1,0,1,0,1,0,0,0,0,0,0,1,0,1,0,1,0},
                                    {0,1,0,1,0,1,0,1,0,1,1,1,1,0,1,0,1,0,1,0},
                                    {0,1,0,1,0,1,0,1,0,1,3,1,1,0,1,0,1,0,1,0},
                                    {0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0},
                                    {0,1,0,1,0,1,0,1,0,1,1,0,1,0,1,0,1,0,1,0},
                                    {0,1,0,1,0,1,0,1,0,0,0,0,1,0,1,0,1,0,1,0},
                                    {0,1,0,1,0,1,0,1,1,1,1,1,1,0,1,0,1,0,1,0},
                                    {0,1,0,1,0,1,0,0,0,0,0,0,0,0,1,0,1,0,1,0},
                                    {0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0},
                                    {0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0},
                                    {0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0},
                                    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
                                    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
                                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
                                    };

*/
/*
const char maze[MAZE_HEIGHT][MAZE_WIDTH] = {
{1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,1,0,0,2,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1},
{1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1},
{1,1,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,1},
{1,1,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,1},
{1,1,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,1},
{1,1,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,1,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1},
{1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1},
{1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,0,1,1,1},
{1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,0,1,1,1},
{1,1,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,1},
{1,1,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1},
{1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,1,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1},
{1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1},
{1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1},
{1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,0,1},
{1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,1},
{1,1,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,1},
{1,1,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1},
{1,1,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1},
{1,1,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0,1},
{1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1},
{1,1,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,1},
{1,1,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,1},
{1,1,1,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,3,1,1,1,1},
{1,1,1,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1},
};
*/

int main(void)
{

    float range = 0.0009;
    //xServo.calibrate(range, 45.0);
    //yServo.calibrate(range, 45.0);
    //pc.baud(115200);
    //timer.start();
    //int beginTime, endTime;
    //beginTime = timer.read_us();
    
    //buttonPressed.fall(&zeroMaze);

    //xServo = 0.5;
    //yServo = 0.5;

    int count = 0;
    int limit = MAZE_WIDTH * MAZE_HEIGHT;
    //while(count < limit) {
        //char c = pc.getc() - '0';
        //maze[count / MAZE_WIDTH][count % MAZE_WIDTH] = c;
        //pc.putc(c);
        //count++;
    //}
    printf("Solving maze\n");


    // Initialize start and end points to unknown
    signed char startX = -1;
    signed char startY = -1;
    signed char endX = -1;
    signed char endY = -1;

    //Find start and end points
    char foundPoints = 0;
    for (int x = 0; x < MAZE_WIDTH; x++) {
        for(int y = 0; y < MAZE_HEIGHT; y++) {
            if(maze[y][x] == 2) {
                startX = x;
                startY = y;
            } else if (maze[y][x] == 3) {
                endX = x;
                endY = y;
            }
            if ((startX != -1) && (endX != -1)) {
                foundPoints = 1;
                break;
            }
        }
        if (foundPoints == 1)
            break;
    }

    //initialize the path
    for (int x = 0; x < MAZE_WIDTH; x++) {
        for(int y = 0; y < MAZE_HEIGHT; y++) {
            path[y][x].fScore = 0;
            path[y][x].gScore = 0;
            path[y][x].hScore = 0;
            path[y][x].parentX = 0;
            path[y][x].parentY = 0;
            path[y][x].setValues = 0;
            path[y][x].used = 0;
        }
    }

    //The current point in the maze
    int cPointX = startX;
    int cPointY = startY;

    //Start the path
    path[cPointY][cPointX].used = 1;
    path[cPointY][cPointX].setValues = 1;
    path[cPointY][cPointX].parentX = cPointX;
    path[cPointY][cPointX].parentY = cPointY;

    //Solve the maze
    int debugCount = 0;
    while ((cPointX != endX) || (cPointY != endY)) {
        CalculateSurroundingSquareValues(cPointX, cPointY, endX, endY);
        GetSmallestFscore(&cPointX, &cPointY);
        debugCount++;
        // printf("debug count = %d\n",debugCount);
    }

    //endTime = timer.read_us();

    //graphic solution
    char mazeSol[MAZE_HEIGHT][MAZE_WIDTH] = {0};
    int tempX = endX;
    int tempY = endY;
    while ((path[tempY][tempX].parentX != tempX) ||(path[tempY][tempX].parentY != tempY)) {
        mazeSol[tempY][tempX] = 1;
        int tempNum = tempY;
        tempY = path[tempY][tempX].parentY;
        tempX = path[tempNum][tempX].parentX;
    }

    //Output maze and solution to console
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for(int x = 0; x < MAZE_WIDTH; x++) {
            printf("%d",maze[y][x]);
        }
        printf("\n");
    }
    printf("\n");
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for(int x = 0; x < MAZE_WIDTH; x++) {
            printf("%d",mazeSol[y][x]);
        }
        printf("\n");
    }

    //printf("\n\nSolved the maze in %dus", endTime - beginTime);

    tempX = endX;
    tempY = endY;
    char direction = 0;
    char newDir= 0;
    while ((path[tempY][tempX].parentX != tempX) ||(path[tempY][tempX].parentY != tempY)) {

        if (path[tempY][tempX].parentY > tempY) {
            newDir = MAZE_RIGHT;
        } else if (path[tempY][tempX].parentY < tempY) {
            newDir = MAZE_LEFT;
        } else if(path[tempY][tempX].parentX > tempX) {
            newDir = MAZE_UP;
        } else if (path[tempY][tempX].parentX < tempX) {
            newDir = MAZE_DOWN;
        }

        char tempX2 = path[tempY][tempX].parentX;
        char tempY2 = path[tempY][tempX].parentY;
        char nextDir = newDir;
        while ((newDir == nextDir) && (maze[tempY2][tempX2] != 2)) {
            if (path[tempY2][tempX2].parentY > tempY2) {
                nextDir = MAZE_RIGHT;
            } else if (path[tempY2][tempX2].parentY < tempY2) {
                nextDir = MAZE_LEFT;
            } else if(path[tempY2][tempX2].parentX > tempX2) {
                nextDir = MAZE_UP;
            } else if (path[tempY2][tempX2].parentX < tempX2) {
                nextDir = MAZE_DOWN;
            }
            int tempNum = tempY2;
            tempY2 = path[tempY2][tempX2].parentY;
            tempX2 = path[tempNum][tempX2].parentX;
        }

        if (newDir != direction) {
            //xServo = 0.5;
            //yServo = 0.5;
            direction = newDir;
            if (direction == MAZE_UP) {
                //yServo = 1;
                printf("UP\n");
            } else if (direction == MAZE_LEFT) {
                printf("LEFT\n");
                //xServo = 1;
            } else if (direction == MAZE_DOWN) {
                printf("DOWN\n");
                //yServo = 0;
            } else if (direction == MAZE_RIGHT) {
                printf("RIGHT\n");
                //xServo = 0;
            }
            if ( nextDir != newDir) {
                if (nextDir == MAZE_UP) {
                    //yServo = 0.25;
                } else if (nextDir == MAZE_LEFT) {
                    //xServo = 0.25;
                } else if (nextDir == MAZE_DOWN) {
                    //yServo = 0.75;
                } else if (nextDir == MAZE_RIGHT) {
                    //xServo = 0.75;
                }
            }
        //wait(1.0);
        }
        //wait(0.2);
        int tempNum = tempY;
        tempY = path[tempY][tempX].parentY;
        tempX = path[tempNum][tempX].parentX;

    }




    //while(1) {
        //myled = 1;
        //wait(1);
        //myled = 0;
        //wait(1);
    //}

}

void CalculateSurroundingSquareValues(int cPointX, int cPointY, int endPointX, int endPointY)
{
    if (cPointY < MAZE_HEIGHT -1) {
        if ((maze[cPointY + 1][cPointX] != 1) && (path[cPointY + 1][cPointX].setValues == 0)) {
            path[cPointY + 1][cPointX].gScore = path[cPointY][cPointX].gScore + 1;
            path[cPointY + 1][cPointX].hScore = (int)sqrt((double)(abs(cPointX - endPointX) + abs(cPointY - endPointY)));
            path[cPointY + 1][cPointX].fScore = path[cPointY + 1][cPointX].gScore + path[cPointY + 1][cPointX].hScore;
            path[cPointY + 1][cPointX].setValues = 1;
            path[cPointY + 1][cPointX].parentX = cPointX;
            path[cPointY + 1][cPointX].parentY = cPointY;
        }
    }
    if (cPointY > 0) {
        if ((maze[cPointY - 1][cPointX] != 1) && (path[cPointY - 1][cPointX].setValues == 0)) {
            path[cPointY - 1][cPointX].gScore = path[cPointX][cPointY].gScore + 1;
            path[cPointY - 1][cPointX].hScore = (int)sqrt((double)(abs(cPointX - endPointX) + abs(cPointY - endPointY)));
            path[cPointY - 1][cPointX].fScore = path[cPointY - 1][cPointX].gScore + path[cPointY - 1][cPointX].hScore;
            path[cPointY - 1][cPointX].setValues = 1;
            path[cPointY - 1][cPointX].parentX = cPointX;
            path[cPointY - 1][cPointX].parentY = cPointY;
        }
    }
    if (cPointX < MAZE_WIDTH -1) {
        if ((maze[cPointY][cPointX + 1] != 1) && (path[cPointY][cPointX + 1].setValues == 0)) {
            path[cPointY][cPointX + 1].gScore = path[cPointY][cPointX].gScore + 1;
            path[cPointY][cPointX + 1].hScore = (int)sqrt((double)(abs(cPointX - endPointX) + abs(cPointY - endPointY)));
            path[cPointY][cPointX + 1].fScore = path[cPointY][cPointX + 1].gScore + path[cPointY][cPointX + 1].hScore;
            path[cPointY][cPointX + 1].setValues = 1;
            path[cPointY][cPointX + 1].parentX = cPointX;
            path[cPointY][cPointX + 1].parentY = cPointY;
        }
    }
    if (cPointX > 0) {
        if ((maze[cPointY][cPointX - 1] != 1) && (path[cPointY][cPointX - 1].setValues == 0)) {
            path[cPointY][cPointX - 1].gScore = path[cPointY][cPointX].gScore + 1;
            path[cPointY][cPointX - 1].hScore = (int)sqrt((double)(abs(cPointX - endPointX) + abs(cPointY - endPointY)));
            path[cPointY][cPointX - 1].fScore = path[cPointY][cPointX - 1].gScore + path[cPointY][cPointX - 1].hScore;
            path[cPointY][cPointX - 1].setValues = 1;
            path[cPointY][cPointX - 1].parentX = cPointX;
            path[cPointY][cPointX - 1].parentY = cPointY;
        }
    }
}

void GetSmallestFscore(int * cPointX, int * cPointY)
{
    int smallestX;
    int smallestY;
    int smallHval = MAZE_WIDTH + MAZE_HEIGHT;
    path[*cPointY][*cPointX].used = 1;
    for(int x = 0; x < MAZE_WIDTH; x++) {
        for(int y = 0; y < MAZE_HEIGHT; y++) {
            if ((path[y][x].setValues == 1) && (path[y][x].used == 0)) {
                if (path[y][x].hScore <= smallHval) {
                    smallestX = x;
                    smallestY = y;
                    smallHval = path[y][x].hScore;
                }
            }
        }
    }
    *cPointX = smallestX;
    *cPointY = smallestY;
}

void zeroMaze(){
    //xServo = 0.5;
    //yServo = 0.5;
}
