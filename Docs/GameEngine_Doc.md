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
7. [GameEngine](#gameengine)

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
    1) BLACK
    2) DARK_BLUE
    3) DARK_GREEN
    4) DARK_CYAN
    5) DARK_RED
    6) DARK_MAGENTA
    7) DARK_GREY
    8) DARK_ORANGE
    9) DARK_BROWN
    10) DARK_PURPLE
    11) ORANGE
    12) GREY
    13) BLUE
    14) GREEN
    15) CYAN
    16) RED
    17) MAGENTA
    18) YELLOW
    19) WHITE
    20) GOLD
    21) PINK
    22) MAROON
    23) LIME
    24) BROWN
    25) BEIGE
    26) VIOLET
    27) PURPLE
    28) NONE

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
Stores info about a texture that you can load from a file or create your own from a sprite

### Fields
- **id** - OpenGL id of a texture (used internaly)
- **uvScale** - simply 1 / **size** (also used internaly)
- **size** - size of the texture

### Methods
- **Load(sprite)** - creates a texture from a sprite
- **Update(sprite)** - updates a texture using sprite data

## Graphic

### Description
Combines texture and sprite instances

### Fields
- **texture** - stores a texture
- **sprite** - stores a sprite

### Methods
- **Load(fileName)** - loads a texture and a sprite from a file
- **Load(size)** - creates a blank sprite and a blank texture
- **Save(fileName, type)** - saves sprite to a file
- **UpdateTexture()** - updates content of a texture with sprite data

## GameEngine

### Description
The main class that you need to inherit from, provides functionality for input handling and drawing

### Methods

- **OnUserCreate()** - you **must** override it and the method is called before the start of the application, return true if you want to continue an execution and return false if you want to terminate the application, this method can be used for initialising variables (e.g. def::Sprite*)

- **OnUserUpdate(deltaTime)** - this method is called on every frame of the application and provides **deltaTime** that stores time between the current frame and the last frame in seconds (you also **must** override it)

- **OnAfterDraw()** - this method is called before flushing the screen buffer and after drawing layers

- **OnTextCapturingComplete(text)** - is called when a user presses the **def::ENTER** key and it stores the entered text in **text**

- **OnConsoleCommand** - is called when a user presses the **def::ENTER** key when the console is opened and stores the entered prompt in **command**, you can respond to **output** and set a colour of the respondence in **colour**

- **Construct(screenWidth, screenHeight, pixelWidth, pixelHeight, fullScreen, vsync, dirtyPixel)** - this method **must** be called in the **main** function and it basically constructs a window (arguments are 
self-explanatory)

- **Run()** - call it when you want to start your application (most of the time after calling the **Construct** method)

    Example:
    ```cpp
    App app;

    if (app.Construct(256, 240, 4, 4))
        app.Run();
    ```

- **Draw(x, y, colour)** - draws a single pixel on the screen at **x** and **y** coordinates with **colour**

- **DrawLine(x1, y1, x2, y2, colour)** - draws a line with start at **x1** and **y1** and end in the **x2** and **y2** coordinates

- **DrawTriangle(x1, y1, x2, y2, x3, y3, colour)** - draws a triangle at the specified coordinates

- **FillTriangle(x1, y1, x2, y2, x3, y3, colour)** - draws a filled triangle with **colour** colour

- **DrawRectangle(x, y, sizeX, sizeY, colour)** - draws a rectangle at **x** and **y** with the size of (**sizeX**, **sizeY**)

- **FillRectangle(x, y, sizeX, sizeY, colour)** - draws a filled rectangle with **colour** colour at **x** and **y** with the size of (**sizeX**, **sizeY**)

- **DrawCircle(x, y, radius, colour)** - draws a circle with the center at (**x**, **y**)

- **FillCircle(x, y, radius, colour)** - draws a filled circle with **colour** colour

- **DrawEllipse(x, y, sizeX, sizeY, colour)** - draws an ellipse with a start at (**x**, **y**) and the size of (**sizeX**, **sizeY**)

- **FillEllipse(x, y, sizeX, sizeY, colour)** - draws a filled ellipse with a start at (**x**, **y**) and the size of (**sizeX**, **sizeY**)


