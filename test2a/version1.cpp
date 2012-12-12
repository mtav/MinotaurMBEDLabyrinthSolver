// #include "mbed.h"
#include <cmath>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
using namespace std;

#define MAZE_WIDTH 50
#define MAZE_HEIGHT 28

// DigitalOut myled(LED1);
// Serial pc(USBTX,USBRX);
// Timer timer;

struct SolvePath{
    unsigned short gScore;
    unsigned short fScore;
    unsigned short hScore;
    char used;
    char setValues;
    char parentX;
    char parentY;
};

//void GetSmallestFscore(SolvePath** path, int cPointX, int cPointY);
//void CalculateSurroundingSquareValues(SolvePath** path, int cPointX, int cPointY, int endPointX, int endPointY);

//SolvePath path[MAZE_HEIGHT][MAZE_WIDTH];

/*char maze[MAZE_HEIGHT][MAZE_WIDTH] = {
                                    {2,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0},
                                    {0,1,1,0,1,1,1,1,0,1,1,1,1,0,1,0,1,0,1,0},
                                    {0,1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,1,0,1,0},
                                    {0,1,0,0,1,0,1,1,1,1,0,1,1,1,1,0,1,0,0,0},
                                    {0,1,0,1,1,0,1,0,0,0,0,1,0,0,0,0,1,1,1,0},
                                    {0,1,0,0,1,0,1,0,1,1,1,1,0,1,1,1,1,0,1,0},
                                    {0,1,1,0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0},
                                    {0,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1},
                                    {0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0},
                                    {1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,0},
                                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
                                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                    {0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
                                    };
                                    */

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

char **createMaze()
{
    char **maze_var=new char*[MAZE_HEIGHT];
    for (int i=0; i<MAZE_HEIGHT; i++)
    {
       maze_var[i] = new char[MAZE_WIDTH];
       for (int j=0; j<MAZE_WIDTH; j++)
         maze_var[i][j]=(char)0;
    }

    return maze_var;
}

void CalculateSurroundingSquareValues(SolvePath** path, int cPointX, int cPointY, int endPointX, int endPointY){
      cout<<cPointX<<" ";
      cout<<cPointY<<" ";
      cout<<endPointX<<" ";
      cout<<endPointY<<endl;
    
    if (cPointX < MAZE_WIDTH -1){
        if ((maze[cPointY + 1][cPointX] != 1) && (path[cPointY + 1][cPointX].setValues == 0)){
            path[cPointY + 1][cPointX].gScore = path[cPointY][cPointX].gScore + 1;
            path[cPointY + 1][cPointX].hScore = (int)sqrt((double)(abs(cPointX - endPointX) + abs(cPointY - endPointY)));
            path[cPointY + 1][cPointX].fScore = path[cPointY + 1][cPointX].gScore + path[cPointY + 1][cPointX].hScore;
            path[cPointY + 1][cPointX].setValues = 1;
            path[cPointY + 1][cPointX].parentX = cPointX;
            path[cPointY + 1][cPointX].parentY = cPointY;
        }
    }
    if (cPointX > 0){
        if ((maze[cPointY - 1][cPointX] != 1) && (path[cPointY - 1][cPointX].setValues == 0)){
            path[cPointY - 1][cPointX].gScore = path[cPointX][cPointY].gScore + 1;
            path[cPointY - 1][cPointX].hScore = (int)sqrt((double)(abs(cPointX - endPointX) + abs(cPointY - endPointY)));
            path[cPointY - 1][cPointX].fScore = path[cPointY - 1][cPointX].gScore + path[cPointY - 1][cPointX].hScore;
            path[cPointY - 1][cPointX].setValues = 1;
            path[cPointY - 1][cPointX].parentX = cPointX;
            path[cPointY - 1][cPointX].parentY = cPointY;
        }
    }
    if (cPointY < MAZE_HEIGHT -1){
        if ((maze[cPointY][cPointX + 1] != 1) && (path[cPointY][cPointX + 1].setValues == 0)){
            path[cPointY][cPointX + 1].gScore = path[cPointY][cPointX].gScore + 1;
            path[cPointY][cPointX + 1].hScore = (int)sqrt((double)(abs(cPointX - endPointX) + abs(cPointY - endPointY)));
            path[cPointY][cPointX + 1].fScore = path[cPointY][cPointX + 1].gScore + path[cPointY][cPointX + 1].hScore;
            path[cPointY][cPointX + 1].setValues = 1;
            path[cPointY][cPointX + 1].parentX = cPointX;
            path[cPointY][cPointX + 1].parentY = cPointY;
        }
    }
    if (cPointY > 0){
        if ((maze[cPointY][cPointX - 1] != 1) && (path[cPointY][cPointX - 1].setValues == 0)){
            path[cPointY][cPointX - 1].gScore = path[cPointY][cPointX].gScore + 1;
            path[cPointY][cPointX - 1].hScore = (int)sqrt((double)(abs(cPointX - endPointX) + abs(cPointY - endPointY)));
            path[cPointY][cPointX - 1].fScore = path[cPointY][cPointX - 1].gScore + path[cPointY][cPointX - 1].hScore;
            path[cPointY][cPointX - 1].setValues = 1;
            path[cPointY][cPointX - 1].parentX = cPointX;
            path[cPointY][cPointX - 1].parentY = cPointY;
        }
    }
}

