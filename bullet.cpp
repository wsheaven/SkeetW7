/***********************************************************************
 * Source File:
 *    Bullet
 * Author:
 *    Br. Helfrich
 * Summary:
 *    Stuff that kills other things
 ************************************************************************/

#include "bullet.h"

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

/*********************************************
 * BULLET constructor
 *********************************************/
Bullet::Bullet(double angle, double speed, double radius, int value) :
   dead(false), radius(radius), value(value)
{
   // set the initial position
   pt.setX(dimensions.getX() - 1.0);
   pt.setY(1.0);
   assert(pt.getX() > 100.0);

   // set the initial velocity
   v.setDx(-speed * cos(angle));
   v.setDy(speed * sin(angle));
   assert(v.getDx() <= 0.0);
   assert(v.getDy() >= 0.0);
}

/*********************************************
 * BOMB DEATH
 * Bombs have a tendency to explode!
 *********************************************/
void Bomb::death(std::list<Bullet*>& bullets)
{
   for (int i = 0; i < 20; i++)
      bullets.push_back(new Shrapnel(*this));
}

 /***************************************************************/
 /***************************************************************/
 /*                             MOVE                            */
 /***************************************************************/
 /***************************************************************/

/*********************************************
 * BULLET MOVE
 * Move the bullet along by one time period
 *********************************************/
void Bullet::move(std::list<Effect*> & effects)
{
   // inertia
   pt.add(v);

   // out of bounds checker
   if (isOutOfBounds())
      kill();
}

/*********************************************
 * BOMB MOVE
 * Move the bomb along by one time period
 *********************************************/
void Bomb::move(std::list<Effect*> & effects)
{
    // kill if it has been around too long
    timeToDie--;
    if (!timeToDie)
        kill();

    // do the inertia thing
    Bullet::move(effects);
}

/*********************************************
 * MISSILE MOVE
 * Move the missile along by one time period
 *********************************************/
void Missile::move(std::list<Effect*> & effects)
{
    // kill if it has been around too long
   effects.push_back(new Exhaust(pt, v));

    // do the inertia thing
    Bullet::move(effects);
}

/*********************************************
 * SHRAPNEL MOVE
 * Move the shrapnel along by one time period
 *********************************************/
void Shrapnel::move(std::list<Effect*> & effects)
{
    // kill if it has been around too long
    timeToDie--;
    if (!timeToDie)
        kill();

    // add a streek
    effects.push_back(new Streek(pt, v));
    
    // do the usual bullet stuff (like inertia)
    Bullet::move(effects);
}

/***************************************************************/
/***************************************************************/
/*                            OUTPUT                           */
/***************************************************************/
/***************************************************************/

/*************************************************************************
 * GL VERTEXT POINT
 * Just a more convenient format of glVertext2f
 *************************************************************************/
inline void glVertexPoint(const Position& point)
{
   glVertex2f((GLfloat)point.getX(), (GLfloat)point.getY());
}

/************************************************************************
 * DRAW LINE
 * Draw a line on the screen from the beginning to the end.
 *************************************************************************/
void Bullet::drawLine(const Position& begin, const Position& end,
                      double red, double green, double blue) const
{
   // Get ready...
   glBegin(GL_LINES);
   glColor3f((GLfloat)red, (GLfloat)green, (GLfloat)blue);

   // Draw the actual line
   glVertexPoint(begin);
   glVertexPoint(end);

   // Complete drawing
   glColor3f((GLfloat)1.0 /* red % */, (GLfloat)1.0 /* green % */, (GLfloat)1.0 /* blue % */);
   glEnd();
}

/************************************************************************
 * DRAW DOT
 * Draw a single point (square actually on the screen, r pixels by r pixels
 *************************************************************************/
void Bullet::drawDot(const Position& point, double radius,
                     double red, double green, double blue) const
{
   // Get ready, get set...
   glBegin(GL_TRIANGLE_FAN);
   glColor3f((GLfloat)red, (GLfloat)green, (GLfloat)blue);
   double r = radius / 2.0;

   // Go...
   glVertex2f((GLfloat)(point.getX() - r), (GLfloat)(point.getY() - r));
   glVertex2f((GLfloat)(point.getX() + r), (GLfloat)(point.getY() - r));
   glVertex2f((GLfloat)(point.getX() + r), (GLfloat)(point.getY() + r));
   glVertex2f((GLfloat)(point.getX() - r), (GLfloat)(point.getY() + r));

   // Done!  OK, that was a bit too dramatic
   glColor3f((GLfloat)1.0 /* red % */, (GLfloat)1.0 /* green % */, (GLfloat)1.0 /* blue % */);
   glEnd();
}

/*********************************************
 * PELLET OUTPUT
 * Draw a pellet - just a 3-pixel dot
 *********************************************/
void Pellet::output()
{
   if (!isDead())
      drawDot(pt, 3.0, 1.0, 1.0, 0.0);
}

/*********************************************
 * BOMB OUTPUT
 * Draw a bomb - many dots to make it have a soft edge
 *********************************************/
void Bomb::output()
{
   if (!isDead())
   {
       // Bomb actually has a gradient to cut out the harsh edges
       drawDot(pt, radius + 2.0, 0.50, 0.50, 0.00);
       drawDot(pt, radius + 1.0, 0.75, 0.75, 0.00);
       drawDot(pt, radius + 0.0, 0.87, 0.87, 0.00);
       drawDot(pt, radius - 1.0, 1.00, 1.00, 0.00);
   }
}

/*********************************************
 * SHRAPNEL OUTPUT
 * Draw a fragment - a bright yellow dot
 *********************************************/
void Shrapnel::output()
{
    if (!isDead())
       drawDot(pt, radius, 1.0, 1.0, 0.0);
}

/*********************************************
 * MISSILE OUTPUT
 * Draw a missile - a line and a dot for the fins
 *********************************************/
void Missile::output()
{
    if (!isDead())
    {
        // missile is a line with a dot at the end so it looks like fins.
        Position ptNext(pt);
        ptNext.add(v);
        drawLine(pt, ptNext, 1.0, 1.0, 0.0);
        drawDot(pt, 3.0, 1.0, 1.0, 1.0);
    }
}

/***************************************************************/
/***************************************************************/
/*                             MISC.                           */
/***************************************************************/
/***************************************************************/

/******************************************************************
 * RANDOM
 * This function generates a random number.
 ****************************************************************/
int Bullet::random(int min, int max)
{
   assert(min < max);
   int num = (rand() % (max - min)) + min;
   assert(min <= num && num <= max);
   return num;
}
double Bullet::random(double min, double max)
{
   assert(min <= max);
   double num = min + ((double)rand() / (double)RAND_MAX * (max - min));
   assert(min <= num && num <= max);
   return num;
}
