/***********************************************************************
 * Source File:
 *    Position : The representation of a position on the screen
 * Author:
 *    Br. Helfrich
 * Summary:
 *    Everything we need to know about a location on the screen.
 ************************************************************************/

#include "position.h"
#include <cmath>
#include <cassert>

/******************************************
 * POINT : CONSTRUCTOR WITH X,Y
 * Initialize the point to the passed position
 *****************************************/
Position::Position(double x, double y) : x(0.0), y(0.0)
{
   setX(x);
   setY(y);
}

/******************************************
 * POINT : ADD
 * Move a point according to a velocity
 *****************************************/
void Position::add(const Velocity & v)
{
    *this += v;
}
Position & Position :: operator += (const Velocity & v)
{
    addX(v.getDx());
    addY(v.getDy());
    return *this;
}

/******************************************
 * VELOCITY : CONSTRUCTOR WITH X,Y
 * Initialize the point to the passed position
 *****************************************/
Velocity::Velocity(double dx, double dy) : dx(0.0), dy(0.0)
{
   setDx(dx);
   setDy(dy);
}

/******************************************
 * POINT insertion
 *       Display coordinates on the screen
 *****************************************/
std::ostream & operator << (std::ostream & out, const Position & pt)
{
   out << "(" << pt.getX() << ", " << pt.getY() << ")";
   return out;
}

/*******************************************
 * POINT extraction
 *       Prompt for coordinates
 ******************************************/
std::istream & operator >> (std::istream & in, Position & pt)
{
   double x;
   double y;
   in >> x >> y;

   pt.setX(x);
   pt.setY(y);

   return in;
}
   
double minimumDistance(const Position & pt1, const Velocity & v1,
                       const Position & pt2, const Velocity & v2) 
{
   double d1 = max(abs(v1.getDx()), abs(v1.getDy()));
   double d2 = max(abs(v2.getDx()), abs(v2.getDy()));
   double dMax = max(d1, d2);
   assert(dMax > 0.0);

   // we will move by this number
   double distMin = std::numeric_limits<double>::max();
      
   // what % of the speed will we be working with?
   double slice = 1.0 / dMax;
   
   // go through each percentage points
   for (double percent = 0.0; percent <= 1.0; percent += slice)
   {
      // find the points of the LHS and the RHS
      Position pointLhs(pt1.getX() + (v1.getDx() * percent),
                     pt1.getY() + (v1.getDy() * percent));
      Position pointRhs(pt2.getX() + (v2.getDx() * percent),
                     pt2.getY() + (v2.getDy() * percent));

      // how far apart are they now?
      double distSquared =
         ((pointLhs.getX() - pointRhs.getX()) * (pointLhs.getX() - pointRhs.getX())) +
         ((pointLhs.getY() - pointRhs.getY()) * (pointLhs.getY() - pointRhs.getY()));
      distMin = min(distMin, distSquared);
   }

   return sqrt(distMin);
}
