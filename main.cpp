#include "engine.hpp"
#include "draw.hpp"
#include <glm/glm.hpp>
#include <cstdlib>
#include <iostream>
using namespace std;
using glm::vec2;
using glm::vec3;
using glm::vec4;

void printVec(vec3 v) {
    cout << v.x << " " << v.y << " " << v.z << endl;
}

class Car {
public:
    vec3 position;
    vec3 velocity;
    vec3 dimensions;
    float collisionRadius;
    float thrustFactor, dragFactor;
    float turnRate;
    float directionAngle;
    void draw() {
        glColor3f(0.8,0.2,0.2);
        Draw::rect3(position, dimensions, directionAngle);
        // printVec(position);
    }

    void launch() {
        position = vec3(-1.5, .5, 45); // center of car is 1 m above ground
        velocity = vec3(0, 0, 0);
        directionAngle = 0;
    }
};

class Ball {
public:
    vec3 position;
    float radius;
    vec3 velocity;
    vec3 dimensions;
    float dragFactor;
    void draw() {
        glColor3f(1,1,1);
        Draw::ball(position, radius);
        Draw::disk(position, radius);
    }

    void launch() {
        position = vec3(0, 5, 0);
        double scaler = 25;
        double a = rand() / static_cast<double>(RAND_MAX) * M_PI;
        velocity = vec3(scaler * cos(a), 10, scaler*sin(a));
        // velocity = vec3(50,50,-50);
    }
};

class CarSoccer: public Engine {
public:
    SDL_Window *window;
    Car car;
    Ball ball;

    CarSoccer() {
        srand(time(0));
        window = createWindow("Car Soccer", 1280, 720);
        car.collisionRadius = 2.5;
        car.dimensions = vec3(3, 2, 4);
        car.thrustFactor = 110;
        car.dragFactor = 4;
        car.turnRate = 10;
        car.launch();

        ball.radius = 2;
        ball.dragFactor = .2;
        ball.launch();
    }

    ~CarSoccer() {
        SDL_DestroyWindow(window);
    }

    void run() {
        float fps = 60, timeStep = 1/fps;
        while (!shouldQuit()) {
            Engine::handleInput();
            simulate(timeStep);
            drawGraphics();
            Engine::waitForNextFrame(timeStep);
        }
    }

