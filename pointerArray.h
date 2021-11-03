#pragma once
#include "inc.h"

typedef struct pointerArray{
    int len;
    int allocatedLen;
    int dataSize;
    void *dataPointer;
} pointerArray;

pointerArray* makePointerArray(int lengthToAllocate, int dataSize){
    pointerArray *ret=malloc(sizeof(pointerArray));
    ret->len=0;
    ret->allocatedLen=lengthToAllocate;
    ret->dataSize=dataSize;
    ret->dataPointer=malloc(lengthToAllocate*dataSize);
    return ret;
}

void deallocPointerArray(pointerArray *a){
    free(a->dataPointer);
    free(a);
}

int reallocPointerArray(pointerArray *a, int newLength){
    if(newLength>a->allocatedLen){
        void *newDataPointer=malloc(sizeof(a->dataSize*newLength));
        char *newTemp=newDataPointer;
        char *oldDataTemp=a->dataPointer;
        for(int i=0;i<a->len;i++){
            for(int d=0;d<a->dataSize;d++){
                char t=*oldDataTemp;
                *newTemp=t;
                newTemp++;
                oldDataTemp++;
            }
        }
        free(a->dataPointer);
        a->dataPointer=newDataPointer;
        a->allocatedLen=newLength;
        return 0;
    }
    return 1;
}

void* addToPointerArray(pointerArray *a, void *data){
    if(a->len+1>=a->allocatedLen){
        int newLength=a->allocatedLen*2;
        int didNotRealloc=reallocPointerArray(a,newLength);
        if(didNotRealloc){
            printf("Could not reallocate pointer array\n");
            return 0;
        }
    }
    char *temp=a->dataPointer+(a->dataSize*a->len);
    void *retPointer=temp;
    char *tempD=data;
    for(int i=0;i<a->dataSize;i++){
        char c=*tempD;
        *temp=c;
        tempD++;
        temp++;
    }
    a->len++;
    return retPointer;
}

void* getItem(pointerArray *a, unsigned index){
    if(index>=a->len)
        return 0;
    void *out=a->dataPointer;
    out+=a->dataSize*index;
    return out;
}

void removeItem(pointerArray *a, unsigned index){
    if(index>=a->len || a->len==0)
        return;
    a->len--;
    if(index<a->len){
        char *toBeOverWritten=a->dataPointer+(a->dataSize*index);
        char *toMove=a->dataPointer+(a->dataSize*a->len);
        for(int i=0;i<a->dataSize;i++){
            char temp=*(toMove+i);
            *(toBeOverWritten+i)=temp;
        }
    }
}