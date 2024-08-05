//----------------------------------------------------------------------------
// This demo shows how the powerful OpenGL can be used to render realistic 3D images.
//
//  main.c:  Contains main program structure, initialization and drawing
//  routines.  Positioning callbacks are defined in "positionme.c"
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include <windows.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>
#include "oglcarsim.h"
#include "cviogl.h"

//----------------------------------------------------------------------------
// Define default values
//----------------------------------------------------------------------------
#define DFLT_VIEW_LATITUDE      75.0
#define DFLT_VIEW_LONGITUDE     45.0
#define DFLT_VIEWPOINT_X        0.0
#define DFLT_VIEWPOINT_Y        0.0
#define DFLT_VIEWPOINT_Z        1.0
#define DFLT_VIEW_DISTANCE      8.5
#define DFLT_LIGHT_LATITUDE     30.0
#define DFLT_BASE_ANGLE         -33.0
#define DFLT_SHOULDER_ANGLE     -42.0
#define DFLT_ELBOW_ANGLE        77.0
#define DFLT_WRIST_ANGLE        30.0
#define DFLT_CLAW_ANGLE         50.0

// Define other useful constants
#define ANGLE_INCREMENT         4.0
#define CLAW_OPEN               0
#define CLAW_CLOSED             1
#define TRUE                    1
#define FALSE                   0

//----------------------------------------------------------------------------
// Variables
//----------------------------------------------------------------------------
// Define panel reference vars
int mainPanel, OGLControlPanel;


// Define bitmap handles for joystick button displays
int fourWayButtonUp, fourWayButtonDown, verticalButtonUp, verticalButtonDown,
    horizontalButtonUp, horizontalButtonDown, clawButtonOpen, clawButtonClose;

// Define vars used for rotating arm object and view
double shoulderRotation =       DFLT_SHOULDER_ANGLE;
double elbowRotation =          DFLT_ELBOW_ANGLE;
double baseRotation =           0.0;//DFLT_BASE_ANGLE;
double clawAngle =              DFLT_CLAW_ANGLE;
double wristRotation =          DFLT_WRIST_ANGLE;
double viewLongitude =          DFLT_VIEW_LONGITUDE;
double viewLatitude =           DFLT_VIEW_LATITUDE;
double viewDistance =           DFLT_VIEW_DISTANCE;
double viewPointX =             DFLT_VIEWPOINT_X;
double viewPointY =             DFLT_VIEWPOINT_Y;
double viewPointZ =             DFLT_VIEWPOINT_Z;
double clawState =              CLAW_OPEN;

// Define other useful vars
int holdButtonDown, miscCounter;
int xJoyCoord, yJoyCoord;
GLUquadricObj   *object;

// Function prototypes
void drawCar(GLUquadric* quad);
void drawGrid(float offset);

//----------------------------------------------------------------------------
// Prototypes
//----------------------------------------------------------------------------
void DrawCarImage(int fastFlag);
void InitOGLControl(void);
void RenderCarImage(int fastFlag);

//----------------------------------------------------------------------------
//  Main
//----------------------------------------------------------------------------
int main (int argc, char *argv[])
{
    if (InitCVIRTE (0, argv, 0) == 0)   /* Needed if linking in external compiler; harmless otherwise */
        return -1;  /* out of memory */

    // Load the main program panel
    if ((mainPanel = LoadPanel (0, "oglcarsim.uir", MAINPNL)) < 0)
        return -1;

    // Convert the CVI picture control to an OGL control
    OGLControlPanel = OGLConvertCtrl (mainPanel, MAINPNL_ARM_IMAGE);

    // Initialize the OGL control
    InitOGLControl();

    // Display plot
    OGLRefreshGraph(mainPanel, OGLControlPanel);

    // Display the main Panel
    DisplayPanel(mainPanel);

    // Enter the UI loop
    RunUserInterface();

    // Hide the panel and discard the OGL control along with the panel
    HidePanel (mainPanel);
    OGLDiscardCtrl(mainPanel,OGLControlPanel);
    DiscardPanel (mainPanel);
    return 0;
}

//----------------------------------------------------------------------------
//  InitOGLControl():  Initializes the OGL control and sets the rendering
//  properties appropriate to the arm image