    void simulate(float timeStep) {
        // An oversimplified dynamics model for the car
        /*
        vec2 dir = getControlDirection();
        if (glm::length(dir) > 0)
            dir = glm::normalize(dir);
        vec3 thrust = car.thrustFactor*vec3(dir.x, 0, -dir.y);
        vec3 drag = car.dragFactor*car.velocity;
        car.velocity += (thrust - drag)*timeStep;
        car.position += car.velocity*timeStep;
        */
        vec2 dir = getControlDirection();
        if (glm::length(dir) > 0)
            dir = glm::normalize(dir);
        float speed = glm::length(car.velocity);
        car.directionAngle += car.turnRate * dir.x * speed * timeStep;
        // cout << car.directionAngle << endl;
        float a = (car.directionAngle + 180) * M_PI / 180.0;
        vec3 thrust = dir.y*car.thrustFactor*vec3(-sin(a), 0, cos(a));
        vec3 drag = car.dragFactor*car.velocity;
        // car.velocity = vec3(speed * sin(a), car.velocity.y, speed * cos(a));
        car.velocity += (thrust - drag)*timeStep;
        car.position += car.velocity*timeStep;

        drag = ball.dragFactor*ball.velocity;
        ball.velocity -= drag*timeStep;
        ball.velocity.y -= 9.81 * timeStep * 2.5;
        ball.position += ball.velocity * timeStep;

        // Handle ball/wall, car/wall, and ball/car collisions here

        bool goal = false;

        // Ball / Wall

        bool collision = false;
        // Floor
        if(ball.position.y - ball.radius < 0) {
            ball.position.y = ball.radius;
            ball.velocity.y *= -1;
            collision = true;
        }

        // Ceiling
        else if(ball.position.y + ball.radius > 35) {
            ball.position.y = 35 - ball.radius;
            ball.velocity.y *= -1;
            collision = true;
        }

        // Left side
        if(ball.position.x - ball.radius < -40) {
            ball.position.x = -40 + ball.radius;
            ball.velocity.x *= -1;
            collision = true;
        }

        // Right side
        else if(ball.position.x + ball.radius > 40) {
            ball.position.x = 40 - ball.radius;
            ball.velocity.x *= -1;
            collision = true;
        }

        // Towards screen
        if(ball.position.z + ball.radius > 50) {
            if (ball.position.x <= 10 && ball.position.x >= -10 && ball.position.y <= 10) {
                goal = true;
            } else {
                ball.position.z = 50 - ball.radius;
                ball.velocity.z *= -1;
                collision = true;
            }
        }

        // Away from screen
        else if(ball.position.z - ball.radius < -50) {
            if (ball.position.x <= 10 && ball.position.x >= -10 && ball.position.y <= 10) {
                goal = true;
            } else {
                ball.position.z = -50 + ball.radius;
                ball.velocity.z *= -1;
                collision = true;
            }
        }

        if (collision)
            ball.velocity *= .8;
        if (goal) {
            ball.launch();
            car.launch();
        }

        // Car / wall
        /*
        // Floor
        if(car.position.y - car.dimensions.y / 2 < 0) {
            car.position.y = car.dimensions.y / 2;
            car.velocity.y = 0;
        }

        // Ceiling
        else if(car.position.y + car.dimensions.y / 2 > 35) {
            car.position.y = 35 - car.dimensions.y / 2;
            car.velocity.y = 0;
        }

        // Left side
        if(car.position.x - car.dimensions.x / 2 < -40) {
            car.position.x = -40 + car.dimensions.x / 2;
            car.velocity.x = 0;
        }

        // Right side
        else if(car.position.x + car.dimensions.x / 2 > 40) {
            car.position.x = 40 - car.dimensions.x / 2;
            car.velocity.x = 0;
        }

        // Towards screen
        if(car.position.z + car.dimensions.z / 2 > 50) {
            car.position.z = 50 - car.dimensions.z / 2;
            car.velocity.z = 0;
        }

        // Away from screen
        else if(car.position.z - car.dimensions.z / 2 < -50) {
            car.position.z = -50 + car.dimensions.z / 2;
            car.velocity.z = 0;
        }
        */
        // Floor
        if(car.position.y - car.dimensions.y / 2 < 0) {
            car.position.y = car.dimensions.y / 2;
            car.velocity.y = 0;
        }

        // Ceiling
        else if(car.position.y + car.dimensions.y / 2 > 35) {
            car.position.y = 35 - car.dimensions.y / 2;
            car.velocity.y = 0;
        }

        // Left side
        if(car.position.x - car.dimensions.x / 2 < -40) {
            car.position.x = -40 + car.dimensions.x / 2;
            car.velocity.x = 0;
        }

        // Right side
        else if(car.position.x + car.dimensions.x / 2 > 40) {
            car.position.x = 40 - car.dimensions.x / 2;
            car.velocity.x = 0;
        }

        // Towards screen
        if(car.position.z + car.dimensions.z / 2 > 50) {
            car.position.z = 50 - car.dimensions.z / 2;
            car.velocity.z = 0;
        }

        // Away from screen
        else if(car.position.z - car.dimensions.z / 2 < -50) {
            car.position.z = -50 + car.dimensions.z / 2;
            car.velocity.z = 0;
        }


        // Car / Ball
        if (glm::length(ball.position - car.position) < car.collisionRadius + ball.radius)
        {
            vec3 norm = glm::normalize(ball.position - car.position);
            ball.position = car.position + (car.collisionRadius + ball.radius) * norm;
            vec3 rel = ball.velocity - car.velocity;
            ball.velocity = car.velocity + glm::reflect(rel, norm);
            // ball.velocity = 50.0f * ball.velocity;
            // printVec(ball.velocity);
            // cout << "collision" << endl;
        }

    }

