/*
 * Copyright (C) 2025 Artem Savkin
 * 
 * LuLuEyes - Animated Eyes Library for Arduino
 * 
 * Drawing and rendering methods for LuLuEyes class
 */
#include "LuLuEyes.h"

//*********************************************************************************************
//  PRE-CALCULATIONS AND ACTUAL DRAWINGS
//*********************************************************************************************

void LuLuEyes::calcCleanEyes()
{
    // Cover a slightly larger area to remove any traces
    cleanupLX = eyeLx - cleanupPadding;
    cleanupLY = eyeLy - cleanupPadding;
    cleanupLWidth = eyeLwidthCurrent + 2 * cleanupPadding;
    cleanupLHeight = eyeLheightCurrent + 2 * cleanupPadding;
    cleanupRX = eyeRx - cleanupPadding;
    cleanupRY = eyeRy - cleanupPadding;
    cleanupRWidth = eyeRwidthCurrent + 2 * cleanupPadding;
    cleanupRHeight = eyeRheightCurrent + 2 * cleanupPadding;
    // Make sure parameters are within valid range
    cleanupLX = max(0, cleanupLX);
    cleanupLY = max(0, cleanupLY);
    cleanupLWidth = min(screenWidth - cleanupLX, cleanupLWidth);
    cleanupLHeight = min(screenHeight - cleanupLY, cleanupLHeight);
    cleanupRX = max(0, cleanupRX);
    cleanupRY = max(0, cleanupRY);
    cleanupRWidth = min(screenWidth - cleanupRX, cleanupRWidth);
    cleanupRHeight = min(screenHeight - cleanupRY, cleanupRHeight);
}

void LuLuEyes::cleanEyes()
{
    // Clear the left eye area
    sprite->clear();
}

void LuLuEyes::drawHeart(int x, int y, int size, uint16_t color)
{
    // Draw the heart using triangles and circles
    // Top left circle
    sprite->fillCircle(x + size/4, y + size/4, size/4, color);
    // Top right circle
    sprite->fillCircle(x + 3*size/4, y + size/4, size/4, color);
    // Bottom triangle
    sprite->fillTriangle(
        x, y + size/3,
        x + size, y + size/3,
        x + size/2, y + size,
        color
    );
}

