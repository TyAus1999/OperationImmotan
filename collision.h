#pragma once
#include "inc.h"

typedef struct vec2f{
    float x, y;
} vec2f;

float getDistance(float x1, float y1, float x2, float y2){
    float out=0;
    float temp=y2-y1;
    out=x2-x1;
    out*=out;
    temp*=temp;
    out+=temp;
    out=sqrtf(out);
    return out;
}

float getDistanceVec2f(vec2f *a, vec2f *b){
    float out=b->x-a->x;
    out*=out;
    float temp=b->y-a->y;
    temp*=temp;
    out+=temp;
    out=sqrtf(out);
    return out;
}

char hasCollidedShipRock(ship *s, spaceRock *sr){
    char hasCollided=0;

    vec2f ship, rock;
    ship.x=s->x;
    ship.y=s->y;
    rock.x=sr->x;
    rock.y=sr->y;
    float distance=getDistanceVec2f(&ship,&rock);
    //printf("Distance: %f\n", distance);
    if(distance<(0.9f/(float)sr->splitNumber))
        hasCollided=1;
    return hasCollided;
}

char hasCollidedBulletRock(shipBullet *sb, spaceRock *sr){
    vec2f bullet, rock;
    bullet.x=sb->x;
    bullet.y=sb->y;
    rock.x=sr->x;
    rock.y=sr->y;
    float distance=getDistanceVec2f(&bullet, &rock);
    if(distance<(0.9f/(float)sr->splitNumber))
        return 1;
    return 0;
}

int indexHasCollidedWith(ship *s, spaceRockArray *sra){
    int out=-1;
    char hC=0;
    spaceRock *working=sra->start;
    for(int i=0;i<sra->len;i++){
        hC=hasCollidedShipRock(s, &working[i]);
        if(hC){
            out=i;
            break;
        }
    }
    return out;
}

int indexOfRockCollision(shipBullet *sb, spaceRockArray *sra){
    int out=-1;
    char hC=0;
    spaceRock *working=sra->start;
    for(int i=0;i<sra->len;i++){
        hC=hasCollidedBulletRock(sb, &working[i]);
        if(hC){
            out=i;
            break;
        }
    }

    return out;
}

void shipBulletCollisionManagment(shipBulletArray *sba, spaceRockArray *sra, u64 *amountDestroyed){
    shipBullet *working=sba->start;
    for(int i=0;i<sba->len;i++){
        int rockIndex=indexOfRockCollision(&working[i], sra);
        if(rockIndex!=-1){
            //removeSpaceRock(sra, rockIndex);
            spaceRock *workingRock=sra->start;
            workingRock=&workingRock[rockIndex];
            if(workingRock->splitNumber>3)
                removeSpaceRock(sra, rockIndex);
            else{
                int splitAmount;
                switch(workingRock->splitNumber){
                    case 1:
                        splitAmount=5;
                        break;
                    case 2:
                        splitAmount=3;
                        break;
                    case 3:
                        splitAmount=2;
                        break;
                }
                splitSpaceRock(sra, rockIndex, splitAmount);
            }
            removeBullet(sba, i);
            *amountDestroyed+=1;
            //printf("%u\n", *amountDestroyed);
            i=0;
        }
    }
}