    vec2 getControlDirection() {
        vec2 dir(0,0);
        if (Engine::isKeyDown(SDL_SCANCODE_LEFT))
            dir.x--;
        if (Engine::isKeyDown(SDL_SCANCODE_RIGHT))
            dir.x++;
        if (Engine::isKeyDown(SDL_SCANCODE_UP))
            dir.y++;
        if (Engine::isKeyDown(SDL_SCANCODE_DOWN))
            dir.y--;
        return dir;
    }

    void addLight(GLenum light, vec4 position, vec3 color) {
        glEnable(light);
        glLightfv(light, GL_POSITION, &position[0]);
        glLightfv(light, GL_DIFFUSE, &color[0]);
    }
    
    void drawGraphics() {
        // Allow lines to show up on top of filled polygons
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1,1);
        // Set up the camera in a good position to see the entire field
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60, 16/9., 1,1000);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0,60,70, 0,0,10, 0,1,0);
        // Clear window
        glClearColor(0.2,0.2,0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Add some lights
        addLight(GL_LIGHT0, vec4(+1,0.5,+1,0), vec3(0.5,0.5,0.5));
        addLight(GL_LIGHT1, vec4(+1,0.5,-1,0), vec3(0.5,0.5,0.5));
        addLight(GL_LIGHT2, vec4(-1,0.5,-1,0), vec3(0.5,0.5,0.5));
        addLight(GL_LIGHT3, vec4(-1,0.5,+1,0), vec3(0.5,0.5,0.5));
        glEnable(GL_LIGHTING);
        // Draw the ground as a quadrilateral
        glColor3f(0.3,0.6,0.2);
        glBegin(GL_QUADS);
        glNormal3f(0,1,0);
        glVertex3f(-50,0,-60);
        glVertex3f(+50,0,-60);
        glVertex3f(+50,0,+60);
        glVertex3f(-50,0,+60);
        glEnd();

        // Draw the car
        car.draw();
        
        // Draw the field borders, the pitch markings, and the goals here
        glEnable(GL_LIGHTING);
        glColor3f(1, 1, 1);
        Draw::circleXZ(vec3(0, 0, 0), 10);
        Draw::rect2(vec3(-40, 0, 50), vec3(0, 0, -100), vec3(80, 0, 0));
        Draw::rect2(vec3(-20, 0, 50), vec3(0, 0, -15), vec3(40, 0, 0));
        Draw::rect2(vec3(-20, 0, -50), vec3(0, 0, 15), vec3(40, 0, 0));
        Draw::rect2(vec3(-40, 35, 50), vec3(0, 0, -100), vec3(80, 0, 0));
        Draw::line(vec3(-40, 0, 0), vec3(40, 0, 0));

        Draw::line(vec3(-40, 0, 50), vec3(-40, 35, 50));
        Draw::line(vec3(-40, 0, -50), vec3(-40, 35, -50));
        Draw::line(vec3(40, 0, -50), vec3(40, 35, -50));
        Draw::line(vec3(40, 0, 50), vec3(40, 35, 50));

        glDisable(GL_LIGHTING);

        // goals
        for (int i = 0; i < 2; i++) {
            glColor3f(1 - i, 1, i);
            for (int j = 0; j <= 20; j++) {
                Draw::line(vec3(-10 + j, 0, 50 - 100 * i), vec3(-10 + j, 10, 50 - 100 * i));
            }
            for (int j = 0; j <= 10; j++) {
                Draw::line(vec3(-10, j, 50 - 100 * i), vec3(10, j, 50 - 100 * i));
            }
        }

        // Draw the ball
        ball.draw();

        SDL_GL_SwapWindow(window);
    }

    void onKeyDown(SDL_KeyboardEvent &e) {
        if (Engine::isKeyDown(SDL_SCANCODE_SPACE)) {
            ball.launch();
        }

        // Check whether the space bar was pressed, and if so, reset the ball
        // to the center of the pitch and give it a "kick-off" velocity.
        // We found that a nice initial velocity is (25 cos(a), 10, 25 sin(a))
        // where a is a random number between 0 and pi.

    }
};

int main(int argc, char *argv[]) {
    CarSoccer game;
    game.run();
    return 0;
}
