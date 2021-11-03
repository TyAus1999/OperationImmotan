#pragma once
#include "inc.h"

typedef struct block{
    float x,y;
    float destX, destY;
    float xVel, yVel;
    char shouldMove;
    float r,g,b;
} block;

typedef struct blockArray{
    int len;
    int allocatedLen;
    block *start;
} blockArray;

void drawBlock(block *b){
    glPushMatrix();
    glTranslatef(b->x, b->y, defaultZValue);
    glColor3f(b->r, b->g, b->b);
    glBegin(GL_QUADS);
        glVertex3f(0,0,defaultZValue);
        glVertex3f(0.1,0,defaultZValue);
        glVertex3f(0.1,0.1,defaultZValue);
        glVertex3f(0,0.1,defaultZValue);
    glEnd();
    glPopMatrix();
}

void drawBlockArray(blockArray *ba){
    block *working=ba->start;
    for(int i=0;i<ba->len;i++)
        drawBlock(&working[i]);
}

block makeDefaultBlock(float x, float y, float destX, float destY){
    block out;
    out.x=x;
    out.y=y;
    out.destX=destX;
    out.destY=destY;
    out.shouldMove=0;
    out.xVel=0;
    out.yVel=0;
    out.r=0;
    out.g=0;
    out.b=1;
    return out;
}

blockArray* makeBlockArray(int initAllocLength){
    blockArray *out=malloc(sizeof(blockArray));
    out->start=malloc(sizeof(block)*initAllocLength);
    out->len=0;
    out->allocatedLen=initAllocLength;
    return out;
}

void deallocBlockArray(blockArray* ba){
    free(ba->start);
    free(ba);
}

char reallocateBlockArray(blockArray *ba, int size){
    block *newLocation=malloc(sizeof(block)*size);
    if(newLocation==NULL)
        return 0;
    block *working=ba->start;
    for(int i=0;i<ba->len;i++)
        newLocation[i]=working[i];
    free(ba->start);
    ba->start=newLocation;
    ba->allocatedLen=size;
    return 1;
}

block* addBlock(blockArray *ba, float x, float y, float destX, float destY){
    if(ba->len+1==ba->allocatedLen){
        char didRealloc=reallocateBlockArray(ba, ba->allocatedLen*2);
        if(!didRealloc){
            printf("Could not reallocate to add a new block\n");
        }
    }
    block *blockToAdd=ba->start;
    blockToAdd=&blockToAdd[ba->len];
    ba->len++;
    *blockToAdd=makeDefaultBlock(x,y,destX,destY);
    return blockToAdd;
}

block* addBlockFA(blockArray *ba, float *f){
    return addBlock(ba, f[0], f[1], f[2], f[3]);
}

void removeBlockFromArray(blockArray *ba, int index){
    if(index==ba->len-1){
        ba->len--;
    }
    else{
        ba->len--;
        block *working=ba->start;
        working[index]=working[ba->len];
    }
}

void blockMovementManagment(blockArray *ba){
    block *working=ba->start;
    for(int i=0;i<ba->len;i++){
        if(working->x<working->destX){
            working->x+=blockSpeed;
        }
        else if(working->x>working->destX){
            working->x-=blockSpeed;
        }

        if(working->y<working->destY){
            working->y+=blockSpeed;
        }
        else if(working->y>working->destY){
            working->y-=blockSpeed;
        }
    }
}

void makeLetter(blockArray *ba, char letter, float x, float y, float destX, float destY){
    switch(letter){
        case 'a':
        case 'A':
            letter=letter;
            float *workingFloats=_mm_malloc(sizeof(float)*4, 32);
            __m128 origin, valueToAdd, working;
            valueToAdd=_mm_set_ps1(0.1f);
            origin=_mm_set_ps(x,y,destX,destY);
            addBlock(ba, x,y,destX,destY);

            working=_mm_add_ps(origin, valueToAdd);
            _mm_store_ps(workingFloats, working);
            addBlockFA(ba, workingFloats);

            working=_mm_add_ps(working, valueToAdd);
            _mm_store_ps(workingFloats, working);
            addBlockFA(ba, workingFloats);

            working=_mm_add_ps(working, valueToAdd);
            _mm_store_ps(workingFloats, working);
            addBlockFA(ba, workingFloats);
            
            break;
        default:
            break;
    }
}





