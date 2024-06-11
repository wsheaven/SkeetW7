/***********************************************************************
 * Header File:
 *    Score
 * Author:
 *    Br. Helfrich
 * Summary:
 *    How well the player is doing
 ************************************************************************/

#pragma once
#include <string>

/**********************
 * STATUS
 * How well the player is doing
 **********************/
class Status
{
public:
    Status() {}
    virtual std::string getText() const = 0;
    virtual void adjust(int value) = 0;
    virtual void reset() = 0;
};

/**********************
 * SCORE
 * Points earned vs lost
 **********************/
class Score : public Status
{
public:
    Score() { reset(); }
    std::string getText() const;
    void adjust(int value) { points += value; }
    void reset() { points = 0; }
private:
    int points;
};

/**********************
 * HIT RATIO
 * Bird hit ratio
 **********************/
class HitRatio : public Status
{
public:
    HitRatio()  { reset(); }
    std::string getText() const;
    void adjust(int value);
    void reset() { numKilled = numMissed = 0; }
private:
    int numKilled;
    int numMissed;
};
