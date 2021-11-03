#pragma once
#include "inc.h"

typedef struct shipBullet{
    float x, y;
    float xVel, yVel;
    float zRotation;
    char alive;
} shipBullet;

typedef struct shipBulletArray{
    int len;
    int allocatedLength;
    shipBullet *start;
} shipBulletArray;

void drawShipBullet(shipBullet *sb){
    glPushMatrix();
    glColor3f(1,0,0);
    glTranslatef(sb->x, sb->y, defaultZValue);
    glRotatef(sb->zRotation, 0, 0, 1);
    glBegin(GL_TRIANGLES);
        glVertex3f(-0.1, -0.1, defaultZValue);
        glVertex3f(0.1, -0.1, defaultZValue);
        glVertex3f(0, 0.1, defaultZValue);
    glEnd();
    glPopMatrix();
}

shipBullet makeShipBullet(float x, float y, float rotation){
    shipBullet ret;
    ret.x=x;
    ret.y=y;
    ret.zRotation=rotation;
    ret.alive=1;
    //Use pythagorium to figure out velocities
    //float f=sqrtf(1.5);
    //SOHCAHTOA
    //y=sin(angle)*speed
    //x=cos(angle)*speed

    xyVelocities temp=calculateVelocities(rotation, bulletSpeed);
    ret.xVel=temp.xVel;
    ret.yVel=temp.yVel;

    return ret;
}

shipBulletArray* makeShipBulletArray(int allocLen){
    shipBulletArray *ret=malloc(sizeof(shipBulletArray));
    ret->start=malloc(sizeof(shipBullet)*allocLen);
    ret->len=0;
    ret->allocatedLength=allocLen;
    return ret;
}

void deallocShipBulletArray(shipBulletArray *sba){
    free(sba->start);
    free(sba);
}

int reallocateShipBulletArray(shipBulletArray *sba, int newLength){
    if(newLength>sba->allocatedLength){
        shipBullet *newShipBullet=malloc(sizeof(shipBullet)*newLength);
        for(int i=0;i<sba->len;i++){
            newShipBullet[i]=sba->start[i];
        }
        free(sba->start);
        sba->start=newShipBullet;
        sba->allocatedLength=newLength;
        return 0;
    }
    return 1;
}

void addToShipBulletArray(shipBulletArray *sba, float x, float y, float rotAmount){
    if(sba->len+1>=sba->allocatedLength){
        int newLength=sba->allocatedLength*2;
        int didNotRealloc=reallocateShipBulletArray(sba, newLength);
        if(didNotRealloc){
            printf("Could not reallocate Ship Bullet array\n");
        }
    }
    shipBullet *temp=sba->start;
    temp[sba->len]=makeShipBullet(x,y,rotAmount);
    sba->len++;
}

void shipBulletMotion(shipBulletArray *sba){
    if(sba->len>0){
        shipBullet *startPointer=sba->start;
        shipBullet *workingPointer;
        for(int i=0;i<sba->len;i++){
            workingPointer=&startPointer[i];
            workingPointer->x+=workingPointer->xVel;
            workingPointer->y+=workingPointer->yVel;
        }
    }
}

void drawShipBullets(shipBulletArray *sba){
    if(sba->len>0){
        shipBullet *startPointer=sba->start;
        shipBullet *workingPointer;
        for(int i=0;i<sba->len;i++){
            workingPointer=&startPointer[i];
            drawShipBullet(workingPointer);
        }
    }
}

void removeBullet(shipBulletArray *sba, int index){
    shipBullet *working=sba->start;
    sba->len--;
    if(sba->len==index){
        working[index].alive=0;
    }
    else{
        working[index]=working[sba->len];
        working[sba->len].alive=0;
    }
}

void shipBulletBoundManagment(shipBulletArray *sba){
    shipBullet *working=sba->start;
    for(int i=0;i<sba->len;i++){
        if(working[i].x>10.5 || working[i].x<-10.5 || working[i].y>10.5 || working[i].y<-10.5){
            removeBullet(sba, i);
            i=0;
        }
    }
}

