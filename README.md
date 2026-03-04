# LuLuEyes - Animated Robot Eyes Library for Arduino

A library for creating animated robot eyes with various moods, expressions, and animations for OLED displays using LGFX.

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

// Initialize display
LGFX lcd;

// Create eyes instance
LuLuEyes eyes;

void setup() {
  lcd.init();
  
  // Initialize eyes with screen dimensions
  eyes.begin(128, 64, &lcd);
  
  // Set eye size
  eyes.setWidth(80, 80);
  eyes.setHeight(80, 80);
  
  // Enable auto-blinking
  eyes.setAutoblinker(true, 2, 3);
}

void loop() {
  // Update eyes animation
  eyes.update();
  
  // Example: Set mood
  eyes.setMood(HAPPY);
  
  // Example: Play animation
  eyes.anim_hearts();
  
  delay(100);
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

Copyright (C) 2025 Artem Savkin

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.