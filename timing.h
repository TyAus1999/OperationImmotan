#pragma once
#include "inc.h"

typedef struct timeSt{
    clock_t start;
    clock_t end;
    int millis;
    int seconds;
} timeSt;

void timeStart(struct timeSt *t){
    t->start=clock();
}
void timeStop(struct timeSt *t){
    t->end=clock();
    clock_t diff=t->end-t->start;
    t->seconds=diff*1000/CLOCKS_PER_SEC;
    t->millis=t->seconds%1000;
    t->seconds/=1000;
}
float getSeconds(struct timeSt *t){
    float out=(float)t->millis;
    out/=100;
    out+=(float)t->seconds;
    return out;
}
void printTime(struct timeSt *t){
    printf("===========================\n");
    //printf("Seconds: %i\nMillis: %i\n", t->seconds, t->millis);
    printf("Time: %f\n", getSeconds(t));
    printf("===========================\n");
}