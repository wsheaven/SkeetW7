/***********************************************************************
 * Source File:
 *     UI INTERACT
 * Author:
 *     Br. Helfrich
 * Description:
 *     Implement the interfaces specified in uiInterface.h.  This handles
 *     all the interfaces and events necessary to work with OpenGL.  Your
 *     program will interface with this thorough the callback function
 *     pointer towards the bottom of the file.
 ************************************************************************/

#include <string>     // need you ask?
#include <sstream>    // convert an integer into text
#include <cassert>    // I feel the need... the need for asserts
#include <time.h>     // for clock
#include <cstdlib>    // for rand()


#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <openGL/gl.h>    // Main OpenGL library
#include <GLUT/glut.h>    // Second OpenGL library
#endif // __APPLE__

#ifdef __linux__
#include <GL/gl.h>    // Main OpenGL library
#include <GL/glut.h>  // Second OpenGL library
#endif // __linux__

#ifdef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <Gl/glut.h>           // OpenGL library we copied
#include <ctime>            // for ::Sleep();
#include <Windows.h>

#define _USE_MATH_DEFINES
#include <math.h>
#endif // _WIN32

#include "uiInteract.h"
#include "position.h"

using namespace std;


/*********************************************************************
 * SLEEP
 * Pause for a while.  We want to put the program to sleep until it
 * is time to output again.  Note that this requires us to tell the OS
 * that we are idle.  the nanosleep function performs this task for us
 *   INPUT: msSleep: sleep time in milliseconds
 *********************************************************************/
void sleep(unsigned long msSleep)
{
   // Windows handles sleep one way
#ifdef _WIN32
   ::Sleep(msSleep + 35);

   // Unix-based operating systems (OS-X, Linux) do it another
#else // LINUX, XCODE
   timespec req = {};
   time_t sec = (int)(msSleep / 1000);
   msSleep -= (sec * 1000);

   req.tv_sec = sec;
   req.tv_nsec = msSleep * 1000000L;

   while (nanosleep(&req, &req) == -1)
      ;
#endif // LINUX, XCODE
   return;
}

/************************************************************************
 * DRAW CALLBACK
 * This is the main callback from OpenGL. It gets called constantly by
 * the graphics engine to refresh and output the window.  Here we will
 * clear the background buffer, output on it, and send it to the forefront
 * when the appropriate time period has passsed.
 *
 * Note: This and all other callbacks can't be member functions, they must
 * have global scope for OpenGL to see them.
 *************************************************************************/
void drawCallback()
{
   // even though this is a local variable, all the members are static
   UserInput ui;
   // Prepare the background buffer for drawing
   glClear(GL_COLOR_BUFFER_BIT); //clear the screen
   glColor3f((GLfloat)1.0 /* red % */, (GLfloat)1.0 /* green % */, (GLfloat)1.0 /* blue % */);
   
   //calls the client's display function
   assert(ui.callBack != NULL);
   ui.callBack(&ui, ui.p);
   
   //loop until the timer runs out
   if (!ui.isTimeToDraw())
      sleep((unsigned long)((ui.getNextTick() - clock()) / 1000));

   // from this point, set the next output time
   ui.setNextDrawTime();

   // bring forth the background buffer
   glutSwapBuffers();

   // clear the space at the end
   ui.keyEvent();
}

/************************************************************************
 * KEY DOWN CALLBACK
 * When a key on the keyboard has been pressed, we need to pass that
 * on to the client.  Currently, we are only registering the arrow keys
 *   INPUT   key:   the key we pressed according to the GLUT_KEY_ prefix
 *           x y:   the position in the window, which we ignore
 *************************************************************************/
void keyDownCallback(int key, int x, int y)
{
   // Even though this is a local variable, all the members are static
   // so we are actually getting the same version as in the constructor.
   UserInput ui;
   ui.keyEvent(key, true /*fDown*/);
}

/************************************************************************
 * KEY UP CALLBACK
 * When the user has released the key, we need to reset the pressed flag
 *   INPUT   key:   the key we pressed according to the GLUT_KEY_ prefix
 *           x y:   the position in the window, which we ignore
 *************************************************************************/
void keyUpCallback(int key, int x, int y)
{
   // Even though this is a local variable, all the members are static
   // so we are actually getting the same version as in the constructor.
   UserInput ui;
   ui.keyEvent(key, false /*fDown*/);
}

/***************************************************************
 * KEYBOARD CALLBACK
 * Generic callback to a regular ascii keyboard event, such as
 * the space bar or the letter 'q'
 ***************************************************************/
void keyboardCallback(unsigned char key, int x, int y)
{
   // Even though this is a local variable, all the members are static
   // so we are actually getting the same version as in the constructor.
   UserInput ui;
   ui.keyEvent(key, true /*fDown*/);
}

/************************************************************************
 * CLOSE CALLBACK
 * Get the close button to appear so we can exit
 *************************************************************************/
void closeCallback()
{
   exit(0);
}

/***************************************************************
 * INTERFACE : KEY EVENT
 * Either set the up or down event for a given key
 *   INPUT   key     which key is pressed
 *           fDown   down or brown
 ****************************************************************/
