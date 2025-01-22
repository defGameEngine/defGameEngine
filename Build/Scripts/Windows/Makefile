CC=g++

CC_GLFW_INCLUDE=C:\SDK\glfw\include
CC_GLFW_LIB=C:\SDK\glfw\x64\lib-mingw-w64
CC_STB_INCLUDE=C:\SDK\stb
CC_LIBS=-lgdi32 -luser32 -lkernel32 -lopengl32 -lglfw3dll -lglu32

CC_FLAGS=-Wall -pedantic -std=c++20 -fms-extensions
CC_DEPENDENCIES=-I"$(CC_STB_INCLUDE)" -I"$(CC_GLFW_INCLUDE)" -L"$(CC_GLFW_LIB)" $(CC_LIBS)

CC_ENGINE_SOURCES=../../../Sources/defGameEngine.cpp ../../../Sources/Graphic.cpp ../../../Sources/KeyState.cpp \
					../../../Sources/Pixel.cpp ../../../Sources/PlatformGL.cpp ../../../Sources/PlatformGLFW3.cpp \
					../../../Sources/Sprite.cpp ../../../Sources/StbImage.cpp ../../../Sources/Texture.cpp

define compile
	$(CC) -I"../../../Include" $(CC_FLAGS) -o ../../Target/$(1) $(CC_ENGINE_SOURCES) ../../../Examples/$(1).cpp $(CC_DEPENDENCIES)
endef

.PHONY: all
all:
	$(call compile,Tetris)
	$(call compile,Asteroids)
	$(call compile,AxisKiller)
	$(call compile,BreakOut)
	$(call compile,CAD)
	$(call compile,Camera2D)
	$(call compile,Checkers)
	$(call compile,ColorPalette)
	$(call compile,ConsoleExample)
	$(call compile,Cube3D)
	$(call compile,DDA_Raycaster2D)
	$(call compile,DDA_Raycaster3D)
	$(call compile,Fireworks)
	$(call compile,Jackpot)
	$(call compile,Matrices)
	$(call compile,Mazes)
	$(call compile,Mode7)
	$(call compile,PathFinder)
	$(call compile,PerlinNoise)
	$(call compile,PlatformGame)
	$(call compile,Plots)
	$(call compile,Pool)
	$(call compile,SlidingPuzzle)
	$(call compile,Snake)
	$(call compile,Snow)
	$(call compile,Splines)
	$(call compile,StarField)
	$(call compile,Timer)
	$(call compile,Yahtzee)
