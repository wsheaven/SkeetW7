/***********************************************************************
 * Header File:
 *    TIME
 * Author:
 *    Br. Helfrich
 * Summary:
 *    Time in level, what level it is, etc.
 ************************************************************************/

#pragma once
#include <array>
#include <string>
#include <cassert>

#define FRAMES_PER_SECOND 30

/************************
 * TIME
 * Time in level, etc.
 ************************/
class Time
{
public:
    Time() { reset(); }
    
    // which level are we in?
    int level() const  { return levelNumber; }
    
    // are we currently playing in a level?
    bool isPlaying() const;
    
    // are we currently showing the status screen?
    bool isStatus() const { return !isPlaying(); }
    
    // has the game ended?
    bool isGameOver() const { return levelNumber == 0; }
    
    // is this the very start of the playing time of the leve
    bool isStartLevel() const;
    
    // how much time is there left?
    int secondsLeft() const;
    
    // what percent is left?
    double percentLeft() const;

    // move the time counter by one frame
    void operator++(int postfix);
    
    // text
    std::string getText() const;

    // reset
    void reset();

private:
    // number of frames left in this level
    int framesLeft;
    
    // the level number we are currently in
    int levelNumber;
    
    // length in seconds of each level
    std::array<int, 5> levelLength;
    
    // seconds from frames
    int secondsFromFrames(int frame) const
    {
        return frame / FRAMES_PER_SECOND;
    }
    
    // how long have we been in the level in seconds?
    int secondsInLevel() const
    {
        assert(levelNumber < (int)levelLength.size());
        return levelLength[levelNumber] - secondsFromFrames(framesLeft);
    }
    
};


