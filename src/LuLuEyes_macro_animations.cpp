/*
 * Copyright (C) 2025 Artem Savkin
 * 
 * LuLuEyes - Animated Eyes Library for Arduino
 * 
 * Macro animations for LuLuEyes class
 */
#include "LuLuEyes.h"

//*********************************************************************************************
//  MACRO ANIMATION METHODS
//*********************************************************************************************

void LuLuEyes::anim_confused()
{
    confused = 1;
}

void LuLuEyes::anim_laugh()
{
    laugh = 1;
}

void LuLuEyes::anim_hearts()
{
    hearts = 1;
    heartsToggle = 0;
    heartsAnimationTimer = millis();
}

void LuLuEyes::anim_fallingAsleep()
{
    fallingAsleep = 1;
    fallingAsleepStage = 1;
    fallingAsleepTimer = millis();
    sleepBlinkCounter = 0;
    sleepBlinkTimer = millis();
    // Disable other animations during sleep
    autoblinker = 0;
    idle = 0;
    hFlicker = 0;
    vFlicker = 0;
}

void LuLuEyes::anim_wakeUp()
{
    fallingAsleep = 0;
    fallingAsleepStage = 0;
    // Open eyes
    eyeLheightNext = eyeLheightDefault;
    eyeRheightNext = eyeRheightDefault;
    eyeL_open = 1;
    eyeR_open = 1;
}