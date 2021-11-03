#include "inc.h"

void idleFunction();
void displayFunction();
void reshapeFunction(int w, int h);
void keyDown(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);

int currentWidth=700, currentHeight=700;
char keyW=0, keyS=0, keyA=0, keyD=0, keySpace=0;
char isPaused=0;
char hasDied=0;

spaceRockArray *spaceRocks;
shipBulletArray *bullets;
pointerArray *powerups;
ship player;
timeSt rockSpawnTimer;
u64 amountOfRocksDestroyed=0;
u64 level=1;
char levelHasStarted=0;

float lightDiffuse[]={1,0,0,0};
float lightPosition[]={0,0,1,0};

// Define vertices in counter-clockwise (CCW) order with normal pointing out
int main(int argc, char **argv){
    srand(time(NULL));
    player=makeDefaultShip();
    player.activePowerup=9;
    spaceRocks=makeSpaceRockArray(2);
    bullets=makeShipBulletArray(2);
    powerups=makePointerArray(2,sizeof(powerup));
    generatePowerup(powerups);

    //powerup t=makePowerup(5,5,1,1);
    //addToPointerArray(powerups, &t);

	glutInit(&argc, argv);
    glutSetOption(GLUT_MULTISAMPLE, 12);
    glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_MULTISAMPLE|GLUT_DOUBLE);
    glutInitWindowPosition(500,0);
    glutInitWindowSize(currentWidth,currentHeight);
    glutCreateWindow("Operation Immotan");
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    //glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    //glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glutIdleFunc(idleFunction);
    glutDisplayFunc(displayFunction);
    glutReshapeFunc(reshapeFunction);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
    timeStart(&rockSpawnTimer);
    glutMainLoop();
    deallocSpaceRockArray(spaceRocks);
    deallocShipBulletArray(bullets);
    deallocPointerArray(powerups);
    //printf("Shut Down\n");
}

void idleFunction(){
    if(!isPaused){
        if(keyW){
            addShipSpeed(&player, maxShipSpeed, shipAcceleration);
            player.isMovingForward=1;
        }
        else
            player.isMovingForward=0;
        
        if(keyS){
            addShipSpeed(&player, maxNegShipSpeed, shipAcceleration);
        }
        
        if(keyA){
            player.zRotation+=5;
            if(player.zRotation>=360)
                player.zRotation=player.zRotation-360;
        }
        else if(keyD){
            player.zRotation-=5;
            if(player.zRotation<0)
                player.zRotation=360+player.zRotation;
        }

        if(keySpace){
            keySpace=0;
            char what=player.activePowerup&8;
            if(what==8)
                keySpace=1;
            if(player.activePowerup&1==1){
                addToShipBulletArray(bullets, player.x, player.y, player.zRotation);
                addToShipBulletArray(bullets, player.x, player.y, player.zRotation-3);
                addToShipBulletArray(bullets, player.x, player.y, player.zRotation+3);
            }
            else{
                addToShipBulletArray(bullets, player.x, player.y, player.zRotation);
            }
        }  
        
        
        powerupMotion(powerups);
        spaceRockMotion(spaceRocks);
        shipBulletMotion(bullets);
        shipMotion(&player);


        if(player.x>10.5f || player.x<-10.5f)
            player.x*=-1;
        if(player.y>10.5f || player.y<-10.5f)
            player.y*=-1;

        char what=player.activePowerup&2;
        if(what==0){
            int collisionIndex=indexHasCollidedWith(&player, spaceRocks);
            //collisionIndex=-1;
            if(collisionIndex!=-1){
                //printf("Has collided with %i\n", collisionIndex);
                player.hasCollided=1;
                hasDied=1;
                isPaused=1;
                printf("Destroyed %llu\n", amountOfRocksDestroyed);
            }
            else
                player.hasCollided=0;
        }
        else{
            timeStop(&player.invincibilityTime);
            float t=getSeconds(&player.invincibilityTime);
            if(t>5.0f){
                player.activePowerup-=2;
                player.b=0;
            }
        }


        shipManagment(&player);
        shipBulletBoundManagment(bullets);
        spaceRockManagment(spaceRocks);
        shipBulletCollisionManagment(bullets, spaceRocks, &amountOfRocksDestroyed);
        powerupCollision(powerups, &player);

        /*
        timeStop(&rockSpawnTimer);
        float currentTime=getSeconds(&rockSpawnTimer);
        float timeOfSpawn=(float)3/(float)amountOfRocksDestroyed;
        if(amountOfRocksDestroyed==0)
            timeOfSpawn=3.0f;
        timeOfSpawn=0.5f;
        if(currentTime>timeOfSpawn){
            timeStart(&rockSpawnTimer);
            spaceRockSpawnManagment(spaceRocks);
        }
        */

        if(!levelHasStarted){
            levelHasStarted=1;
            printf("Starting Level %llu\n",level);
            for(u64 i=0;i<level*3;i++)
                spaceRockSpawnManagment(spaceRocks);
        }
        else{
            if(spaceRocks->len==0){
                levelHasStarted=0;
                printf("Completed Level %llu\n",level);
                level++;
            }
        }

        //printf("Seconds: %f\n", getSeconds(&rockSpawnTimer));
        //printf("%i, %i\n", spaceRocks->len, spaceRocks->allocatedLength);
        //printf("%i,%i\n", bullets->len, bullets->allocatedLength);
        //printf("X: %f\t\tY: %f\n", player.x, player.y);
        
        glutPostRedisplay();
    }
}

void displayFunction(){
    glClearColor(0.1,0.1,0.1,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    drawAllPowerups(powerups);
    drawSpaceRockArray(spaceRocks);
    drawShip(&player);
    drawShipBullets(bullets);

    glutSwapBuffers();
}

void reshapeFunction(int w, int h){
    glViewport(0,0,w,h);
    currentWidth=w;
    currentHeight=h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-10,10,-10,10,1,100);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

void keyDown(unsigned char key, int x, int y){
    switch (key)
    {
        case 'w':
        case 'W':
            keyW=1;
            break;
        case 's':
        case 'S':
            keyS=1;
            break;
        case 'a':
        case 'A':
            keyA=1;
            break;
        case 'd':
        case 'D':
            keyD=1;
            break;
        case ' ':
            keySpace=1;
            break;
        case 'p':
            if(isPaused){
                deallocShipBulletArray(bullets);
                deallocSpaceRockArray(spaceRocks);
                deallocPointerArray(powerups);
                player=makeDefaultShip();
                spaceRocks=makeSpaceRockArray(2);
                bullets=makeShipBulletArray(2);
                powerups=makePointerArray(2,sizeof(powerup));
                generatePowerup(powerups);
                isPaused=0;
                amountOfRocksDestroyed=0;
                level=1;
                levelHasStarted=0;
            }
        default:
            break;
    }
}

void keyUp(unsigned char key, int x, int y){
    switch (key)
    {
        case 'w':
        case 'W':
            keyW=0;
            break;
        case 's':
        case 'S':
            keyS=0;
            break;
        case 'a':
        case 'A':
            keyA=0;
            break;
        case 'd':
        case 'D':
            keyD=0;
            break;
        case ' ':
            keySpace=0;
            break;
        default:
            break;
    }
}


