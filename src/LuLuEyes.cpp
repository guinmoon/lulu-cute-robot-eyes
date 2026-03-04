/*
 * Copyright (C) 2025 Artem Savkin
 * 
 * LuLuEyes - Animated Eyes Library for Arduino
 * 
 * Main file for LuLuEyes class - General methods, setters, getters
 */
#include "LuLuEyes.h"

//*********************************************************************************************
//  GENERAL METHODS
//*********************************************************************************************

void LuLuEyes::begin(int width, int height, LGFX_Sprite* _sprite)
{
    sprite = _sprite;
    
    screenWidth = width;     // OLED display width, in pixels
    screenHeight = height;   // OLED display height, in pixels
    eyeLheightCurrent = 1;   // start with closed eyes
    eyeRheightCurrent = 1;   // start with closed eyes
}

void LuLuEyes::update()
{
    // Limit drawing updates to defined max framerate
    // if (millis() - fpsTimer >= frameInterval)
    // {
    drawEyes();
    // fpsTimer = millis();
    // }
}

//*********************************************************************************************
//  SETTERS METHODS
//*********************************************************************************************

void LuLuEyes::setFramerate(byte fps)
{
    frameInterval = 1000 / fps;
}

void LuLuEyes::setWidth(byte leftEye, byte rightEye)
{
    eyeLwidthNext = leftEye;
    eyeRwidthNext = rightEye;
    eyeLwidthDefault = leftEye;
    eyeRwidthDefault = rightEye;
}

void LuLuEyes::setHeight(byte leftEye, byte rightEye)
{
    eyeLheightNext = leftEye;
    eyeRheightNext = rightEye;
    eyeLheightDefault = leftEye;
    eyeRheightDefault = rightEye;
}

void LuLuEyes::setBorderradius(byte leftEye, byte rightEye)
{
    eyeLborderRadiusNext = leftEye;
    eyeRborderRadiusNext = rightEye;
    eyeLborderRadiusDefault = leftEye;
    eyeRborderRadiusDefault = rightEye;
}

void LuLuEyes::setSpacebetween(int space)
{
    spaceBetweenNext = space;
    spaceBetweenDefault = space;
}

void LuLuEyes::setMood(unsigned char mood)
{
    switch (mood)
    {
    case TIRED:
        tired = 1;
        angry = 0;
        happy = 0;
        break;
    case ANGRY:
        tired = 0;
        angry = 1;
        happy = 0;
        break;
    case HAPPY:
        tired = 0;
        angry = 0;
        happy = 1;
        break;
    default:
        tired = 0;
        angry = 0;
        happy = 0;
        break;
    }
}

void LuLuEyes::setPosition(unsigned char position)
{
    switch (position)
    {
    case N:
        // North, top center
        eyeLxNext = getScreenConstraint_X() / 2;
        eyeLyNext = 0;
        break;
    case NE:
        // North-east, top right
        eyeLxNext = getScreenConstraint_X();
        eyeLyNext = 0;
        break;
    case E:
        // East, middle right
        eyeLxNext = getScreenConstraint_X();
        eyeLyNext = getScreenConstraint_Y() / 2;
        break;
    case SE:
        // South-east, bottom right
        eyeLxNext = getScreenConstraint_X();
        eyeLyNext = getScreenConstraint_Y();
        break;
    case S:
        // South, bottom center
        eyeLxNext = getScreenConstraint_X() / 2;
        eyeLyNext = getScreenConstraint_Y();
        break;
    case SW:
        // South-west, bottom left
        eyeLxNext = 0;
        eyeLyNext = getScreenConstraint_Y();
        break;
    case W:
        // West, middle left
        eyeLxNext = 0;
        eyeLyNext = getScreenConstraint_Y() / 2;
        break;
    case NW:
        // North-west, top left
        eyeLxNext = 0;
        eyeLyNext = 0;
        break;
    default:
        // Middle center
        eyeLxNext = getScreenConstraint_X() / 2;
        eyeLyNext = getScreenConstraint_Y() /2;
        break;
    }
}

void LuLuEyes::setAutoblinker(bool active, int interval, int variation)
{
    autoblinker = active;
    blinkInterval = interval;
    blinkIntervalVariation = variation;
}

void LuLuEyes::setAutoblinker(bool active)
{
    autoblinker = active;
}

void LuLuEyes::setIdleMode(bool active, int interval, int variation)
{
    idle = active;
    idleInterval = interval;
    idleIntervalVariation = variation;
}

void LuLuEyes::setIdleMode(bool active)
{
    idle = active;
}

void LuLuEyes::setCuriosity(bool curiousBit)
{
    curious = curiousBit;
}

void LuLuEyes::setCyclops(bool cyclopsBit)
{
    cyclops = cyclopsBit;
}

void LuLuEyes::setHFlicker(bool flickerBit, byte Amplitude)
{
    hFlicker = flickerBit;         // turn flicker on or off
    hFlickerAmplitude = Amplitude; // define amplitude of flickering in pixels
}

void LuLuEyes::setHFlicker(bool flickerBit)
{
    hFlicker = flickerBit; // turn flicker on or off
}

void LuLuEyes::setVFlicker(bool flickerBit, byte Amplitude)
{
    vFlicker = flickerBit;         // turn flicker on or off
    vFlickerAmplitude = Amplitude; // define amplitude of flickering in pixels
}

void LuLuEyes::setVFlicker(bool flickerBit)
{
    vFlicker = flickerBit; // turn flicker on or off
}

//*********************************************************************************************
//  GETTERS METHODS
//*********************************************************************************************

int LuLuEyes::getScreenConstraint_X()
{
    return screenWidth - eyeLwidthCurrent - spaceBetweenCurrent - eyeRwidthCurrent;
}

int LuLuEyes::getScreenConstraint_Y()
{
    return screenHeight - eyeLheightDefault; // using default height here, because height will vary when blinking and in curious mode
}