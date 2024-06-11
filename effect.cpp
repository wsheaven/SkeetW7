/***********************************************************************
 * Source File:
 *    Fragment : Pieces that fly off a dead bird
 * Author:
 *    Br. Helfrich
 * Summary:
 *    Pieces that fly off a dead bird
 ************************************************************************/

#include "effect.h"
#include <cassert>

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <openGL/gl.h>    // Main OpenGL library
#include <GLUT/glut.h>    // Second OpenGL library
#define GLUT_TEXT GLUT_BITMAP_HELVETICA_18
#endif // __APPLE__

#ifdef __linux__
#include <GL/gl.h>        // Main OpenGL library
#include <GL/glut.h>      // Second OpenGL library
#define GLUT_TEXT GLUT_BITMAP_HELVETICA_12
#endif // __linux__

#ifdef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>         // OpenGL library we copied
#define _USE_MATH_DEFINES
#include <math.h>
#define GLUT_TEXT GLUT_BITMAP_HELVETICA_12
#endif // _WIN32

/******************************************************************
 * RANDOM
 * This function generates a random number.
 ****************************************************************/
double random(double min, double max)
{
   assert(min <= max);
   double num = min + ((double)rand() / (double)RAND_MAX * (max - min));
   assert(min <= num && num <= max);
   return num;
}



/***************************************************************/
/***************************************************************/
/*                       CONSTRUCTORS                          */
/***************************************************************/
/***************************************************************/

/************************************************************************
 * FRAGMENT constructor
 *************************************************************************/
Fragment::Fragment(const Position & pt, const Velocity & v) : Effect(pt)
{
   // the velocity is a random kick plus the velocity of the thing that died
   this->v.setDx(v.getDx() * 0.5 + random(-6.0, 6.0));
   this->v.setDy(v.getDy() * 0.5 + random(-6.0, 6.0));
   
    // age
    age = random(0.4, 1.0);
    
    // size
    size = random(1.0, 2.5);
}

/************************************************************************
 * STREEK constructor
 *************************************************************************/
Streek::Streek(const Position & pt, Velocity v) : Effect(pt)
{
   ptEnd = pt;
   v *= -1.0;
   ptEnd += v;
   
    // age
    age = 0.5;
}

/************************************************************************
 * EXHAUST constructor
 *************************************************************************/
Exhaust::Exhaust(const Position & pt, Velocity v) : Effect(pt)
{
    ptEnd = pt;
    v *= -1.0;
    ptEnd += v;

    // age
    age = 0.5;
}

/***************************************************************/
/***************************************************************/
/*                           RENDER                            */
/***************************************************************/
/***************************************************************/

/************************************************************************
 * FRAGMENT RENDER
 * Draw the fragment on the screen
 *************************************************************************/
void Fragment::render() const
{
    // Do nothing if we are already dead
    if (isDead())
        return;
    
    // Draw this sucker
    glBegin(GL_TRIANGLE_FAN);
    
    // the color is a function of age - fading to black
    glColor3f((GLfloat)age, (GLfloat)age, (GLfloat)age);
    
    // draw the fragment
    glVertex2f((GLfloat)(pt.getX() - size), (GLfloat)(pt.getY() - size));
    glVertex2f((GLfloat)(pt.getX() + size), (GLfloat)(pt.getY() - size));
    glVertex2f((GLfloat)(pt.getX() + size), (GLfloat)(pt.getY() + size));
    glVertex2f((GLfloat)(pt.getX() - size), (GLfloat)(pt.getY() + size));
    glColor3f((GLfloat)1.0 /* red % */, (GLfloat)1.0 /* green % */, (GLfloat)1.0 /* blue % */);
    glEnd();
}

/************************************************************************
 * STREEK RENDER
 * Draw the shrapnel streek on the screen
 *************************************************************************/
void Streek::render() const
{
    // Do nothing if we are already dead
    if (isDead())
        return;
    
    // Draw this sucker
    glBegin(GL_LINES);
    glColor3f((GLfloat)age, (GLfloat)age, (GLfloat)age);

    // Draw the actual line
    glVertex2f((GLfloat)pt.getX(), (GLfloat)pt.getY());
    glVertex2f((GLfloat)ptEnd.getX(), (GLfloat)ptEnd.getY());

    glColor3f((GLfloat)1.0 /* red % */, (GLfloat)1.0 /* green % */, (GLfloat)1.0 /* blue % */);
    glEnd();
}

/************************************************************************
 * EXHAUST RENDER
 * Draw a missile exhaust on the screen
 *************************************************************************/
void Exhaust::render() const
{
   // Do nothing if we are already dead
   if (isDead())
       return;
   
   // Draw this sucker
   glBegin(GL_LINES);
   glColor3f((GLfloat)age, (GLfloat)age, (GLfloat)age);

   // Draw the actual line
   glVertex2f((GLfloat)pt.getX(), (GLfloat)pt.getY());
   glVertex2f((GLfloat)ptEnd.getX(), (GLfloat)ptEnd.getY());

   glColor3f((GLfloat)1.0 /* red % */, (GLfloat)1.0 /* green % */, (GLfloat)1.0 /* blue % */);
   glEnd();
}

/***************************************************************/
/***************************************************************/
/*                            FLY                              */
/***************************************************************/
/***************************************************************/

/************************************************************************
 * FRAGMENT FLY
 * Move the fragment on the screen
 *************************************************************************/
void Fragment :: fly()
{
    // move it forward with inertia (no gravity)
    pt += v;
    
    // increase the age so it fades away
    age -= 0.02;
    size *= 0.95;
}

/************************************************************************
 * STREEK FLY
 * The streek will just fade away
 *************************************************************************/
void Streek :: fly()
{
    // move it forward with inertia (no gravity)
//    pt += v;
    
   // increase the age so it fades away
   age -= 0.10;
}

/************************************************************************
 * EXHAUST FLY
 * The exhaust will just fade away
 *************************************************************************/
void Exhaust :: fly()
{
   // move it forward with inertia (no gravity)
//   pt += v;
    
   // increase the age so it fades away
   age -= 0.025;
}
