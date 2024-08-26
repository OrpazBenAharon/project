#include <windows.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>
#include <toolbox.h>
#include <stdbool.h>
#include <stdlib.h>
#include "oglcarsim.h"
#include "cviogl.h"

#define MAX_OBSTACLES 50   
//----------------------------------------------------------------------------
// External variables for interaction with other parts of the program
extern double forwardPush;   // External push to the car
extern double steeringAngle;  // Steering angle

// Internal representation of the car's state
double carPositionX = 0.0;
double carPositionY = 0.0;
double carDirection = 0.0; // Angle in degrees
double prevForwardPush = 0.0; // To track changes in forwardPush

//grid prameters
double centerX ;
double centerY ;
float gridSize = 32.0f;
int lineCount = 17;

// Car geometry parameters
double wheelBase = 2.0; // Distance between the axles
double trackWidth = 1.2; // Distance between the wheels on an axle

// Obstacle and Wall
// Structure to represent an obstacle or wall
typedef struct {
    float posX;
    float posY;
    float width;
    float height;
    bool isWall; // True if it's a wall, false if it's an obstacle
} Obstacle;

// Array to hold obstacles and walls
Obstacle obstacles[MAX_OBSTACLES];
int obstacleCount = 0;
//----------------------------------------------------------------------------
// Function prototypes
void drawWheelAndBolts(GLUquadric* quad);
void drawCar(GLUquadric* quad);
bool isNearCenter(float carX, float carY, float centerX, float centerY);
void renderGridLines(int lineCount, float lineSpacing);
void resetGridCenter(float carX, float carY) ;
void drawGrid();
void drawAxleSystem(GLUquadric* quad);
void updateCarPosition(double push);
bool isNearCenter(float carX, float carY, float centerX, float centerY);
void initializeObstacles();
void drawObstacles();
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Draws a cyclic grid
//----------------------------------------------------------------------------
void drawGrid()
{
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 1.0f);
    glLineWidth(3.0);
    float lineSpacing = (2 * gridSize) / (lineCount - 1);

    // Render the primary grid
    glTranslatef(centerX, 0.0f, 0.0f);
    renderGridLines(lineCount, lineSpacing);
	drawObstacles();
    // Check if the car is near the center to render additional grids
    if (!isNearCenter(carPositionX, carPositionY, centerX, centerY))
    {
        // Reset the grid center when the car passes the center
		resetGridCenter(carPositionX, carPositionY); 
    }
    glPopMatrix();
}


void renderGridLines(int lineCount, float lineSpacing)
{
    // Horizontal lines
    glBegin(GL_LINES);
    for (int i = 0; i < lineCount; i++)
    {
        float y = -gridSize + lineSpacing * i;
        glVertex2f(-gridSize, y/16);
        glVertex2f(gridSize, y/16);
    }
    glEnd();

    // Vertical lines
    glBegin(GL_LINES);
    for (i = 0; i < 16*lineCount; i++)
    {
        float x = -gridSize + lineSpacing/16 * i;
        glVertex2f(x, -gridSize/16);
        glVertex2f(x, gridSize/16);
    }
    glEnd();
}

bool isNearCenter(float carX, float carY, float centerX, float centerY)
{
    float threshold = gridSize / 2.0f;  // Define a suitable threshold for proximity
    return (fabs(carX - centerX) < threshold);
}

void resetGridCenter(float carX, float carY)
{
    // Reset the grid center to the current car position
    centerX = carX;
}

//----------------------------------------------------------------------------
// Drawing a car
//----------------------------------------------------------------------------
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

//---------------------------------------------------------------------------- 
// Car Position
//---------------------------------------------------------------------------- 
void updateCarPosition(double pushChange)
{
    double movementAmount = pushChange * 0.01; // Scale the push appropriately


    // Update the car's position
    carPositionX += movementAmount * cos(carDirection * PI / 180.0);
    carPositionY += movementAmount * sin(carDirection * PI / 180.0);


    // Update the car's direction based on the steering angle
    if (movementAmount != 0.0 && steeringAngle != 0.0)
    {
        double turningRadius = wheelBase / tan(steeringAngle * PI / 180.0);
        carDirection += (movementAmount / turningRadius) * (180.0 / PI); // Convert to degrees
    }
}
//---------------------------------------------------------------------------- 
// Obstacle and Wall
//---------------------------------------------------------------------------- 

void initializeObstacles()
{
	// Initialize the random number generator with the current time as the seed
	srand(time(NULL));
    obstacleCount = 1 + rand() % 10; // Randomize the number of obstacles between 1 and 10
    for (int i = 0; i < obstacleCount; i++)
    {
        // Generate random positions using floating-point arithmetic
        obstacles[i].posX = ((float)rand() / RAND_MAX) * (gridSize/2)- gridSize/2; // X position between -gridSize/2 and gridSize/2
        obstacles[i].posY = ((float)rand() / RAND_MAX) * (gridSize/32) - gridSize/32; // Y position between -gridSize/32 and gridSize/32

        obstacles[i].width = 0.5f; // Random width between 1.0 and 5.0
        obstacles[i].height = 0.5f; // Random height between 1.0 and 5.0
        obstacles[i].isWall = rand() % 2; // Randomly decide if it's a wall (true) or obstacle (false)
    }
}


 void drawObstacles()
{
    //glPushMatrix();
    for (int i = 0; i < obstacleCount; i++)
    {
        if (obstacles[i].isWall)
            glColor3f(0.5f, 0.5f, 0.5f); // Gray color for walls
        else
            glColor3f(1.0f, 0.0f, 0.0f); // Red color for obstacles

        glBegin(GL_QUADS);
            glVertex2f(obstacles[i].posX, obstacles[i].posY);
            glVertex2f(obstacles[i].posX + obstacles[i].width, obstacles[i].posY);
            glVertex2f(obstacles[i].posX + obstacles[i].width, obstacles[i].posY + obstacles[i].height);
            glVertex2f(obstacles[i].posX, obstacles[i].posY + obstacles[i].height);
        glEnd();
    }
    //glPopMatrix();
}

 bool checkCollision(double carX, double carY, double carWidth, double carHeight)
{
    for (int i = 0; i < obstacleCount; i++)
    {
        if (carX < obstacles[i].posX + obstacles[i].width &&
            carX + carWidth > obstacles[i].posX &&
            carY < obstacles[i].posY + obstacles[i].height &&
            carY + carHeight > obstacles[i].posY)
        {
            // Collision detected
            return true;
        }
    }
    return false;
}
