# LuLuEyes - Animated Robot Eyes Library for Arduino

A library for creating animated robot eyes with various moods, expressions, and animations for displays using LGFX.

## Features

- **Multiple Moods**: Tired, Angry, Happy expressions
- **Animations**: Blinking, confused, laughing, hearts, falling asleep
- **Special Modes**: Cyclops mode, curious mode, idle mode
- **Flicker Effects**: Horizontal and vertical flickering
- **Customizable**: Adjust eye size, position, border radius, and spacing

## Installation

1. Copy the `LuLuEyes` folder to your Arduino libraries folder
2. Or install via Arduino Library Manager (if published)

## Dependencies

- [LovyanGFX](https://github.com/lovyan03/LovyanGFX) - Display driver library

## Example Usage

```cpp
#include <LovyanGFX.hpp>
#include <LuLuEyes.h>
#include <LGFX_AUTODETECT.hpp>  // クラス"LGFX"を用意します
// #include <lgfx_user/LGFX_ESP32_sample.hpp> // またはユーザ自身が用意したLGFXクラスを準備します

#define EYEBORDER 40

// Initialize display;
static LGFX gfx; 
static LGFX_Sprite *eyesSprite;

// Create eyes instance
LuLuEyes luluEyes;


void setup() {
    gfx.init();
    eyesSprite = new LGFX_Sprite(gfx);
    eyesSprite->setPsram(true);    
    eyesSprite->createSprite(gfx->width(), gfx->height() - EYEBORDER * 2);        
    luluEyes->begin(gfx->width(), gfx->height() - EYEBORDER * 2, eyesSprite); 
    luluEyes->setAutoblinker(ON, 3, 2); // Start auto blinker animation cycle -> bool active, int interval, int variation -> turn on/off, set interval between each blink in full seconds, set range for random interval variation in full seconds
    luluEyes->setIdleMode(ON, 2, 2);    
    luluEyes->setSpacebetween(40);
}

void loop() {
  luluEyes->update();
  delay(20);
}
```

## API Reference

### Initialization
- `begin(int width, int height, LGFX_Sprite* sprite)` - Initialize the eyes

### Basic Control
- `update()` - Update animation frame
- `setFramerate(byte fps)` - Set maximum frame rate

### Appearance
- `setWidth(byte leftEye, byte rightEye)` - Set eye width
- `setHeight(byte leftEye, byte rightEye)` - Set eye height
- `setBorderradius(byte leftEye, byte rightEye)` - Set border radius
- `setSpacebetween(int space)` - Set space between eyes

### Moods
- `setMood(unsigned char mood)` - Set mood (DEFAULT, TIRED, ANGRY, HAPPY)

### Position
- `setPosition(unsigned char position)` - Set position (N, NE, E, SE, S, SW, W, NW)

### Animations
- `setAutoblinker(bool active)` - Enable/disable auto-blinking
- `setAutoblinker(bool active, int interval, int variation)` - Configure auto-blinking
- `setIdleMode(bool active)` - Enable/disable idle mode
- `setCuriosity(bool curiousBit)` - Enable/disable curious mode
- `setCyclops(bool cyclopsBit)` - Enable/disable cyclops mode
- `setHFlicker(bool flickerBit)` - Enable/disable horizontal flicker
- `setVFlicker(bool flickerBit)` - Enable/disable vertical flicker

### Animation Methods
- `anim_confused()` - Play confused animation
- `anim_laugh()` - Play laugh animation
- `anim_hearts()` - Play hearts animation
- `anim_fallingAsleep()` - Play falling asleep animation
- `anim_wakeUp()` - Wake up after falling asleep

### Blink Control
- `close()` - Close both eyes
- `open()` - Open both eyes
- `blink()` - Blink both eyes
- `close(bool left, bool right)` - Close individual eyes
- `open(bool left, bool right)` - Open individual eyes
- `blink(bool left, bool right)` - Blink individual eyes

## Constants

### Moods
- `DEFAULT` - Normal expression
- `TIRED` - Tired expression
- `ANGRY` - Angry expression
- `HAPPY` - Happy expression

### Positions
- `N` - North (top center)
- `NE` - North-East (top right)
- `E` - East (middle right)
- `SE` - South-East (bottom right)
- `S` - South (bottom center)
- `SW` - South-West (bottom left)
- `W` - West (middle left)
- `NW` - North-West (top left)

## License

Copyright (C) 2026 Artem Savkin

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.