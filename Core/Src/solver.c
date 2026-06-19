/*
 * solver.c
 */

#include "main.h"
#include "solver.h"
#include "irs.h"
#include "controller.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


// Replace API functions with physical implementations
static int API_mazeHeight(void){
	return 16;
}
static int API_mazeWidth(void){
	return 16;
}

static int API_wallFront(void){
	return readFrontLeftIR() > 2000 || readFrontRightIR() > 2000;
}
static int API_wallLeft(void){
	return readLeftIR() > 2000;
}
static int API_wallRight(void){
	return readRightIR() > 2000;
}

// Mock function for the simulator
static void API_setWall(int r, int c, char dir)   { (void)r; (void)c; (void)dir; }

//coord
typedef struct Coord
{
    int row;
    int col;
} Coord;

Coord newCoord(int row, int col){
    Coord c;
    c.row = row;
    c.col = col;
    return c;
}

//linked list
typedef struct Node {
    struct Node* next;
    struct Coord pos;
} Node;

Node* createNode(Coord pos){
    Node* newNode = (Node*)malloc(sizeof(Node));
    if(newNode != NULL){
        newNode->pos = pos;
        newNode->next = NULL;
    }
    return newNode;
}

//queue
typedef struct queue {
    int size;
    Node* head;
    Node* tail;
} queue;

queue* createQueue(){
    queue* q = (queue*)malloc(sizeof(queue));
    if(q != NULL){
        q->size = 0;
        q->head = NULL;
        q->tail = NULL;
    }
    return q;
}

void queue_push(queue* q, Coord pos) {
    Node* newNode = createNode(pos);
    if(q->head == NULL){
        q->head = newNode;
        q->tail = newNode;
    }else{
        q->tail->next = newNode;
        q->tail = newNode;
    }
    q->size++;
}

Coord queue_pop(queue* q) {
    if(q->size > 0){
        Node* temp = q->head;
        Coord pos = temp->pos;
        q->head = q->head->next;

        if(q->head == NULL){
            q->tail = NULL;
        }

        free(temp);
        q->size--;
        return pos;
    }

    return newCoord(-1, -1);
}

Action solver() {
    return floodFill();
}

int maxRows;
int maxCols;
int** mazeVert = NULL;
int** mazeHorz = NULL;
int** dist = NULL;
int initialized = 0;
int centerLeft;
int centerTop;
int dir;
queue* q;
Coord currPos;

void calcDist(){
    //init manhantann distances to grid of -1
    for(int i =0; i < maxRows;i ++){
        memset(dist[i], -1, maxCols*sizeof(int));
    }

    //set goal dist to 0
    dist[centerLeft][centerTop] = 0;
    dist[centerLeft + 1][centerTop] = 0;
    dist[centerLeft][centerTop + 1] = 0;
    dist[centerLeft + 1][centerTop + 1] = 0;

    //reset queue
    while(q->size > 0){
        queue_pop(q);
    }
    queue_push(q, newCoord(centerLeft, centerTop));
    queue_push(q, newCoord(centerLeft+1, centerTop));
    queue_push(q, newCoord(centerLeft, centerTop+1));
    queue_push(q, newCoord(centerLeft+1, centerTop+1));

    while(q->size != 0){
        Coord curr = queue_pop(q);
        int r = curr.row;
        int c = curr.col;

        //north
        if(r >= 0 && r < maxRows && c+1 >= 0 && c+1 < maxCols){
            if(mazeHorz[r][c] != 2 && dist[r][c+1] == -1){
                dist[r][c+1] = dist[r][c] + 1;
                queue_push(q, newCoord(r, c+1));
            }
        }
        //east
        if(r+1 >= 0 && r+1 < maxRows && c >= 0 && c < maxCols){
            if(mazeVert[r+1][c] != 2 && dist[r+1][c] == -1){
                dist[r+1][c] = dist[r][c] + 1;
                queue_push(q, newCoord(r+1, c));
            }
        }
        //south
        if(r >= 0 && r < maxRows && c-1 >= 0 && c-1 < maxCols){
            if(mazeHorz[r][c-1] != 2 && dist[r][c-1] == -1){
                dist[r][c-1] = dist[r][c] + 1;
                queue_push(q, newCoord(r, c-1));
            }
        }
        //west
        if(r-1 >= 0 && r-1 < maxRows && c >= 0 && c < maxCols){
            if(mazeVert[r][c] != 2 && dist[r-1][c] == -1){
                dist[r-1][c] = dist[r][c] + 1;
                queue_push(q, newCoord(r-1, c));
            }
        }
    }

}