void LuLuEyes::drawEyes()
{
    // PRE-CALCULATIONS - EYE SIZES AND VALUES FOR ANIMATION TWEENINGS
    calcCleanEyes();
    
    // Handle hearts animation
    if (hearts)
    {
        unsigned long elapsed = millis() - heartsAnimationTimer;
        if (elapsed >= heartsAnimationDuration)
        {
            hearts = 0; // Animation finished
            // Reset eye positions after hearts animation
            eyeLheightNext = eyeLheightDefault;
            eyeRheightNext = eyeRheightDefault;
            eyeLwidthNext = eyeLwidthDefault;
            eyeRwidthNext = eyeRwidthDefault;
            // Reset eye positions to default
            eyeLxNext = eyeLxDefault;
            eyeLyNext = eyeLyDefault;
            eyeRxNext = eyeRxDefault;
            eyeRyNext = eyeRyDefault;
        }
    }
    
    // Handle falling asleep animation
    if (fallingAsleep && fallingAsleepStage == 1)
    {
        unsigned long elapsed = millis() - fallingAsleepTimer;
        
        // Animation progresses through stages based on elapsed time
        if (elapsed >= fallingAsleepDuration)
        {
            // Animation complete - eyes fully closed
            fallingAsleepStage = 2;
            eyeLheightNext = 1;
            eyeRheightNext = 1;
        }
        else
        {
            // Calculate progress (0.0 to 1.0)
            float progress = (float)elapsed / fallingAsleepDuration;
            
            // Perform slow, heavy blinks that get slower as we progress
            // Stage 1: First blink (0-20% of animation)
            if (progress < 0.2f && sleepBlinkCounter == 0)
            {
                if (millis() - sleepBlinkTimer > 800) // First blink after 800ms
                {
                    close();
                    sleepBlinkCounter = 1;
                    sleepBlinkTimer = millis();
                }
            }
            // Opening after first blink (20-30%)
            else if (progress >= 0.2f && sleepBlinkCounter == 1)
            {
                open();
                sleepBlinkCounter = 2;
            }
            // Stage 2: Second blink (30-50% of animation)
            else if (progress < 0.5f && sleepBlinkCounter == 2)
            {
                if (millis() - sleepBlinkTimer > 1200) // Second blink after 1200ms (slower)
                {
                    close();
                    sleepBlinkCounter = 3;
                    sleepBlinkTimer = millis();
                }
            }
            // Opening after second blink (50-60%)
            else if (progress >= 0.5f && sleepBlinkCounter == 3)
            {
                open();
                sleepBlinkCounter = 4;
            }
            // Stage 3: Third blink (60-80% of animation)
            else if (progress < 0.8f && sleepBlinkCounter == 4)
            {
                if (millis() - sleepBlinkTimer > 1800) // Third blink after 1800ms (even slower)
                {
                    close();
                    sleepBlinkCounter = 5;
                    sleepBlinkTimer = millis();
                }
            }
            // Opening after third blink (80-90%)
            else if (progress >= 0.8f && sleepBlinkCounter == 5)
            {
                open();
                sleepBlinkCounter = 6;
            }
            // Final close (90-100%)
            else if (progress >= 0.9f && sleepBlinkCounter == 6)
            {
                close();
                sleepBlinkCounter = 7; // Won't open again
            }
            
            // Gradually reduce eye size to simulate heavy eyelids
            // Eyes become slightly smaller with each stage
            if (progress < 0.3f)
            {
                eyeLheightNext = eyeLheightDefault;
                eyeRheightNext = eyeRheightDefault;
            }
            else if (progress < 0.6f)
            {
                eyeLheightNext = eyeLheightDefault * 0.9f;
                eyeRheightNext = eyeRheightDefault * 0.9f;
            }
            else if (progress < 0.85f)
            {
                eyeLheightNext = eyeLheightDefault * 0.7f;
                eyeRheightNext = eyeRheightDefault * 0.7f;
            }
            else
            {
                eyeLheightNext = eyeLheightDefault * 0.5f;
                eyeRheightNext = eyeRheightDefault * 0.5f;
            }
        }
    }
    
    // Vertical size offset for larger eyes when looking left or right (curious gaze)
    if (curious && !hearts)
    {
        if (eyeLxNext <= 10)
        {
            eyeLheightOffset = 8;
        }
        else if (eyeLxNext >= (getScreenConstraint_X() - 10) && cyclops)
        {
            eyeLheightOffset = 8;
        }
        else
        {
            eyeLheightOffset = 0;
        } // left eye
        if (eyeRxNext >= screenWidth - eyeRwidthCurrent - 10)
        {
            eyeRheightOffset = 8;
        }
        else
        {
            eyeRheightOffset = 0;
        } // right eye
    }
    else
    {
        eyeLheightOffset = 0; // reset height offset for left eye
        eyeRheightOffset = 0; // reset height offset for right eye
    }
    // Left eye height
    eyeLheightCurrent = (eyeLheightCurrent + eyeLheightNext + eyeLheightOffset) / 2;
    eyeLy += ((eyeLheightDefault - eyeLheightCurrent) / 2); // vertical centering of eye when closing
    eyeLy -= eyeLheightOffset / 2;
    // Right eye height
    eyeRheightCurrent = (eyeRheightCurrent + eyeRheightNext + eyeRheightOffset) / 2;
    eyeRy += (eyeRheightDefault - eyeRheightCurrent) / 2; // vertical centering of eye when closing
    eyeRy -= eyeRheightOffset / 2;
    // Open eyes again after closing them
    if (eyeL_open && !hearts && !fallingAsleep)
    {
        if (eyeLheightCurrent <= 1 + eyeLheightOffset)
        {
            eyeLheightNext = eyeLheightDefault;
        }
    }
    if (eyeR_open && !hearts && !fallingAsleep)
    {
        if (eyeRheightCurrent <= 1 + eyeRheightOffset)
        {
            eyeRheightNext = eyeRheightDefault;
        }
    }
    // Left eye width
    eyeLwidthCurrent = (eyeLwidthCurrent + eyeLwidthNext) / 2;
    // Right eye width
    eyeRwidthCurrent = (eyeRwidthCurrent + eyeRwidthNext) / 2;
    // Space between eyes
    spaceBetweenCurrent = (spaceBetweenCurrent + spaceBetweenNext) / 2;
    // Left eye coordinates
    eyeLx = (eyeLx + eyeLxNext) / 2;
    eyeLy = (eyeLy + eyeLyNext) / 2;
    // Right eye coordinates
    eyeRxNext = eyeLxNext + eyeLwidthCurrent + spaceBetweenCurrent; // right eye's x position depends on left eyes position + the space between
    eyeRyNext = eyeLyNext;                                          // right eye's y position should be the same as for the left eye
    eyeRx = (eyeRx + eyeRxNext) / 2;
    eyeRy = (eyeRy + eyeRyNext) / 2;
    // Left eye border radius
    eyeLborderRadiusCurrent = (eyeLborderRadiusCurrent + eyeLborderRadiusNext) / 2;
    // Right eye border radius
    eyeRborderRadiusCurrent = (eyeRborderRadiusCurrent + eyeRborderRadiusNext) / 2;
    // APPLYING MACRO ANIMATIONS
    if (autoblinker && !hearts && !fallingAsleep)
    {
        if (millis() >= blinktimer)
        {
            blink();
            blinktimer = millis() + (blinkInterval * 1000) + (random(blinkIntervalVariation) * 1000); // calculate next time for blinking
        }
    }
    // Laughing - eyes shaking up and down for the duration defined by laughAnimationDuration (default = 500ms)
    if (laugh && !hearts && !fallingAsleep)
    {
        if (laughToggle)
        {
            setVFlicker(1, 5);
            laughAnimationTimer = millis();
            laughToggle = 0;
        }
        else if (millis() >= laughAnimationTimer + laughAnimationDuration)
        {
            setVFlicker(0, 0);
            laughToggle = 1;
            laugh = 0;
        }
    }
    // Confused - eyes shaking left and right for the duration defined by confusedAnimationDuration (default = 500ms)
    if (confused && !hearts && !fallingAsleep)
    {
        if (confusedToggle)
        {
            setHFlicker(1, 20);
            confusedAnimationTimer = millis();
            confusedToggle = 0;
        }
        else if (millis() >= confusedAnimationTimer + confusedAnimationDuration)
        {
            setHFlicker(0, 0);
            confusedToggle = 1;
            confused = 0;
        }
    }
    // Idle - eyes moving to random positions on screen
    if (idle && !hearts && !fallingAsleep)
    {
        if (millis() >= idleAnimationTimer)
        {
            eyeLxNext = random(getScreenConstraint_X());
            eyeLyNext = random(getScreenConstraint_Y());
            idleAnimationTimer = millis() + (idleInterval * 1000) + (random(idleIntervalVariation) * 1000); // calculate next time for eyes repositioning
        }
    }
    // Adding offsets for horizontal flickering/shivering
    if (hFlicker && !hearts)
    {
        if (hFlickerAlternate)
        {
            eyeLx += hFlickerAmplitude;
            eyeRx += hFlickerAmplitude;
        }
        else
        {
            eyeLx -= hFlickerAmplitude;
            eyeRx -= hFlickerAmplitude;
        }
        hFlickerAlternate = !hFlickerAlternate;
    }
    // Adding offsets for horizontal flickering/shivering
    if (vFlicker && !hearts)
    {
        if (vFlickerAlternate)
        {
            eyeLy += vFlickerAmplitude;
            eyeRy += vFlickerAmplitude;
        }
        else
        {
            eyeLy -= vFlickerAmplitude;
            eyeRy -= vFlickerAmplitude;
        }
        vFlickerAlternate = !vFlickerAlternate;
    }
    // Cyclops mode, set second eye's size and space between to 0
    if (cyclops && !hearts)
    {
        eyeRwidthCurrent = 0;
        eyeRheightCurrent = 0;
        spaceBetweenCurrent = 0;
    }
    // ACTUAL DRAWINGS
    if (cleanupLX != eyeLx || cleanupLY != eyeLy || cleanupLWidth != eyeLwidthCurrent 
        || cleanupLHeight != eyeLheightCurrent || cleanupRX != eyeRx || cleanupRY != eyeRy 
        || cleanupRWidth != eyeRwidthCurrent || cleanupRHeight != eyeRheightCurrent) // if the left eye has changed, clear it and redraw it
        cleanEyes();
        
    // Handle hearts animation drawing
    if (hearts)
    {
        unsigned long elapsed = millis() - heartsAnimationTimer;
        float progress = (float)elapsed / heartsAnimationDuration;
        
        if (progress <= 0.5f)
        {
            // Transforming to hearts (0% to 50% of animation)
            float transformProgress = progress * 2.0f; // Scale to 0.0 - 1.0
            
            // Calculate heart positions - keep them centered where eyes are
            int centerX = screenWidth / 2;
            int centerY = screenHeight / 2;
            
            // Heart size based on eye size and transformation progress
            int heartSize = eyeLwidthDefault * transformProgress;
            
            // Position hearts at the same location as eyes would be
            int heartLx = eyeLx + (eyeLwidthCurrent - heartSize) / 2;
            int heartLy = eyeLy + (eyeLheightCurrent - heartSize) / 2;
            int heartRx = eyeRx + (eyeRwidthCurrent - heartSize) / 2;
            int heartRy = eyeRy + (eyeRheightCurrent - heartSize) / 2;
            
            // Draw hearts
            drawHeart(heartLx, heartLy, heartSize, HEARTCOLOR);
            if (!cyclops)
            {
                drawHeart(heartRx, heartRy, heartSize, HEARTCOLOR);
            }
        }
        else
        {
            // Transforming back from hearts (50% to 100% of animation)
            float transformProgress = 1.0f - ((progress - 0.5f) * 2.0f); // Scale from 1.0 to 0.0
            
            // Calculate heart positions
            int centerX = screenWidth / 2;
            int centerY = screenHeight / 2;
            
            // Heart size based on transformation progress
            int heartSize = eyeLwidthDefault * transformProgress;
            
            // Position hearts at the same location as eyes
            int heartLx = eyeLx + (eyeLwidthCurrent - heartSize) / 2;
            int heartLy = eyeLy + (eyeLheightCurrent - heartSize) / 2;
            int heartRx = eyeRx + (eyeRwidthCurrent - heartSize) / 2;
            int heartRy = eyeRy + (eyeRheightCurrent - heartSize) / 2;
            
            // Draw hearts with decreasing size
            drawHeart(heartLx, heartLy, heartSize, HEARTCOLOR);
            if (!cyclops)
            {
                drawHeart(heartRx, heartRy, heartSize, HEARTCOLOR);
            }
        }
    }
    else
    {
        // Draw basic eye rectangles
        sprite->fillRoundRect(eyeLx, eyeLy, eyeLwidthCurrent, eyeLheightCurrent, eyeLborderRadiusCurrent, MAINCOLOR); // left eye
        if (!cyclops)
        {
            sprite->fillRoundRect(eyeRx, eyeRy, eyeRwidthCurrent, eyeRheightCurrent, eyeRborderRadiusCurrent, MAINCOLOR); // right eye
        }
        // Prepare mood type transitions (only if not falling asleep)
        if (!fallingAsleep)
        {
            if (tired)
            {
                eyelidsTiredHeightNext = eyeLheightCurrent / 2;
                eyelidsAngryHeightNext = 0;
            }
            else
            {
                eyelidsTiredHeightNext = 0;
            }
            if (angry)
            {
                eyelidsAngryHeightNext = eyeLheightCurrent / 2;
                eyelidsTiredHeightNext = 0;
            }
            else
            {
                eyelidsAngryHeightNext = 0;
            }
            if (happy)
            {
                eyelidsHappyBottomOffsetNext = eyeLheightCurrent / 2;
            }
            else
            {
                eyelidsHappyBottomOffsetNext = 0;
            }
        }
        else
        {
            // Disable mood effects during falling asleep
            eyelidsTiredHeightNext = 0;
            eyelidsAngryHeightNext = 0;
            eyelidsHappyBottomOffsetNext = 0;
        }
        // Draw tired top eyelids
        eyelidsTiredHeight = (eyelidsTiredHeight + eyelidsTiredHeightNext) / 2;
        if (!cyclops)
        {
            sprite->fillTriangle(eyeLx, eyeLy - 1, eyeLx + eyeLwidthCurrent, eyeLy - 1, eyeLx, eyeLy + eyelidsTiredHeight - 1, BGCOLOR);                    // left eye
            sprite->fillTriangle(eyeRx, eyeRy - 1, eyeRx + eyeRwidthCurrent, eyeRy - 1, eyeRx + eyeRwidthCurrent, eyeRy + eyelidsTiredHeight - 1, BGCOLOR); // right eye
        }
        else
        {
            // Cyclops tired eyelids
            sprite->fillTriangle(eyeLx, eyeLy - 1, eyeLx + (eyeLwidthCurrent / 2), eyeLy - 1, eyeLx, eyeLy + eyelidsTiredHeight - 1, BGCOLOR);                                       // left eyelid half
            sprite->fillTriangle(eyeLx + (eyeLwidthCurrent / 2), eyeLy - 1, eyeLx + eyeLwidthCurrent, eyeLy - 1, eyeLx + eyeLwidthCurrent, eyeLy + eyelidsTiredHeight - 1, BGCOLOR); // right eyelid half
        }
        // Draw angry top eyelids
        eyelidsAngryHeight = (eyelidsAngryHeight + eyelidsAngryHeightNext) / 2;
        if (!cyclops)
        {
            sprite->fillTriangle(eyeLx, eyeLy - 1, eyeLx + eyeLwidthCurrent, eyeLy - 1, eyeLx + eyeLwidthCurrent, eyeLy + eyelidsAngryHeight - 1, BGCOLOR); // left eye
            sprite->fillTriangle(eyeRx, eyeRy - 1, eyeRx + eyeRwidthCurrent, eyeRy - 1, eyeRx, eyeRy + eyelidsAngryHeight - 1, BGCOLOR);                    // right eye
        }
        else
        {
            // Cyclops angry eyelids
            sprite->fillTriangle(eyeLx, eyeLy - 1, eyeLx + (eyeLwidthCurrent / 2), eyeLy - 1, eyeLx + (eyeLwidthCurrent / 2), eyeLy + eyelidsAngryHeight - 1, BGCOLOR);                    // left eyelid half
            sprite->fillTriangle(eyeLx + (eyeLwidthCurrent / 2), eyeLy - 1, eyeLx + eyeLwidthCurrent, eyeLy - 1, eyeLx + eyeLwidthCurrent / 2, eyeLy + eyelidsAngryHeight - 1, BGCOLOR); // right eyelid half
        }
        // Draw happy bottom eyelids
        eyelidsHappyBottomOffset = (eyelidsHappyBottomOffset + eyelidsHappyBottomOffsetNext) / 2;
        sprite->fillRoundRect(eyeLx - 1, (eyeLy + eyeLheightCurrent) - eyelidsHappyBottomOffset + 1, eyeLwidthCurrent + 2, eyeLheightDefault, eyeLborderRadiusCurrent, BGCOLOR); // left eye
        if (!cyclops)
        {
            sprite->fillRoundRect(eyeRx - 1, (eyeRy + eyeRheightCurrent) - eyelidsHappyBottomOffset + 1, eyeRwidthCurrent + 2, eyeRheightDefault, eyeRborderRadiusCurrent, BGCOLOR); // right eye
        }
    }
    
    sprite->pushSprite(0, 0);
}