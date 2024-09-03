#ifndef OGLCARSIM_H
#define OGLCARSIM_H

// Standard and OpenGL includes
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <ansi_c.h>
#include <cvirte.h>
#include <toolbox.h>
#include <stdbool.h>
#include <rs232.h>
#include "cviogl.h"
#include "OGL_CarSim.h"
#include "locatecom.h"

// Constants for simulation parameters
#define DISTANCE        7.0        // Camera distance from car
#define HEIGHT          3.0        // Camera height
#define CENTERZ         0.4        // Center Z-coordinate for camera focus
#define FPS             60         // Frames per second
#define FRAME_TIME      (1.0/FPS)  // Time per frame
#define MAX_OBSTACLES   50         // Maximum number of obstacles
#define WHEEL_BASE      2.0        // Distance between front and rear axles
#define TRACK_WIDTH     1.2        // Distance between left and right wheels
#define GRID_SIZE       90.0       // Size of the grid
#define LINE_COUNT      9          // Number of grid lines
#define INCREMENT_FORWARD 1.0      // Forward movement increment
#define INCREMENT_ANGLE 3          // Steering angle increment
#define MAX_ANGLE       30         // Maximum steering angle

// Structure to represent obstacles
typedef struct {
    float posX;
    float posY;
    float width;
    float height;
    bool isObstacle;
} Obstacle;

// Structure to represent the car's state
typedef struct {
    double positionX;
    double positionY;
    double direction;
    double forwardMove;
    double steeringAngle;
    double prevForwardMove;
} CarState;

// Global variables
extern int mainPanel, OGLControlPanel;
extern CarState car;
extern int portNumber;
extern int isRunning;
extern double centerX;
extern Obstacle obstacles[MAX_OBSTACLES];
extern int obstacleCount;

// Function prototypes

// Initialization and Setup
void InitializeObstacles(void);
void InitOGLControl(void);

// Rendering Functions
void RenderCar(GLUquadric* quad);
void RenderCarImage(void);
void RenderGrid(void);
void RenderGridLines(int lineCount, float lineSpacing);
void RenderObstacleCars(void);

// Update and Position Functions
void ResetCarPosition(void);
void ResetGridCenter(float carX);
void UpdateCameraPosition(void);
void UpdateCarPosition(double pushChange);

// Collision and Boundary Checks
bool CheckCollision(double carX, double carY, double carWidth, double carHeight);
bool isNearCenter(float carX, float centerX);

// Utility Functions
int IntCompareWrapper(const void *item1, const void *item2);

// Communication Functions
void DisplayRS232Error(int err);
void FindComPorts(void);

// Callback Functions
void CVICALLBACK ComCallback(int portNumber, int eventMask, void *callbackData);
static int CVICALLBACK ThreadFunction(void *functionData);
static int CVICALLBACK CarPhysicsLoop(void *functionData);

#endif // OGLCARSIM_H