void init_floodFill(){
    maxRows = API_mazeHeight();
    maxCols = API_mazeWidth();

    //mazeVert array
    mazeVert = (int**)calloc(maxRows+1, sizeof(int*));
    for(int i = 0; i <= maxRows; i++){
        mazeVert[i] = (int*)calloc(maxCols, sizeof(int));
    }

    //mazeHorz array
    mazeHorz = (int**)calloc(maxRows+1, sizeof(int*));
    for(int i = 0; i <= maxRows; i++){
        mazeHorz[i] = (int*)calloc(maxCols, sizeof(int));
    }

    //manhattan dist
    dist = (int**)malloc(maxRows * sizeof(int*));
    for(int i = 0; i < maxRows; i++){
        dist[i] = (int*)malloc(maxCols * sizeof(int));
    }

    //set goal coords
    centerLeft = maxRows/2 - 1;
    centerTop = maxCols/2 - 1;

    q = createQueue();
    dir = 0;

    currPos = newCoord(0, 0);

    calcDist();
}

//DONE: fix //API_setWall(0, 0,'n');
void checkWalls(int r, int c){
    if(API_wallFront()){
        switch(dir){
            case 0: //mouse facing north
                mazeHorz[r][c] = 2;
                API_setWall(r, c,'n');
                break;
            case 1: //east
                mazeVert[r+1][c] = 2;
                API_setWall(r+1, c,'w');
                break;
            case 2: //south
                mazeHorz[r][c-1] = 2;
                API_setWall(r, c-1,'n');
                break;
            case 3: //west
                mazeVert[r][c] = 2;
                API_setWall(r, c,'w');
                break;

        }
    }
    if(API_wallLeft()){
        switch(dir){
            case 0: //mouse facing north
                mazeVert[r][c] = 2;
                API_setWall(r, c,'w');
                break;
            case 1: //east
                mazeHorz[r][c] = 2;
                API_setWall(r, c,'n');
                break;
            case 2: //south
                mazeVert[r+1][c] = 2;
                API_setWall(r+1, c,'w');
                break;
            case 3: //west
                mazeHorz[r][c-1] = 2;
                API_setWall(r, c-1,'n');
                break;

        }
    }
    if(API_wallRight()){
        switch(dir){
            case 0: //mouse facing north
                mazeVert[r+1][c] = 2;
                API_setWall(r+1, c,'w');
                break;
            case 1: //east
                mazeHorz[r][c-1] = 2;
                API_setWall(r, c-1,'n');
                break;
            case 2: //south
                mazeVert[r][c] = 2;
                API_setWall(r, c,'w');
                break;
            case 3: //west
                mazeHorz[r][c] = 2;
                API_setWall(r, c,'n');
                break;

        }
    }
}

int minDistDir(int r, int c){
    int minDist = 999;
    int minDir = 4;
    if(mazeHorz[r][c] != 2 && c < maxCols-1 && dist[r][c+1] < minDist){
        minDist = dist[r][c+1];
        minDir = 0;
    }
    if(mazeVert[r+1][c] != 2 && r < maxRows-1 && dist[r+1][c] < minDist){
        minDist = dist[r+1][c];
        minDir = 1;
    }
    if(mazeHorz[r][c-1] != 2 && c > 0 && dist[r][c-1] < minDist){
        minDist = dist[r][c-1];
        minDir = 2;
    }
    if(mazeVert[r][c] != 2 && r > 0 && dist[r-1][c] < minDist){
        minDist = dist[r-1][c];
        minDir = 3;
    }
    return minDir;
}

// Put your implementation of floodfill here!
Action floodFill() {
    if(!initialized){
        init_floodFill();
        initialized = 1;
    }

    int r = currPos.row;
    int c = currPos.col;

    //if goal reached
    if(dist[r][c] == 0){
        return IDLE;
    }

    //check for walls
    checkWalls(r,c);

    //calcualte manhattan distances
    calcDist();

    //DONE: movement
    int newDir = minDistDir(r, c);

    //DONE: make turn smarter
    if(newDir > 3){ //remove or handle better?
        return IDLE;
    }

    if(newDir != dir){
        int val = (dir - newDir + 4) % 4;
        if(val == 3){
            dir = (dir + 1)%4;
            return RIGHT;
        }else if(val == 1){
            dir = (dir - 1 + 4)%4;
            return LEFT;
        }else{
            dir = (dir + 1)%4;
            return RIGHT;
        }
    }else{
        if(!API_wallFront()){
            switch(dir){
                case 0: //north
                    currPos = newCoord(r, c+1);
                    break;
                case 1: //east
                    currPos = newCoord(r+1, c);
                    break;
                case 2: //south
                    currPos = newCoord(r, c-1);
                    break;
                case 3: //west
                    currPos = newCoord(r-1, c);
                    break;
            }
            return FORWARD;
        }

        return IDLE;

    }
}

