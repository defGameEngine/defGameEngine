# GameEngine Documentation

## Introduction

You can copy one of the sample games under the `Examples` folder of the repository into `.cpp` file of your project for a quick look and demo.
Emscripten demo: https://defini7.itch.io/defgameengine-raycaster.

## Table of Contents

### Classes and structures

1. [Vector2D](#vector2d)
2. [KeyState](#keystate)
3. [Pixel](#pixel)
4. [Sprite](#sprite)
5. [Texture](#texture)
6. [Graphic](#graphic)
7. [Layer](#layer)
8. [GameEngine](#gameengine)

## Vector2D

### Description
Stores **x** and **y** components and provides an useful set of methods and operations to manipulate them

### Fields
- **x** - first coordinate
- **y** - second coordinate

### Methods
- **Clamp(start, end)** - clamps each component withing the [*start*, *end*] range
- **Lerp(vector, time)** - linearly interpolates between *this* and *vector* by *time*
- **Distance(vector)** - computes distance between *this* and *vector*
- **DotProduct(vector)** - computes dot product of *this* and *vector*
- **CrossProduct(vector)** - computes cross product of *this* and *vector*
- **Angle(vector)** - computes angle between *this* and *vector* using dot product
- **Length2()** - returns squared length of a vector
- **Length()** - returns length of a vector
- **ManhattanDistance(vector)** - returns manhattan distance between *this* and *vector*
- **Max(vector)** - performs **std::max** on each component with *this* and *vector*
- **Min(vector)** - performs **std::min** on each component with *this* and *vector*
- **Swap(vector)** - swaps values of *this* and *vector* using **std::swap**
- **Normalise()** - normalises a vector
- **Abs()** - performs **std::abs** on each component of a vector
- **Perpendicular()** - gives a vector that's perpendicular to *this*
- **Floor()** - performs **std::floor** on each component
- **Ceil()** - performs **std::ceil** on each component
- **Round()** - performs **std::round** on each component
- **Cartesian()** - takes *x* as a radius and *y* as an angle and converts from polar to cartesian space
- **Polar()** - sets *x* to a radius and *y* to an angle and converts from cartesian to polar space
- **ToString()** - represents a vector as a string

### Types
- **Vector2i** - Vector2D with int
- **Vector2f** - Vector2D with float
- **Vector2d** - Vector2D with double

## KeyState

### Description
Stores a state of a key 

### Fields
- **held** - true if key has been held otherwise false
- **released** - true if key has been released otherwise false
- **pressed** - true if key has been pressed otherwise false

## Pixel

### Description
Stores pixel data in the RGBA format and provides an useful set of methods to manipulate them

### Fields
- **r** - red component
- **g** - green component
- **b** - blue component
- **a** - alpha component
- **rgba_n** - rgba as a 32-bit integer
- **rgba_v** - rgba as an array of 8-bit integers

### Methods
- **Lerp(pixel, factor)** - linearly interpolates between *this* and *vector* by factor
- **ToString()** - represents a pixel as a string
- **Float(r, g, b, a)** - constructs **def::Pixel** from normalised float values

### Constants
- **Mode**:
    1) **DEFAULT** - simply draws a pixel
    2) **ALPHA** - draws a pixel with alpha component enabled
    3) **MASK** - draws a pixel only when **a** == 255
    4) **CUSTOM** - draws a custom pixel (a.k.a. shaders)
- **Colours**:
*BLACK, DARK_BLUE, DARK_GREEN, DARK_CYAN, DARK_RED, DARK_MAGENTA, DARK_GREY, DARK_ORANGE, DARK_BROWN, DARK_PURPLE, ORANGE, GREY, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, WHITE, GOLD, PINK, MAROON, LIME, BROWN, BEIGE, VIOLET, PURPLE, NONE*

## Sprite

### Description
Stores info about an image, i.e. it's size and pixel data, you're able to
load images from and save them to files

### Fields
- **pixels** - an image by itself represented as a vector of pixels
- **size** - size of the image

### Methods
- **Create(size)** - allocates memory for a new sprite and clears the old one
- **Load(fileName)** - loads image from a file with **fileName** name
- **Save(fileName, type)** - saves all pixels to a specified file with **fileName** name and specified **type**
- **SetPixel(x, y, colour)** - sets **colour** at **x** and **y** coordinates
- **GetPixel(x, y, wrap)** - gets **colour** at the modified by the **wrap** method **x** and **y** coordinates
- **GetPixel(pos, wrap)** - the same as before but using **def::Vector2i**
- **SetPixelData(colour)** - fill the pixels vector with **colour**
- **Sample(x, y, sample, wrap)** - samples the pixel with **sample** and **wrap** methods

## Texture

### Description
Stores info about a texture that you can load from a file or create your own from sprite
