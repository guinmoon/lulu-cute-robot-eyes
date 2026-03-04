/*
 * Copyright (C) 2025 Artem Savkin
 * 
 * LuLuEyes - Animated Eyes Library for Arduino
 * 
 * Basic blink animations for LuLuEyes class
 */
#include "LuLuEyes.h"

//*********************************************************************************************
//  BASIC ANIMATION METHODS
//*********************************************************************************************

void LuLuEyes::close()
{
    eyeLheightNext = 1; // closing left eye
    eyeRheightNext = 1; // closing right eye
    eyeL_open = 0;      // left eye not opened (=closed)
    eyeR_open = 0;      // right eye not opened (=closed)
}

void LuLuEyes::open()
{
    eyeL_open = 1; // left eye opened - if true, drawEyes() will take care of opening eyes again
    eyeR_open = 1; // right eye opened
}

void LuLuEyes::blink()
{
    close();
    open();
}

void LuLuEyes::close(bool left, bool right)
{
    if (left)
    {
        eyeLheightNext = 1; // blinking left eye
        eyeL_open = 0;      // left eye not opened (=closed)
    }
    if (right)
    {
        eyeRheightNext = 1; // blinking right eye
        eyeR_open = 0; // right eye not opened (=closed)
    }
}

void LuLuEyes::open(bool left, bool right)
{
    if (left)
    {
        eyeL_open = 1; // left eye opened - if true, drawEyes() will take care of opening eyes again
    }
    if (right)
    {
        eyeR_open = 1; // right eye opened
    }
}

void LuLuEyes::blink(bool left, bool right)
{
    close(left, right);
    open(left, right);
}