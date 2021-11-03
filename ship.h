#pragma once
#include "inc.h"
typedef struct ship{
    float x, y;
    float xVel, yVel;
    float zRotation;
    float r,g,b;
    char isMovingForward;
    char hasCollided;
    char activePowerup;
    timeSt invincibilityTime;
    timeSt rapidFireTime;
} ship;

ship makeDefaultShip(){
    ship out;
    out.x=0;
    out.y=0;
    out.xVel=0;
    out.yVel=0;
    out.zRotation=0;
    out.isMovingForward=0;
    out.hasCollided=0;
    out.activePowerup=0;
    out.r=0;
    out.g=1;
    out.b=0;
    return out;
}

void drawShip(ship *s){
    glPushMatrix();
    /*
    if(s->hasCollided)
        glColor3f(0,0,1);
    else{
        float r,g,b;
        r=0;
        g=1;
        b=0;
        if(s->activePowerup&1==1)
            r=1;
        if(s->activePowerup&2==2)
            b=1;
        glColor3f(r,g,b);
    }
    */
        //glColor3f(0,1,0);
    glColor3f(s->r, s->g, s->b);
    glTranslatef(s->x, s->y, defaultZValue);
    glRotatef(s->zRotation, 0,0,1);
    glBegin(GL_TRIANGLES);
        glVertex3f(-0.25, -0.5, defaultZValue);
        glVertex3f(0.25, -0.5, defaultZValue);
        glVertex3f(0, 0.5, defaultZValue);
    glEnd();

    if(s->isMovingForward){
        float tempRandom=0.0f-(float)((rand()%7)+5);
        tempRandom/=10;
        glColor3f(1,0,0);
        glBegin(GL_TRIANGLES);
            glVertex3f(0.2,-0.35, defaultZValue);
            glVertex3f(-0.2, -0.35, defaultZValue);
            glVertex3f(0, tempRandom, defaultZValue);
        glEnd();
    }

    glPopMatrix();
}

void addShipSpeed(ship *s, float speed, float acceleration){
    xyVelocities temp;
    temp=calculateVelocities(s->zRotation, speed);

    /*
    if(temp.xVel<0){
        s->xVel-=acceleration;
        if(s->xVel<temp.xVel)
            s->xVel=temp.xVel;
    }
    else if(temp.xVel>0){
        s->xVel+=acceleration;
        if(s->xVel>temp.xVel)
            s->xVel=temp.xVel;
    }
    */
    
    if(s->xVel<temp.xVel){
        s->xVel+=acceleration;
        if(s->xVel>temp.xVel)
            s->xVel=temp.xVel;
    }
    else if(s->xVel>temp.xVel){
        s->xVel-=acceleration;
        if(s->xVel<temp.xVel)
            s->xVel=temp.xVel;
    }

    /*
    if(temp.yVel<0){
        s->yVel-=acceleration;
        if(s->yVel<temp.yVel)
            s->yVel=temp.yVel;
    }
    else if(temp.yVel>0){
        s->yVel+=acceleration;
        if(s->yVel>temp.yVel)
            s->yVel=temp.yVel;
    }
    */

    if(s->yVel<temp.yVel){
        s->yVel+=acceleration;
        if(s->yVel>temp.yVel)
            s->yVel=temp.yVel;
    }
    else if(s->yVel>temp.yVel){
        s->yVel-=acceleration;
        if(s->yVel<temp.yVel)
            s->yVel=temp.yVel;
    }

    /*
    if(speed<0)
        temp=calculateVelocities(s->zRotation, speed*-1);
    else
        temp=calculateVelocities(s->zRotation, speed);
    
    
    printf("Angle: %f\n", s->zRotation);
    printf("Speed: %f\n", speed);
    float si=sin(s->zRotation*RADConversion);
    printf("Sin: %f\n", si);
    si*=speed;
    printf("Times Speed: %f\n", si);
    printXYVel(&temp);
    

    if(speed<0)
        s->xVel=temp.xVel*-1;
    else
        s->xVel=temp.xVel;

    if(s->xVel>maxShipSpeed)
        s->xVel=maxShipSpeed;
    else if(s->xVel<maxNegShipSpeed)
        s->xVel=maxNegShipSpeed;
    
    
    if(speed<0)
        s->yVel=temp.yVel*-1;
    else
        s->yVel=temp.yVel;

    if(s->yVel>maxShipSpeed)
        s->yVel=maxShipSpeed;
    else if(s->yVel<maxNegShipSpeed)
        s->yVel=maxNegShipSpeed;
    */
}

void shipMotion(ship *s){
    s->x+=s->xVel;
    s->y+=s->yVel;
}

void shipManagment(ship *s){
    if(s->hasCollided){
        s->r=1;
        s->g=0;
        s->b=0;
    }
    else{
        char what=s->activePowerup&1;
        if(what==1)
            s->r=1;
        what=s->activePowerup&8;
        if(what==8)
            s->r=1;
        what=s->activePowerup&2;
        if(what==2)
            s->b=1;
    }
}
