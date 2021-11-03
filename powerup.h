#pragma once
#include "inc.h"
/*
Powerup types:
    1: Tripple shot
    2: Invincibility for 5 seconds
    4: Freeze Rocks for 5 seconds
    8: Rapid Fire
*/
typedef struct powerup{
    float x,y;
    float rotZ;
    float rotAmount;
    float scaleX,scaleY;
    char powerupType;
} powerup;

powerup makePowerup(float x, float y, float rotAmount, char powerupType){
    powerup ret;
    ret.x=x;
    ret.y=y;
    ret.rotZ=(float)(rand()%360);
    ret.rotAmount=rotAmount;
    ret.scaleX=1;
    ret.scaleY=1;
    ret.powerupType=powerupType;
    return ret;
}

void drawPowerup(powerup *p){
    glPushMatrix();
    glTranslatef(p->x, p->y, defaultZValue);
    glRotatef(p->rotZ, 0 ,0, 1);
    glScalef(p->scaleX, p->scaleY, 1);
    switch(p->powerupType){
        case 1:
            glColor3f(0,0.7,0.7);
            break;
        case 2:
            glColor3f(0.7,0.7,0);
            break;
        case 4:
            glColor3f(0.7,0,0.7);
            break;
        case 8:
            glColor3f(0,0.7,0);
            break;
    }
    glBegin(GL_QUADS);
        glVertex3f(-0.25,0.25,defaultZValue);
        glVertex3f(-0.25,-0.25,defaultZValue);
        glVertex3f(0.25,-0.25,defaultZValue);
        glVertex3f(0.25,0.25,defaultZValue);
    glEnd();
    glPopMatrix();
}

void drawAllPowerups(pointerArray *arr){
    powerup *working=arr->dataPointer;
    for(unsigned i=0;i<arr->len;i++){
        drawPowerup(&working[i]);
    }
}

void powerupMotion(pointerArray *arr){
    powerup *working;
    powerup *start=arr->dataPointer;
    for(unsigned i=0;i<arr->len;i++){
        working=&start[i];
        working->rotZ+=working->rotAmount;
    }
}

void powerupCollision(pointerArray *arr, ship *s){
    vec2f ship,temp;
    ship.x=s->x;
    ship.y=s->y;
    powerup *working=arr->dataPointer;
    for(int i=0;i<arr->len;i++){
        temp.x=working->x;
        temp.y=working->y;
        float dist=getDistanceVec2f(&ship,&temp);
        if(dist<0.5){
            s->activePowerup|=working->powerupType;
            //printf("Power up: %i\n", working->powerupType);
            if(working->powerupType==2)
                timeStart(&s->invincibilityTime);
            removeItem(arr,i);
            break;
        }
    }
}

void generatePowerup(pointerArray *arr){
    powerup toAdd;
    float spawnX,spawnY;
    spawnX=10.0-((float)(rand()%10));
    spawnY=10.0-((float)(rand()%10));
    char powerupType=1<<(rand()%4);
    toAdd=makePowerup(spawnX,spawnY,1,powerupType);
    addToPointerArray(arr, &toAdd);
}
