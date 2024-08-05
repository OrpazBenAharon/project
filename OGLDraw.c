#include <windows.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>
#include <toolbox.h>
#include "oglcarsim.h"
#include "cviogl.h"

// External variables for interaction with other parts of the program
extern double forwardPush;   // External push to the car
extern double steeringAngle;  // Steering angle

// Internal representation of the car's state
double carPositionX = 0.0;
double carPositionY = 0.0;
double carDirection = 0.0; // Angle in degrees
double prevForwardPush = 0.0; // To track changes in forwardPush

// Car geometry parameters
double wheelBase = 2.0; // Distance between the axles
double trackWidth = 1.2; // Distance between the wheels on an axle

// Function prototypes
void drawWheelAndBolts(GLUquadric* quad);
void drawCar(GLUquadric* quad);
void drawGrid();
void drawAxleSystem(GLUquadric* quad);
void updateCarPosition(double push);

// Draws a static grid
void drawGrid()
{
    glPushMatrix();
        glColor3f(1.0f, 0.0f, 1.0f);
        glLineWidth(3.0);

        float gridSize = 20.0f;
        int lineCount = 81;
        float lineSpacing = (2 * gridSize) / (lineCount - 1);

        // Horizontal lines
        glBegin(GL_LINES);
        for (int i = 0; i < lineCount; i++)
        {
            float y = -gridSize + lineSpacing * i;
            glVertex2f(-gridSize, y);
            glVertex2f(gridSize, y);
        }
        glEnd();

        // Vertical lines
        glBegin(GL_LINES);
        for (i = 0; i < lineCount; i++)
        {
            float x = -gridSize + lineSpacing * i;
            glVertex2f(x, -gridSize);
            glVertex2f(x, gridSize);
        }
        glEnd();
    glPopMatrix();
}

// Drawing a wheel with bolts
void drawWheelAndBolts(GLUquadric* quad)
{
    glPushMatrix();
        glColor3f(0.5f, 0.5f, 0.5f); // Gray color for the wheels

        // Draw the wheel
        glRotatef(90.0, 1.0, 0.0, 0.0);
        glPushMatrix();
            glTranslatef(0.0, 0.0, 0.05); // Translate disc to center of the wheel
            gluDisk(quad, 0.0, 0.25, 30, 1); // Inner radius, outer radius, slices, loops
        glPopMatrix();
        gluCylinder(quad, 0.25, 0.25, 0.1, 30, 1); // Add a cylinder to give thickness

        // Draw five bolts around the wheel
        for (int i = 0; i < 5; i++)
        {
            glPushMatrix();
                glRotatef(i * 72.0, 0.0, 0.0, 1.0); // Rotate bolt position
                glTranslatef(0.16, 0.0, 0.0); // Move bolt away from the center of the wheel
                gluCylinder(quad, 0.025, 0.025, 0.1, 10, 10); // Draw bolt
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
        gluCylinder(quad, 0.04, 0.04, 1.0, 20, 1); // Rear axle
    glPopMatrix();

    // Connecting rod
    glPushMatrix();
        glTranslatef(-0.75, 0.0, 0.0);
        glRotatef(90.0, 0.0, 1.0, 0.0); // Align with y-axis
        gluCylinder(quad, 0.04, 0.04, 1.5, 20, 1); // Connecting rod
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
	// Calculate steering angles based on Ackermann geometry
	double innerWheelAngle = atan(wheelBase / (wheelBase / tan(steeringAngle * PI / 180.0) + trackWidth / 2)) * 180.0 / PI;
	double outerWheelAngle = atan(wheelBase / (wheelBase / tan(steeringAngle * PI / 180.0) - trackWidth / 2)) * 180.0 / PI;

	// Update car position based on forwardPush (simulating forward movement)
	if (forwardPush != prevForwardPush)
	{
		updateCarPosition(forwardPush - prevForwardPush);
		prevForwardPush = forwardPush; // Update the previous forwardPush
	}

    glPushMatrix();
        // Translate car according to its position and direction
        glTranslatef(carPositionX, carPositionY, 0.25);
        glRotatef(carDirection, 0.0, 0.0, 1.0);

        // Draw the axle system
        drawAxleSystem(quad);

        // Draw wheels at each corner of the car
        // Front wheels with steering angles
        glPushMatrix();
            glTranslatef(-0.75, 0.55, 0.0); // Position front-right wheel
            glRotatef(-innerWheelAngle, 0.0, 0.0, 1.0); // Rotate wheel for steering
            glRotatef(forwardPush, 0.0, 1.0, 0.0); // Rotate the wheel around its rolling axis
            drawWheelAndBolts(quad);
        glPopMatrix();
    
        glPushMatrix();
            glTranslatef(-0.75, -0.45, 0.0); // Position front-left wheel
            glRotatef(-outerWheelAngle, 0.0, 0.0, 1.0); // Rotate wheel for steering
            glRotatef(forwardPush, 0.0, 1.0, 0.0); // Rotate the wheel around its rolling axis
            drawWheelAndBolts(quad);
        glPopMatrix();
        
        // Rear wheels
        glPushMatrix();
            glTranslatef(0.75, 0.55, 0.0); // Position rear-right wheel
            glRotatef(forwardPush, 0.0, 1.0, 0.0); // Rotate the wheel around its rolling axis
            drawWheelAndBolts(quad);
        glPopMatrix();
        
        glPushMatrix();
            glTranslatef(0.75, -0.45, 0.0); // Position rear-left wheel
            glRotatef(forwardPush, 0.0, 1.0, 0.0); // Rotate the wheel around its rolling axis
            drawWheelAndBolts(quad);
        glPopMatrix();

    glPopMatrix();
}

// Update the car's position based on push and steering angle
void updateCarPosition(double pushChange)
{
	// Convert pushChange to a movement amount (assuming push is an incremental value)
	double movementAmount = pushChange * 0.01; // Scale the push appropriately

	// Update the car's position based on its movement amount and current direction
	carPositionX += movementAmount * cos(carDirection * PI / 180.0);
	carPositionY += movementAmount * sin(carDirection * PI / 180.0);

	// Update the car's direction based on the steering angle
	if (movementAmount != 0.0 && steeringAngle != 0.0)
	{
		double turningRadius = wheelBase / tan(steeringAngle * PI / 180.0);
		carDirection += (movementAmount / turningRadius) * (180.0 / PI); // Convert to degrees
	}
}