void GetSmallestFscore(SolvePath** path, int & cPointX, int & cPointY){
    int smallestX = 0;
    int smallestY = 0;
    int smallHval = MAZE_WIDTH + MAZE_HEIGHT;
    path[cPointY][cPointX].used = 1;
    for(int x = 0; x < MAZE_WIDTH; x++){
        for(int y = 0; y < MAZE_HEIGHT; y++){
            if ((path[y][x].setValues == 1) && (path[y][x].used == 0)){
                if (path[y][x].hScore <= smallHval){
                    smallestX = x;
                    smallestY = y;
                    smallHval = path[y][x].hScore;
                }
            }
        }
    }
    cPointX = smallestX;
    cPointY = smallestY;
    //cPointX = 45;
    //cPointY = 26;
}

int main(void){

//    path = createMaze();
  
//    pc.baud(115200);
//     timer.start();
//     int beginTime, endTime;
//     beginTime = timer.read_us();

    // Initialize start and end points to unknown
    //signed char startX = -1;
    //signed char startY = -1;
    //signed char endX = -1;
    //signed char endY = -1;

    int startX = -1;
    int startY = -1;
    int endX = -1;
    int endY = -1;

    //Find start and end points
    char foundPoints = 0;
    for (int x = 0; x < MAZE_WIDTH; x++){
        for(int y = 0; y < MAZE_HEIGHT; y++){
            if(maze[y][x] == 2){
                startX = x;
                startY = y;
            }else if (maze[y][x] == 3){
                endX = x;
                endY = y;
            }
            if ((startX != -1) && (endX != -1)){
                foundPoints = 1;
                break;
            }
        }
        if (foundPoints == 1)
            break;
    }

    //initialize the path
    SolvePath **path = new SolvePath*[MAZE_HEIGHT];
    for(int y = 0; y < MAZE_HEIGHT; y++){
       path[y] = new SolvePath[MAZE_WIDTH];
        for (int x = 0; x < MAZE_WIDTH; x++){
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
    cout<< "start:"<<cPointX <<" "<< endX <<" "<< cPointY <<" "<< endY <<endl;
    while ((cPointX != endX) || (cPointY != endY)){
        cout<<"pre-CalculateSurroundingSquareValues"<<endl;
        CalculateSurroundingSquareValues(path, cPointX, cPointY, endX, endY);
        cout<<"post-CalculateSurroundingSquareValues"<<endl;
        GetSmallestFscore(path, cPointX, cPointY);
        cout<<"post-GetSmallestFscore"<<endl;
        debugCount++;
       // pc.printf("debug count = %d\n",debugCount);
    }
    
//     endTime = timer.read_us();

    cout<<"graphic solution"<<endl;
    //graphic solution
    char **mazeSol = createMaze();
    //char mazeSol[MAZE_HEIGHT][MAZE_WIDTH] = {0};
    int tempX = endX;
    int tempY = endY;
    while ((path[tempY][tempX].parentX != tempX) ||(path[tempY][tempX].parentY != tempY)){
        mazeSol[tempY][tempX] = 1;
        int tempNum = tempY;
        tempX = path[tempY][tempX].parentX;
        tempY = path[tempNum][tempX].parentY;
    }

    cout<<"Output maze and solution to console"<<endl;
    //Output maze and solution to console
    for (int y = 0; y < MAZE_HEIGHT; y++){
        for(int x = 0; x < MAZE_WIDTH; x++){
            //pc.printf("%d",maze[y][x]);
            printf("%d",maze[y][x]);
        }
        //pc.printf("\n");
        printf("\n");
    }
    //pc.printf("\n");
    printf("\n");
    for (int y = 0; y < MAZE_HEIGHT; y++){
        for(int x = 0; x < MAZE_WIDTH; x++){
            //pc.printf("%d",mazeSol[y][x]);
            printf("%d",mazeSol[y][x]);
        }
        //pc.printf("\n");
        printf("\n");
    }
    
    //pc.printf("\n\nSolved the maze in %dus", endTime - beginTime);

//     while(1){
//         myled = 1;
//         wait(1);
//         myled = 0;
//         wait(1);
//     }

    cout<<"THE END"<<endl;
    return(0);
}
