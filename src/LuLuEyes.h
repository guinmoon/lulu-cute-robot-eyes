/*
 * Copyright (C) 2025 Artem Savkin
 * 
 * LuLuEyes - Animated Eyes Library for Arduino
 */

#ifndef _LULU_EYES_H
#define _LULU_EYES_H

#include <LovyanGFX.h>

// Usage of monochrome display colors
#define BGCOLOR 0          // background and overlays
#define MAINCOLOR TFT_GREEN // drawings
#define HEARTCOLOR TFT_RED // heart color

// For mood type switch 
#define DEFAULT 0
#define TIRED 1
#define ANGRY 2
#define HAPPY 3

// For turning things on or off
#define ON 1
#define OFF 0

// For switch "predefined positions"
#define N 1  // north, top center
#define NE 2 // north-east, top right
#define E 3  // east, middle right
#define SE 4 // south-east, bottom right
#define S 5  // south, bottom center
#define SW 6 // south-west, bottom left
#define W 7  // west, middle left
#define NW 8 // north-west, top left

// for middle center set "DEFAULT"

class LuLuEyes
{
private:
    // Yes, everything is currently still accessible. Be responsibly and don't mess things up :)
public:
    // LGFX_MyDisplay *display;
    inline static LGFX_Sprite* sprite;
    // For general setup - screen size and max. frame rate
    int screenWidth = 128;      // OLED display width, in pixels
    int screenHeight = 64;      // OLED display height, in pixels
    int frameInterval = 20;     // default value for 50 frames per second (1000/50 = 20 milliseconds)
    unsigned long fpsTimer = 0; // for timing the frames per second
    // For controlling mood types and expressions
    bool tired = 0;
    bool angry = 0;
    bool happy = 0;
    bool curious = 0;   // if true, draw the outer eye larger when looking left or right
    bool cyclops = 0;   // if true, draw only one eye
    bool eyeL_open = 0; // left eye opened or closed?
    bool eyeR_open = 0; // right eye opened or closed?
    // Heart mode
    bool hearts = 0;
    unsigned long heartsAnimationTimer = 0;
    int heartsAnimationDuration = 2000; // 2 seconds total (1 second transform to hearts, 1 second transform back)
    bool heartsToggle = 0;
    //*********************************************************************************************
    //  Eyes Geometry
    //*********************************************************************************************
    // EYE LEFT - size and border radius
    int eyeLwidthDefault = 80;
    int eyeLheightDefault = 80;
    int eyeLwidthCurrent = eyeLwidthDefault;
    int eyeLheightCurrent = 1; // start with closed eye, otherwise set to eyeLheightDefault
    int eyeLwidthNext = eyeLwidthDefault;
    int eyeLheightNext = eyeLheightDefault;
    int eyeLheightOffset = 0;
    // Border Radius
    byte eyeLborderRadiusDefault = 8;
    byte eyeLborderRadiusCurrent = eyeLborderRadiusDefault;
    byte eyeLborderRadiusNext = eyeLborderRadiusDefault;
    // EYE RIGHT - size and border radius
    int eyeRwidthDefault = eyeLwidthDefault;
    int eyeRheightDefault = eyeLheightDefault;
    int eyeRwidthCurrent = eyeRwidthDefault;
    int eyeRheightCurrent = 1; // start with closed eye, otherwise set to eyeRheightDefault
    int eyeRwidthNext = eyeRwidthDefault;
    int eyeRheightNext = eyeRheightDefault;
    int eyeRheightOffset = 0;
    // Border Radius
    byte eyeRborderRadiusDefault = 8;
    byte eyeRborderRadiusCurrent = eyeRborderRadiusDefault;
    byte eyeRborderRadiusNext = eyeRborderRadiusDefault;
    // EYE LEFT - Coordinates
    int eyeLxDefault = ((screenWidth) - (eyeLwidthDefault + spaceBetweenDefault + eyeRwidthDefault)) / 2;
    int eyeLyDefault = ((screenHeight - eyeLheightDefault) / 2);
    int eyeLx = eyeLxDefault;
    int eyeLy = eyeLyDefault;
    int eyeLxNext = eyeLx;
    int eyeLyNext = eyeLy;
    // EYE RIGHT - Coordinates
    int eyeRxDefault = eyeLx + eyeLwidthCurrent + spaceBetweenDefault;
    int eyeRyDefault = eyeLy;
    int eyeRx = eyeRxDefault;
    int eyeRy = eyeRyDefault;
    int eyeRxNext = eyeRx;
    int eyeRyNext = eyeRy;
    // BOTH EYES
    // Eyelid top size
    byte eyelidsHeightMax = eyeLheightDefault / 2; // top eyelids max height
    byte eyelidsTiredHeight = 0;
    byte eyelidsTiredHeightNext = eyelidsTiredHeight;
    byte eyelidsAngryHeight = 0;
    byte eyelidsAngryHeightNext = eyelidsAngryHeight;
    // Bottom happy eyelids offset
    byte eyelidsHappyBottomOffsetMax = (eyeLheightDefault / 2) + 3;
    byte eyelidsHappyBottomOffset = 0;
    byte eyelidsHappyBottomOffsetNext = 0;
    // Space between eyes
    int spaceBetweenDefault = 10;
    int spaceBetweenCurrent = spaceBetweenDefault;
    int spaceBetweenNext = 10;
    //*********************************************************************************************
    //  Macro Animations
    //*********************************************************************************************
    // Animation - horizontal flicker/shiver
    bool hFlicker = 0;
    bool hFlickerAlternate = 0;
    byte hFlickerAmplitude = 2;
    // Animation - vertical flicker/shiver
    bool vFlicker = 0;
    bool vFlickerAlternate = 0;
    byte vFlickerAmplitude = 10;
    // Animation - auto blinking
    bool autoblinker = 0;           // activate auto blink animation
    int blinkInterval = 1;          // basic interval between each blink in full seconds
    int blinkIntervalVariation = 4; // interval variaton range in full seconds, random number inside of given range will be add to the basic blinkInterval, set to 0 for no variation
    unsigned long blinktimer = 0;   // for organising eyeblink timing
    // Animation - idle mode: eyes looking in random directions
    bool idle = 0;
    int idleInterval = 1;                 // basic interval between each eye repositioning in full seconds
    int idleIntervalVariation = 3;        // interval variaton range in full seconds, random number inside of given range will be add to the basic idleInterval, set to 0 for no variation
    unsigned long idleAnimationTimer = 0; // for organising eyeblink timing
    // Animation - eyes confused: eyes shaking left and right
    bool confused = 0;
    unsigned long confusedAnimationTimer = 0;
    int confusedAnimationDuration = 500;
    bool confusedToggle = 1;
    // Animation - eyes laughing: eyes shaking up and down
    bool laugh = 0;
    unsigned long laughAnimationTimer = 0;
    int laughAnimationDuration = 500;
    bool laughToggle = 1;
    // Animation - falling asleep: gradual eye closing with slow blinks
    bool fallingAsleep = 0;
    unsigned long fallingAsleepTimer = 0;
    int fallingAsleepDuration = 5000; // 5 seconds total animation
    int fallingAsleepStage = 0; // 0 = not started, 1 = animating, 2 = complete
    int sleepBlinkCounter = 0;
    unsigned long sleepBlinkTimer = 0;
    