void UserInput::keyEvent(int key, bool fDown)
{
   switch(key)
   {
      case GLUT_KEY_DOWN:
         isDownPress = fDown;
         break;
      case GLUT_KEY_UP:
         isUpPress = fDown;
         break;
      case GLUT_KEY_RIGHT:
         isRightPress = fDown;
         break;
      case GLUT_KEY_LEFT:
         isLeftPress = fDown;
         break;
      case GLUT_KEY_HOME:
      case ' ':
         isSpacePress = fDown;
         break;
      case 'b':
         isBPress = fDown;
         break;
      case 'm':
         isMPress = fDown;
         break;
   }

   isShiftPress = (glutGetModifiers () == GLUT_ACTIVE_SHIFT);
}

/***************************************************************
 * INTERFACE : KEY EVENT
 * Either set the up or down event for a given key
 *   INPUT   key     which key is pressed
 *           fDown   down or brown
 ****************************************************************/
void UserInput::keyEvent()
{
   if (isDownPress)
      isDownPress++;
   if (isUpPress)
      isUpPress++;
   if (isLeftPress)
      isLeftPress++;
   if (isRightPress)
      isRightPress++;
   isSpacePress = false;
   isMPress = false;
   isBPress = false;
}

/************************************************************************
 * INTEFACE : IS TIME TO DRAW
 * Have we waited long enough to output swap the background buffer with
 * the foreground buffer?
 *************************************************************************/
bool UserInput::isTimeToDraw()
{
   return ((unsigned int)clock() >= nextTick);
}

/************************************************************************
 * INTERFACE : SET NEXT DRAW TIME
 * What time should we output the buffer again?  This is a function of
 * the current time and the frames per second.
 *************************************************************************/
void UserInput::setNextDrawTime()
{
   nextTick = clock() + static_cast<int> (timePeriod * CLOCKS_PER_SEC);
}

/************************************************************************
 * INTERFACE : SET FRAMES PER SECOND
 * The frames per second dictates the speed of the game.  The more frames
 * per second, the quicker the game will appear to the user.  We will default
 * to 30 frames/second but the client can set this at will.
 *    INPUT  value        The number of frames per second.  30 is default
 *************************************************************************/
void UserInput::setFramesPerSecond(double value)
{
    timePeriod = (1 / value);
}

/***************************************************
 * STATICS
 * All the static member variables need to be initialized
 * Somewhere globally.  This is a good spot
 **************************************************/
int          UserInput::isDownPress  = 0;
int          UserInput::isUpPress    = 0;
int          UserInput::isLeftPress  = 0;
int          UserInput::isRightPress = 0;
bool         UserInput::isSpacePress = false;
bool         UserInput::isMPress     = false;
bool         UserInput::isBPress     = false;
bool         UserInput::isShiftPress = false;
bool         UserInput::initialized  = false;
double       UserInput::timePeriod   = 1.0 / 30; // default to 30 frames/second
unsigned long UserInput::nextTick     = 0;        // redraw now please
void *       UserInput::p            = NULL;
void (*UserInput::callBack)(const UserInput *, void *) = NULL;


/************************************************************************
 * INTEFACE : INITIALIZE
 * Initialize our drawing window.  This will set the size and position,
 * get ready for drawing, set up the colors, and everything else ready to
 * output the window.  All these are part of initializing Open GL.
 *  INPUT    argc:       Count of command-line arguments from main
 *           argv:       The actual command-line parameters
 *           title:      The text for the titlebar of the window
 *************************************************************************/
void UserInput::initialize(int argc, char ** argv, const char * title, const Position & dimensions)
{
   if (initialized)
      return;
   
   // set up the random number generator
   srand((unsigned int)time(NULL));

   // create the window
   glutInit(&argc, argv);
   glutInitWindowSize((int)dimensions.getX() * 2, (int)dimensions.getY() * 2);   // size of the window
 

   glutInitWindowPosition( 10, 10);                // initial position 
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);    // double buffering
   glutCreateWindow(title);                        // text on titlebar
   glutIgnoreKeyRepeat(true);
   
   // set up the drawing style: B/W and 2D
   glClearColor(0, 0, 0, 0);                // Black is the background color
   gluOrtho2D(0, (int)dimensions.getX(),       // range of x values: (0, width)
              0, (int)dimensions.getY());      // range of y values: (0, height)
   glutReshapeWindow((int)dimensions.getX(), (int)dimensions.getY());

   
   // register the callbacks so OpenGL knows how to call us
   glutDisplayFunc(   drawCallback    );
   glutIdleFunc(      drawCallback    );
   glutKeyboardFunc(  keyboardCallback);
   glutSpecialFunc(   keyDownCallback );
   glutSpecialUpFunc( keyUpCallback   );
#ifdef __APPLE__
   glutWMCloseFunc(   closeCallback   );
#endif 
   initialized = true;
   
   // done
   return;
}

/************************************************************************
 * INTERFACE : RUN
 *            Start the main graphics loop and play the game
 * INPUT callBack:   Callback function.  Every time we are beginning
 *                   to output a new frame, we first callback to the client
 *                   to see if he wants to do anything, such as move
 *                   the game pieces or respond to input
 *       p:          Void point to whatever the caller wants.  You
 *                   will need to cast this back to your own data
 *                   type before using it.
 *************************************************************************/
void UserInput::run(void (*callBack)(const UserInput *, void *), void *p)
{
   // setup the callbacks
   this->p = p;
   this->callBack = callBack;

   glutMainLoop();
   return;
}
