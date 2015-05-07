//Alexzander Avila
//Lab 6
//Source Files: Ragdoll Physics Implementation
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#define STARTING_ALTITUDE 12000

typedef float Flt;
typedef Flt Vec[3];
typedef Flt Matrix[3][3];

void DefineRagdoll();
void identity33(Matrix m);
struct Shape 
{
    float width, height, radius;
    Vec center;
    float velocityx, velocityy;
    Flt rot, rotInc;
    Matrix m;
    unsigned char color[3];
};

struct Game
{

    Shape body, rarm1, rarm2, larm1, larm2, rleg1, rleg2, lleg1, lleg2;
    float altitude;
    int n;
    Game()
    {
	altitude = STARTING_ALTITUDE;
	n = 0;
    }
};

void DefineRagdoll(Game *game)
{
    Shape *s;
    //define body shape
    s = &game->body;
    s->width = 40;
    s->height = 100;
    s->center[0] = s->width/2;
    s->center[1] = s->height/2;
    //place body
    s->center[0] += 300.0;
    s->center[1] += 50;
    s->color[0] = 90;
    s->color[1] = 0;
    s->color[2] = 90;
    s->rotInc = 0.0f;
    s->rot = 0.0f;
    identity33(s->m);
    
    //define upper right arm			
    s = &game->rarm1;				
    s->width = 20;
    s->height = 40;
    s->center[0] = s->width/2;
    s->center[1] = 0;
    //place on body
    s->center[0] += 40;
    s->center[1] += 40;
    s->color[0] = 90;
    s->color[1] = 45;
    s->color[2] = 45;
    s->rotInc = 0.0f;
    s->rot = 0.0f;
    identity33(s->m);

    //define lower right arm
    s = &game->rarm2;
    s->width = 15;
    s->height = 25;
    s->center[0] = s->width/2;
    s->center[1] = 0;
    //place on upper arm
    s->center[0] += 5;
    s->center[1] += 40;
    s->color[0] = 45;
    s->color[1] = 90;
    s->color[2] = 45;
    s->rotInc = 0.0f;
    s->rot = 0.0f;
    identity33(s->m);

    //define upper left arm
    s = &game->larm1;
    s->width = 20;
    s->height = 40;
    s->center[0] = s->width/2;
    s->center[1] = 0;
    //place on body
    s->center[0] -= 40;
    s->center[1] += 40;
    s->color[0] = 90;
    s->color[1] = 45;
    s->color[2] = 45;
    s->rotInc = 0.0f;
    s->rot = 0.0f;
    identity33(s->m);

    //define lower left arm
    s = &game->larm2;
    s->width = 15;
    s->height = 25;
    s->center[0] = s->width/2;
    s->center[1] = 0;
    //place on upper arm
    s->center[0] += -5;
    s->center[1] += 40;
    s->color[0] = 45;
    s->color[1] = 90;
    s->color[2] = 45;
    s->rotInc = 0.0f;
    s->rot = 0.0f;
    identity33(s->m);

    //define right quad
    s = &game->rleg1;
    s->width = 15;
    s->height = 40;
    s->center[0] = s->width/2;
    s->center[1] = 0;
    //place quad on body
    s->center[0] += 40;
    s->center[1] += -30;
    s->color[0] = 90;
    s->color[1] = 45;
    s->color[2] = 45;
    s->rotInc = 0.0f;
    s->rot = 0.0f;
    identity33(s->m);

    //define right shin
    s = &game->rleg2;
    s->width = 7;
    s->height = 15;
    s->center[0] = s->width/2;
    s->center[1] = 0;
    //place shin on quad
    s->center[0] += 20;
    s->center[1] += -10;
    s->color[0] = 45;
    s->color[1] = 90;
    s->color[2] = 45;
    s->rotInc = 0.0f;
    s->rot = 0.0f;
    identity33(s->m);

    //define left quad
    s = &game->lleg1;
    s->width = 15;
    s->height = 40;
    s->center[0] = s->width/2;
    s->center[1] = 0;
    //place on body
    s->center[0] += 40;
    s->center[1] += -30;
    s->color[0] = 90;
    s->color[1] = 45;
    s->color[2] = 45;
    s->rotInc = 0.0f;
    s->rot = 0.0f;
    identity33(s->m);

    //define left shin
    s = &game->lleg2;
    s->width = 7;
    s->height = 15;
    s->center[0] = s->width/2;
    s->center[1] = 0;
    //place shin on quad
    s->center[0] += 20;
    s->center[1] += -10;
    s->color[0] = 45;
    s->color[1] = 90;
    s->color[2] = 45;
    s->rotInc = 0.0f;
    s->rot = 0.0f;
    identity33(s->m);
}

void identity33(Matrix m)
{
    m[0][0] = m[1][1] = m[2][2] = 1.0f;
    m[0][1] = m[0][2] = m[1][0] = m[1][2] = m[2][0] = m[2][1] = 0.0f;
}

void yy_transform(Flt rotate, Matrix a)
{
    if(rotate != 0.0f)
    {
	Flt ct = cos(rotate), st = sin(rotate);
	Flt t00 = ct*a[0][0] - st*a[1][0];
	Flt t01 = ct*a[0][1] - st*a[1][1];
	Flt t02 = ct*a[0][2] - st*a[1][2];
	Flt t10 = st*a[0][0] + ct*a[1][0];
	Flt t11 = st*a[0][1] + ct*a[1][1];
	Flt t12 = st*a[0][2] + ct*a[1][2];
	a[0][0] = t00;
	a[0][1] = t01;
	a[0][2] = t02;
	a[1][0] = t10;
	a[1][1] = t11;
	a[1][2] = t12;
	return;
    }
}

void trans_vector(Matrix mat, const Vec in, Vec out)
{
    Flt f0 = mat[0][0] * in[0] + mat[1][0] * in[1] + mat[2][0] * in[2];
    Flt f1 = mat[0][1] * in[0] + mat[1][1] * in[1] + mat[2][1] * in[2];
    Flt f2 = mat[0][2] * in[0] + mat[1][2] * in[1] + mat[2][2] * in[2];
    out[0] = f0;
    out[1] = f1;
    out[2] = f2;
}

