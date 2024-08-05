#include <windows.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>
#include <toolbox.h>
#include "oglcarsim.h"
#include "cviogl.h"

extern double baseRotation;
extern double elbowRotation; // Steering angle

// Function prototypes
void drawWheelAndBolts(GLUquadric* quad);
void drawCar(GLUquadric* quad);
void drawGrid(float offset);
void drawAxleSystem(GLUquadric* quad);

// Draws a larger circular grid with consistent line thickness
void drawGrid(float offset)
{
    glPushMatrix();
        glColor3f(1.0f, 0.0f, 1.0f);
        glLineWidth(2.0);
    
        float gridSize = 20.0f;
        int lineCount = 81;
        float lineSpacing = (2 * gridSize) / (lineCount - 1);
    
        // Horizontal lines
        glBegin(GL_LINES);
        for (int i = 0; i < lineCount; i++)
        {
            float y = -gridSize + lineSpacing * i;
            for (float x = -gridSize; x <= gridSize; x += 0.2f)
            {
                float x1 = x + fmod(offset, lineSpacing);
                if (x1 > gridSize) x1 -= 2 * gridSize;
                glVertex2f(x1, y);
                glVertex2f(x1 + 0.2f, y);
            }
        }
        glEnd();
    
        // Vertical lines
        glBegin(GL_LINES);
        for (i = 0; i < lineCount; i++)
        {
            float x = -gridSize + lineSpacing * i + fmod(offset, lineSpacing);
            if (x > gridSize) x -= 2 * gridSize;
            for (float y = -gridSize; y <= gridSize; y += 0.2f)
            {
                glVertex2f(x, y);
                glVertex2f(x, y + 0.2f);
            }
        }
        glEnd();
    glPopMatrix();
}

// Drawing a wheel with bolts
void drawWheelAndBolts(GLUquadric* quad)
{
    glPushMatrix();    
        glColor3f(0.5f, 0.5f, 0.5f); // Gray color for the axes
        // Draw the wheel
        glRotatef(90.0, 1.0, 0.0, 0.0);    
        glPushMatrix();        
            glTranslatef(0.0, 0.0, 0.05); // Translate disc to center of the wheel
            gluDisk(quad, 0.0, 0.4, 30, 1); // Inner radius, outer radius, slices, loops
        glPopMatrix();
        gluCylinder(quad, 0.4, 0.4, 0.1, 30, 1); // Add a cylinder to give thickness
        
        // Draw five bolts around the wheel
        for (int i = 0; i < 5; i++)
        {
            glPushMatrix();
                glRotatef(i * 72.0, 0.0, 0.0, 1.0); // Rotate bolt position
                glTranslatef(0.3, 0.0, 0.0); // Move bolt away from the center of the wheel
                gluCylinder(quad, 0.04, 0.04, 0.1, 10, 10); // Draw bolt
            glPopMatrix();
        }
    glPopMatrix();
}

// Draws the axle system using spheres and cylinders
void drawAxleSystem(GLUquadric* quad)
{
    glColor3f(0.5f, 0.5f, 0.5f); // Gray color for the axles

    // Draw front axle
    glPushMatrix();
        glTranslatef(-0.75, 0.5, 0.0);
        glRotatef(90.0, 1.0, 0.0, 0.0); // Align with y-axis
        gluCylinder(quad, 0.04, 0.04, 1.0, 20, 1); // Front axle
    glPopMatrix();

    // Draw rear axle
    glPushMatrix();
        glTranslatef(0.75, 0.5, 0.0);
        glRotatef(90.0, 1.0, 0.0, 0.0); // Align with y-axis
        gluCylinder(quad, 0.04, 0.04, 1.0, 20, 1); // Auxiliary axle
    glPopMatrix();

    // Connecting rod
    glPushMatrix();
        glTranslatef(-0.75, 0.0, 0.0);
		 glRotatef(90.0, 0.0, 1.0, 0.0); // Align with y-axis
        gluCylinder(quad, 0.04, 0.04, 1.5, 20, 1); // Connecting rod 1
    glPopMatrix();

    // Spheres at joints
    glColor3f(0.7f, 0.7f, 0.7f); // Light gray for spheres
    glPushMatrix();
        glTranslatef(-0.75, 0.5, 0.0);
        gluSphere(quad, 0.06, 20, 20); // Sphere at joint 1
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-0.75, -0.5, 0.0);
        gluSphere(quad, 0.06, 20, 20); // Sphere at joint 2
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.75, 0.5, 0.0);
        gluSphere(quad, 0.06, 20, 20); // Sphere at joint 3
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.75, -0.5, 0.0);
        gluSphere(quad, 0.06, 20, 20); // Sphere at joint 4
    glPopMatrix();
}

// Drawing the complete car using a quadric
void drawCar(GLUquadric* quad)
{
    // Calculate the steering angles for the Ackermann geometry
    double wheelBase = 2.0; // Distance between the axles
    double trackWidth = 1.2; // Distance between the wheels on an axle

    // Calculate steering angles based on Ackermann geometry
    double innerWheelAngle = atan(wheelBase / (wheelBase / tan(elbowRotation * PI / 180.0) - trackWidth / 2)) * 180.0 / PI;
    double outerWheelAngle = atan(wheelBase / (wheelBase / tan(elbowRotation * PI / 180.0) + trackWidth / 2)) * 180.0 / PI;

    glPushMatrix();
        glTranslatef(0.0, 0.0, 0.4); // Move the whole car up (z-axis)

        // Draw the axle system
        drawAxleSystem(quad);

		// Draw wheels at each corner of the car
		// Front wheels with steering angles
        glPushMatrix();
            glTranslatef(-0.75, 0.55, 0.0); // Position front-right wheel
            glRotatef(outerWheelAngle, 0.0, 0.0, 1.0); // Rotate wheel for steering
            glRotatef(baseRotation, 0.0, 1.0, 0.0); // Rotate the wheel around its rolling axis
            drawWheelAndBolts(quad);
        glPopMatrix();
    
        glPushMatrix();
            glTranslatef(-0.75, -0.45, 0.0); // Position front-left wheel
            glRotatef(innerWheelAngle, 0.0, 0.0, 1.0); // Rotate wheel for steering
            glRotatef(baseRotation, 0.0, 1.0, 0.0); // Rotate the wheel around its rolling axis
            drawWheelAndBolts(quad);
        glPopMatrix();
		
        // Rear wheels
        glPushMatrix();
            glTranslatef(0.75, 0.55, 0.0); // Position rear-right wheel
            glRotatef(baseRotation, 0.0, 1.0, 0.0); // Rotate the wheel around its rolling axis
            drawWheelAndBolts(quad);
        glPopMatrix();
        
        glPushMatrix();
            glTranslatef(0.75, -0.45, 0.0); // Position rear-left wheel
            glRotatef(baseRotation, 0.0, 1.0, 0.0); // Rotate the wheel around its rolling axis
            drawWheelAndBolts(quad);
        glPopMatrix();

    glPopMatrix();
}