    //*********************************************************************************************
    //  GENERAL METHODS
    //*********************************************************************************************
    // Startup RoboEyes with defined screen-width, screen-height and max. frames per second
    void begin(int width, int height, LGFX_Sprite* _sprite);
    void update();
    
    //*********************************************************************************************
    //  SETTERS METHODS
    //*********************************************************************************************
    // Calculate frame interval based on defined frameRate
    void setFramerate(byte fps);
    void setWidth(byte leftEye, byte rightEye);
    void setHeight(byte leftEye, byte rightEye);
    // Set border radius for left and right eye
    void setBorderradius(byte leftEye, byte rightEye);
    // Set space between the eyes, can also be negative
    void setSpacebetween(int space);
    // Set mood expression
    void setMood(unsigned char mood);
    // Set predefined position
    void setPosition(unsigned char position);
    // Set automated eye blinking, minimal blink interval in full seconds and blink interval variation range in full seconds
    void setAutoblinker(bool active, int interval, int variation);
    void setAutoblinker(bool active);
    // Set idle mode - automated eye repositioning, minimal time interval in full seconds and time interval variation range in full seconds
    void setIdleMode(bool active, int interval, int variation);
    void setIdleMode(bool active);
    // Set curious mode - the respectively outer eye gets larger when looking left or right
    void setCuriosity(bool curiousBit);
    // Set cyclops mode - show only one eye
    void setCyclops(bool cyclopsBit);
    // Set horizontal flickering (displacing eyes left/right)
    void setHFlicker(bool flickerBit, byte Amplitude);
    void setHFlicker(bool flickerBit);
    // Set vertical flickering (displacing eyes up/down)
    void setVFlicker(bool flickerBit, byte Amplitude);
    void setVFlicker(bool flickerBit);
    
    //*********************************************************************************************
    //  GETTERS METHODS
    //*********************************************************************************************
    // Returns the max x position for left eye
    int getScreenConstraint_X();
    // Returns the max y position for left
    int getScreenConstraint_Y();
    
    //*********************************************************************************************
    //  BASIC ANIMATION METHODS
    //*********************************************************************************************
    // BLINKING FOR BOTH EYES AT ONCE
    // Close both eyes
    void close();
    // Open both eyes
    void open();
    // Trigger eyeblink animation
    void blink();
    // BLINKING FOR SINGLE EYES, CONTROL EACH EYE SEPARATELY
    // Close eye(s)
    void close(bool left, bool right);
    // Open eye(s)
    void open(bool left, bool right);
    // Trigger eyeblink(s) animation
    void blink(bool left, bool right);
    
    //*********************************************************************************************
    //  MACRO ANIMATION METHODS
    //*********************************************************************************************
    // Play confused animation - one shot animation of eyes shaking left and right
    void anim_confused();
    // Play laugh animation - one shot animation of eyes shaking up and down
    void anim_laugh();
    // Play hearts animation - eyes transform to hearts and back
    void anim_hearts();
    // Play falling asleep animation - gradual eye closing with slow, heavy blinks
    void anim_fallingAsleep();
    // Wake up - open eyes after falling asleep
    void anim_wakeUp();
    
    //*********************************************************************************************
    //  PRE-CALCULATIONS AND ACTUAL DRAWINGS
    //*********************************************************************************************
    const int cleanupPadding = 0;
    // Create a rectangle that will be used for cleanup
    int cleanupLX = eyeLx - cleanupPadding;
    int cleanupLY = eyeLy - cleanupPadding;
    int cleanupLWidth = eyeLwidthCurrent + 2 * cleanupPadding;
    int cleanupLHeight = eyeLheightCurrent + 2 * cleanupPadding;
    int cleanupRX = eyeRx - cleanupPadding;
    int cleanupRY = eyeRy - cleanupPadding;
    int cleanupRWidth = eyeRwidthCurrent + 2 * cleanupPadding;
    int cleanupRHeight = eyeRheightCurrent + 2 * cleanupPadding;
    
    void calcCleanEyes();
    void cleanEyes();
    void drawHeart(int x, int y, int size, uint16_t color);
    void drawEyes();
};

#endif