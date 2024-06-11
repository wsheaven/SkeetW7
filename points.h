/***********************************************************************
 * Header File:
 *    Points : Points graphic drawn on the screen
 * Author:
 *    Br. Helfrich
 * Summary:
 *    Inert point values on the screen.
 ************************************************************************/


#pragma once

#include "position.h"

 /*********************************************
  * POINTS
  * Points graphic drawn on the screen
  *********************************************/
class Points
{
public:
   Points(const Position& pt, int value);
   void show() const;
   void update();
   bool isDead() const {return age <= 0.0; }
private:
   Position pt;
   Velocity v;
   int value;
   float age;
};