void InitOGLControl(void)
{
    // Setup viewing position for system
    OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_PROJECTION_TYPE, OGLVAL_PERSPECTIVE);
    OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_VIEW_DIRECTION, OGLVAL_USER_DEFINED);
    OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_VIEW_LATITUDE, 55.0);
    OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_VIEW_LONGITUDE, 30.0);
    OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_VIEW_CENTERX,0.15);
    OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_VIEW_CENTERY,0.0);
    OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_VIEW_CENTERZ,0.30);
	OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_VIEW_AUTO_DISTANCE, OGLVAL_FALSE);
    OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_VIEW_DISTANCE, 3.0);

   // Setup lighting for system
    OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_LIGHTING_ENABLE, 1);
    OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_LIGHT_SELECT,    1);
    OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_LIGHT_ENABLE,    1);
    OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_LIGHT_DISTANCE,  2.0);
    OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_LIGHT_LATITUDE, DFLT_LIGHT_LATITUDE);
	
	// Setup plot axes, grids, scaling, and plot area
	OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_PLOTAREA_ZSTART, 0.0);
	OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_ZNAME, "z-axis");
	OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_XNAME, "x-axis");
	OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_YNAME, "y-axis");
	OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_ZNAME_POINT_SIZE, 20);
	OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_XNAME_POINT_SIZE, 20); 
	OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_YNAME_POINT_SIZE, 20);
	OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_ZNAME_COLOR, OGLVAL_YELLOW); 
	OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_XNAME_COLOR, OGLVAL_YELLOW);
	OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_YNAME_COLOR, OGLVAL_YELLOW);
	OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_YZ_GRID_VISIBLE, 1);
	OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_XY_GRID_VISIBLE, 1);
	OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_XZ_GRID_VISIBLE, 1);
	OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_YZ_GRID_COLOR, OGLVAL_LT_GRAY); 
	OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_XY_GRID_COLOR, OGLVAL_LT_GRAY);
	OGLSetCtrlAttribute (mainPanel, OGLControlPanel, OGLATTR_XZ_GRID_COLOR, OGLVAL_LT_GRAY);
    // Disable 3D plotting feature of the OGL instrument driver; use only lighting properties
    // and coordinate system
    // OGLSetCtrlAttribute (mainPanel,OGLControlPanel, OGLATTR_PLOTTING_ENABLE, 0);
}

//----------------------------------------------------------------------------
//  RenderCarImage():  Renders the arm image to the OGL control.
//----------------------------------------------------------------------------
void RenderCarImage(int fastFlag)
{
    GLfloat specularLight0[] = {1.0f, 1.0f, 1.0f, 1.0f};

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		
			glEnable(GL_DEPTH_TEST);
			glShadeModel(GL_SMOOTH);

			glEnable(GL_COLOR_MATERIAL);
			glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularLight0);
			glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 128);

			DrawCarImage(fastFlag);
		
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
	
	glPopMatrix();
	glPopAttrib();
	glFlush();
}

//----------------------------------------------------------------------------
//  DrawCarImage():  Draws the OGL car image in its entirety
//----------------------------------------------------------------------------
void DrawCarImage(int fastFlag)
{
	// Create a new quadric object; we'll refer to this structure when using
    // the GLU routines to draw higher-level primitives
    object = gluNewQuadric();
	
	// Draw the moving grid
    drawGrid(-baseRotation * 0.0001); // Scale the grid movement appropriately
    
    // Draw in "Line" mode for speed
    if (fastFlag)
        gluQuadricDrawStyle(object,GLU_LINE);
	
	drawCar(object);
	
    gluDeleteQuadric(object);
    return;
}

//----------------------------------------------------------------------------
//  OGLCallback():  Required by CVI for image refreshes and paints
//----------------------------------------------------------------------------
int CVICALLBACK OGLCallback (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    switch (event) {
        case OGLEVENT_REFRESH:
            // Render the arm image when REFRESH event is received
            RenderCarImage(eventData1);
            break;
    }
    return 0;
}

int CVICALLBACK PanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2)
{
	if( event==EVENT_CLOSE )
		QuitUserInterface(0);
	return 0;
}
