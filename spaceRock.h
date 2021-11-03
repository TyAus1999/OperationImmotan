#pragma once
#include "inc.h"

typedef struct spaceRock{
    float x,y;
    float xVel, yVel;
    float rotZ;
    float rotAmount;
    float scaleX, scaleY;
    unsigned splitNumber;
    char alive;
} spaceRock;

typedef struct spaceRockArray{
    int len;
    int allocatedLength;
    spaceRock *start;
} spaceRockArray;

spaceRock makeSpaceRock(float x, float y, float xVel, float yVel, float rotAmount){
    spaceRock ret;
    ret.x=x;
    ret.y=y;
    ret.xVel=xVel;
    ret.yVel=yVel;
    ret.rotAmount=rotAmount;
    ret.rotZ=(float)(rand()%360);
    ret.scaleX=1;
    ret.scaleY=1;
    ret.splitNumber=1;
    ret.alive=1;
    return ret;
}

spaceRockArray* makeSpaceRockArray(int lengthToAllocate){
    spaceRockArray *ret=malloc(sizeof(spaceRockArray));
    ret->len=0;
    ret->allocatedLength=lengthToAllocate;
    ret->start=malloc(sizeof(spaceRock)*lengthToAllocate);
    return ret;
}

void deallocSpaceRockArray(spaceRockArray *sra){
    free(sra->start);
    free(sra);
}

int reallocateSpaceRockArray(spaceRockArray *sra, int newLength){
    if(newLength>sra->allocatedLength){
        spaceRock *newSpaceRocks=malloc(sizeof(spaceRock)*newLength);
        for(int i=0;i<sra->len;i++){
            newSpaceRocks[i]=sra->start[i];
        }
        free(sra->start);
        sra->start=newSpaceRocks;
        sra->allocatedLength=newLength;
        return 0;
    }
    return 1;
}

spaceRock* addToSpaceRockArray(spaceRockArray *sra, float x, float y, float xVel, float yVel, float rotAmount){
    if(sra->len+1>=sra->allocatedLength){
        int newLength=sra->allocatedLength*2;
        int didNotRealloc=reallocateSpaceRockArray(sra, newLength);
        if(didNotRealloc){
            printf("Could not reallocate space rock array\n");
        }
    }
    spaceRock *temp=sra->start;
    temp[sra->len]=makeSpaceRock(x,y,xVel,yVel,rotAmount);
    sra->len++;
    return &temp[sra->len-1];
}

void drawSpaceRock(spaceRock *s){
    glPushMatrix();
    glTranslatef(s->x, s->y, defaultZValue);
    glRotatef(s->rotZ, 0, 0 ,1);
    glScalef(s->scaleX,s->scaleY,1);
    //s->rotZ+=s->rotAmount;
    glColor3f(0.8, 0.8, 0.8);

    glBegin(GL_TRIANGLES);
        glVertex3f(-0.5, -0.4, defaultZValue);
        glVertex3f(0, 0.75, defaultZValue);
        glVertex3f(0.5, -0.4, defaultZValue);

        glVertex3f(-0.5, 0.4, defaultZValue);
        glVertex3f(0.5, 0.4, defaultZValue);
        glVertex3f(0, -0.75, defaultZValue);
    glEnd();

    glPopMatrix();
}

void drawSpaceRockArray(spaceRockArray *sra){
    spaceRock *temp=sra->start;
    for(int i=0;i<sra->len;i++){
        drawSpaceRock(&temp[i]);
    }
}

void spaceRockMotion(spaceRockArray *sra){
    spaceRock *startPtr=sra->start;
    spaceRock *working;
    for(int i=0;i<sra->len;i++){
        working=&startPtr[i];
        working->rotZ+=working->rotAmount;
        working->x+=working->xVel;
        working->y+=working->yVel;
    }
}

void removeSpaceRock(spaceRockArray *sra, int index){
    spaceRock *working=sra->start;
    sra->len--;
    if(sra->len==index){
        working[index].alive=0;
    }
    else{
        working[index]=working[sra->len];
        working[sra->len].alive=0;
    }
}

void spaceRockManagment(spaceRockArray *sra){
    spaceRock *working=sra->start;
    for(int i=0;i<sra->len;i++){
        if(working[i].x>15.5 || working[i].x<-15.5)
            working[i].x*=-1;
        if(working[i].y>15.5 || working[i].y<-15.5)
            working[i].y*=-1;
        if(working[i].alive==0){
            removeSpaceRock(sra, i);
            i=0;
        }

        /*
        if(working[i].x>15.5 || working[i].x<-15.5 || working[i].y>15.5 || working[i].y<-15.5 || working[i].alive==0){
            removeSpaceRock(sra, i);
            i=0;
        }
        */
    }
}

void spaceRockSpawnManagment(spaceRockArray *sra){
    float randomSpawnX, randomSpawnY, randomXVel, randomYVel, randomRotation;
    if(rand()%2==0){
        randomSpawnX=rand()%90;
        randomSpawnX+=5;
        randomSpawnX/=10;
        if(rand()%2==0){
            randomSpawnX*=-1;
            randomSpawnY=12.0f;
            randomYVel=-0.01;
            randomYVel-=(float)((rand()%10)/100);
            randomXVel=(float)((rand()%20)-10)/100;
        }
        else{
            randomSpawnY=-12.0f;
            randomYVel=0.01;
            randomYVel+=(float)((rand()%10)/100);
            randomXVel=(float)((rand()%20)-10)/100;
        }
        randomRotation=(rand()%3)+1;
    }
    else{
        randomSpawnY=rand()%90;
        randomSpawnY+=5;
        randomSpawnY/=10;
        if(rand()%2==0){
            randomSpawnY*=-1;
            randomSpawnX=12.0f;
            randomXVel=-0.01;
            randomXVel-=(float)((rand()%10)/100);
            randomYVel=(float)((rand()%20)-10)/100;
        }
        else{
            randomSpawnX=-12.0f;
            randomXVel=0.01;
            randomXVel+=(float)((rand()%10)/100);
            randomYVel=(float)((rand()%20)-10)/100;
        }
        randomRotation=(rand()%3)+1;
        randomRotation*=-1;
    }
    addToSpaceRockArray(sra, randomSpawnX, randomSpawnY, randomXVel, randomYVel, randomRotation);
}

void generateRandomVelocities(float *xVel, float *yVel){
    float x=(float)(rand()%20);
    x-=10;
    x/=100;
    float y=(float)(rand()%20);
    y-=10;
    y/=100;
    *xVel=x;
    *yVel=y;
}

void splitSpaceRock(spaceRockArray *sra, int spaceRockIndex, int splitAmount){
    spaceRock *currentSpaceRock=sra->start;
    currentSpaceRock=&currentSpaceRock[spaceRockIndex];
    float currentX,currentY;
    unsigned int currentSplit;
    currentX=currentSpaceRock->x;
    currentY=currentSpaceRock->y;
    currentSplit=currentSpaceRock->splitNumber;
    removeSpaceRock(sra, spaceRockIndex);
    for(int i=0;i<splitAmount;i++){
        float xVel, yVel;
        float randomRotation=(rand()%6);
        randomRotation-=3;
        generateRandomVelocities(&xVel, &yVel);
        spaceRock *justAdded=addToSpaceRockArray(sra, currentX, currentY, xVel, yVel, randomRotation);
        justAdded->splitNumber=currentSplit+1;
        justAdded->scaleX=(float)1/(float)justAdded->splitNumber;
        justAdded->scaleY=justAdded->scaleX;
    }
}
