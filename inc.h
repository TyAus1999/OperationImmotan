#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>
#include "GL/freeglut.h"
#include "GL/gl.h"
#include "GL/glut.h"
#include "GL/glu.h"
#include <math.h>
#include <time.h>

#define defaultZValue -0.5
#define bulletSpeed 0.25
#define maxShipSpeed 0.2
#define maxNegShipSpeed (maxShipSpeed*-1)
#define shipAcceleration 0.001
#define PI 3.141592653589793
#define RADConversion (PI/180)
#define blockSpeed 0.1

typedef unsigned long long u64;

#include "timing.h"
#include "blocks.h"
#include "rotationMath.h"
#include "ship.h"
#include "shipBullet.h"
#include "spaceRock.h"
#include "collision.h"
#include "pointerArray.h"
#include "powerup.h"
