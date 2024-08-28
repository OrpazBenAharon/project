#ifndef OGLCARSIM_H
#define OGLCARSIM_H

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

// Constants
#define DISTANCE        7.0
#define HEIGHT          3.0
#define CENTERZ         0.4
#define FPS             60
#define FRAME_TIME      (1.0/FPS)
#define MAX_OBSTACLES   50
#define WHEEL_BASE      2.0
#define TRACK_WIDTH     1.2
#define GRID_SIZE       90.0
#define LINE_COUNT      9
#define INCREMENT_FORWARD 1.0
#define INCREMENT_ANGLE 3
#define MAX_ANGLE       30

// Structs
typedef struct {
    float posX;
    float posY;
    float width;
    float height;
    bool isWall;
} Obstacle;

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