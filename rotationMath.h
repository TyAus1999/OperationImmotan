#pragma once
#include "inc.h"

typedef struct xyVelocities{
    float xVel, yVel;
} xyVelocities;

void printXYVel(xyVelocities *v){
    printf("XVel: %f\nYVel: %f\n", v->xVel, v->yVel);
}

xyVelocities calculateVelocities(float angle, float maxSpeed){
    xyVelocities out;
    
    if(angle==0.0f || angle==360.0f){
        out.xVel=0;
        out.yVel=maxSpeed;
    }
    else if(angle==90.0f){
        out.xVel=-1*maxSpeed;
        out.yVel=0;
    }
    else if(angle==180.0f){
        out.xVel=0;
        out.yVel=-1*maxSpeed;
    }
    else if(angle==270.0f){
        out.xVel=maxSpeed;
        out.yVel=0;
    }
    else{
        /*
        if(angle>90){
            //printf("Greater than 90\n");
            float angleToUse=angle;
            int amount=0;
            while(angleToUse>90){
                angleToUse-=90;
                amount++;
            }
            out.xVel=sin(angleToUse*RADConversion)*maxSpeed;
            out.yVel=cos(angleToUse*RADConversion)*maxSpeed;
            switch(amount){
                case 1:
                    out.yVel*=-1;
                    out.xVel*=-1;
                    break;
                case 2:
                    out.yVel*=-1;
                    break;
            }
        }
        else{
            //printf("90Less\n");
            float radCon=angle*RADConversion;
            out.xVel=sin(radCon);
            out.xVel*=maxSpeed;
            out.xVel*=-1;
            out.yVel=cos(radCon);
            out.yVel*=maxSpeed;
        }
        */
        /*
        float cvt=angle*RADConversion;
        if(angle>0 && angle<90){
            out.xVel=sinf(cvt)*maxSpeed;
            out.xVel*=-1;
            out.yVel=cosf(cvt)*maxSpeed;
        }
        else if(angle>90 && angle<180){
            out.xVel=cosf(cvt)*maxSpeed;
            out.yVel=sinf(cvt)*maxSpeed;
            out.yVel*=-1;
        }
        else if(angle>180 && angle<270){
            out.xVel=cosf(cvt)*maxSpeed;
            out.yVel=sinf(cvt)*maxSpeed;
        }
        else if(angle>270 && angle<360){
            out.xVel=sinf(cvt)*maxSpeed;
            out.yVel=cosf(cvt)*maxSpeed;
        }
        */

       if(angle<90){
           out.xVel=sinf(angle*RADConversion)*maxSpeed;
           out.xVel*=-1;
           out.yVel=cosf(angle*RADConversion)*maxSpeed;
       }
       else{
           float angleToUse=angle;
           int amount=0;
           while(angleToUse>90){
               angleToUse-=90;
               amount++;
           }
           out.yVel=sinf(angleToUse*RADConversion)*maxSpeed;
           out.xVel=cosf(angleToUse*RADConversion)*maxSpeed;
           switch(amount){
                case 1:
                    out.xVel*=-1;
                    out.yVel*=-1;
                    break;
                case 2:
                    amount=2;
                    float temp=out.xVel;
                    out.xVel=out.yVel;
                    out.yVel=temp;
                    out.yVel*=-1;
                    break;
           }
       }
    }
    /*
    printf("===================\n");
    printf("Angle: %f\n", angle);
    printXYVel(&out);
    printf("===================\n");
    */
    return out;
}