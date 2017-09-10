#ifndef DRAW_HPP
#define DRAW_HPP

#define _USE_MATH_DEFINES
#include <cmath>
#include <GL/glu.h>
#include <glm/vec3.hpp>
using glm::vec3;

namespace Draw {

    void axes();
    void unitCube();
    void unitSphere();
    void rect3(vec3 pos, vec3 dim, float angle);
    void rect2(vec3 p1, vec3 len, vec3 height);
    void circleXZ(vec3 point, float radius);
    void line(vec3 p1, vec3 p2);
    inline void ball(vec3 pos, float r);
    inline void disk(vec3 point, float radius);

    // Definitions below

    inline void axes() {
        glBegin(GL_TRIANGLES);
        glColor3f(1,0,0);
        glVertex3f(0,-0.2,0);
        glVertex3f(1, 0.0,0);
        glVertex3f(0,+0.2,0);
        glColor3f(0,1,0);
        glVertex3f(0,0,-0.2);
        glVertex3f(0,1, 0.0);
        glVertex3f(0,0,+0.2);
        glColor3f(0,0,1);
        glVertex3f(-0.2,0,0);
        glVertex3f( 0.0,0,1);
        glVertex3f(+0.2,0,0);
        glEnd();
    }

    inline void unitCube() {
        vec3 verts[8] = {
            vec3(-.5,-.5,-.5),
            vec3(.5,-.5,-.5),
            vec3(-.5,.5,-.5),
            vec3(.5,.5,-.5),
            vec3(-.5,-.5,.5),
            vec3(.5,-.5,.5),
            vec3(-.5,.5,.5),
            vec3(.5,.5,.5)
            /*
            vec3(0,0,0),
            vec3(1,0,0),
            vec3(0,1,0),
            vec3(1,1,0),
            vec3(0,0,1),
            vec3(1,0,1),
            vec3(0,1,1),
            vec3(1,1,1)
            */
        };
        int faces[6][4] = {
            {0,2,3,1},
            {0,4,6,2},
            {0,1,5,4},
            {4,5,7,6},
            {1,3,7,5},
            {2,6,7,3}
        };
        vec3 normals[6] = {
            vec3(0,0,-1),
            vec3(-1,0,0),
            vec3(0,-1,0),
            vec3(0,0,+1),
            vec3(+1,0,0),
            vec3(0,+1,0)
        };
        glBegin(GL_QUADS);
        for (int f = 0; f < 6; f++) {
            vec3 n = normals[f];
            glNormal3f(n.x,n.y,n.z);
            for (int i = 0; i < 4; i++) {
                vec3 v = verts[faces[f][i]];
                glVertex3f(v.x,v.y,v.z);
            }
        }
        glEnd();
    }

    inline void rect3(vec3 pos, vec3 dim, float angle) {
        glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);
        glRotatef(-angle, 0, 1, 0);
        glScalef(dim.x, dim.y, dim.z);
        Draw::unitCube();
        glPopMatrix();
    }

    inline void ball(vec3 pos, float r) {
        glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);
        glScalef(r, r, r);
        Draw::unitSphere();
        glPopMatrix();
    }

    inline void rect2(vec3 p1, vec3 len, vec3 height) {
        glLineWidth(1);
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINE_LOOP);
        glVertex3f(p1.x + len.x, p1.y + len.y, p1.z + len.z);
        glVertex3f(p1.x + height.x + len.x, p1.y + height.y + len.y, p1.z + height.z + len.z);
        glVertex3f(p1.x + height.x, p1.y + height.y, p1.z + height.z);
        glVertex3f(p1.x, p1.y, p1.z);
        glEnd();
    }

    inline void circleXZ(vec3 point, float radius) {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINE_LOOP);
        int points = 40;
        for(int i = 0; i < points; i++) {
            float t = 2*M_PI * i / points;
            glVertex3f(point.x + radius * cos(t), point.y, point.z + radius*sin(t));
        }
        glEnd();
    }

    inline void disk(vec3 point, float radius) {
        glColor3f(.5, .5, .5);
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(point.x, .1, point.z);
        int points = 40;
        for(int i = 0; i <= points; i++) {
            float x = point.x + radius*cos(2*M_PI * i / points);
            float z = point.z + radius*sin(2*M_PI * i / points);
            glVertex3f(x, .1, z);
        }
        glEnd();
    }

    inline void line(vec3 p1, vec3 p2) {
        glBegin(GL_LINES);
        glVertex3f(p1.x, p1.y, p1.z);
        glVertex3f(p2.x, p2.y, p2.z);
        glEnd();
    }

    inline void unitSphere() {
        static GLUquadric* quadric = gluNewQuadric();
        gluSphere(quadric, 1, 40, 40);
    }

}

#endif
