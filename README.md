# Rocket-League

## Description
This is a real basic version of the game Rocket League. Aka, soccer with cars. This basic version just has a rectangular prism as the player's car, the ball, a playing field, and no other objects or opponents. You can move the car around and try to hit the ball into the opposite goal. The ball has a random launch vector when it spawns. The main goal was just to understand translation, scaling, and rotation matrices better, as well as do some physics simulation.

## Tools Used
- OpenGL 2 immediate mode for the drawing
- SDL2 for the windowing and user input

## Features
- Custom play field created using basic geometric shapes
- Player controlled car
- Realistic collisions done calculating or estimating the reflection vectors of collisions
- Gravity, friction, and thrust on the ball and car

## Controls
- Arrow Keys: Move car
- Spacebar: Relaunch the ball

## Installing
On Linux:
```sh
g++ main.cpp -lGL -lGLU -lSDL2
```
