//----------------------------------------------------------------------------
// This program shows how OpenGL can be used to render simple car chassis mechanics in 3D.
//
//  oglarmsim.c :  	Contains main program structure and initialization and.
//  oglarmsim2.c:  	Contains Positioning callbacks.
//	OGLDraw.c	:	Contains drawing routines.
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
#include <toolbox.h>
#include "oglcarsim.h"
#include "cviogl.h"

//----------------------------------------------------------------------------
// Define default values
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Variables
//----------------------------------------------------------------------------
// Define panel reference vars
int mainPanel, OGLControlPanel;

// Define vars used for manipulating car object and view
double forwardPush =            0.0;
double steeringAngle =          0.0;

// Internal representation of the car's state
extern double carPositionX;
extern double carPositionY;
extern double carDirection; // Angle in degrees

// Define other useful vars
GLUquadricObj   *object;

// Function prototypes
void drawCar(GLUquadric* quad);
void drawGrid();
void updateCameraPosition();

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
//  properties appropriate to the car image
//----------------------------------------------------------------------------
void InitOGLControl(void)
{
    // Initial viewing setup
    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_PROJECTION_TYPE, OGLVAL_PERSPECTIVE);
    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_VIEW_DIRECTION, OGLVAL_USER_DEFINED);
    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_VIEW_AUTO_DISTANCE, OGLVAL_FALSE);
	
	updateCameraPosition();

    // Setup lighting for system
    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_LIGHTING_ENABLE, 1);
    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_LIGHT_SELECT, 1);
    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_LIGHT_ENABLE, 1);
    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_LIGHT_DISTANCE, 2.0);
    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_LIGHT_LATITUDE, 30.0);

	// Disable 3D plotting feature of the OGL instrument driver
 	// use only lighting properties and coordinate system
    OGLSetCtrlAttribute (mainPanel,OGLControlPanel, OGLATTR_PLOTTING_ENABLE, 0);
}


//----------------------------------------------------------------------------
//  RenderCarImage():  Renders the car image to the OGL control.
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
						
			if(!fastFlag)
				// Update the camera position and orientation
				updateCameraPosition();

			// Draw the static grid
			drawGrid();

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

	// Draw in "Line" mode for speed
	if (fastFlag)
		gluQuadricDrawStyle(object,GLU_LINE);

	drawCar(object);

	gluDeleteQuadric(object);
	return;
}

void updateCameraPosition()
{
	// Set the camera attributes
	OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_VIEW_LATITUDE, 75.0);
	OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_VIEW_LONGITUDE, carDirection);
	OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_VIEW_CENTERX, carPositionX);
	OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_VIEW_CENTERY, carPositionY);
	OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_VIEW_CENTERZ, 0.4);
	OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_VIEW_DISTANCE, 3.0);
}


//----------------------------------------------------------------------------
//  OGLCallback():  Required by CVI for image refreshes and paints
//----------------------------------------------------------------------------
int CVICALLBACK OGLCallback (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case OGLEVENT_REFRESH:
			// Render the arm image when REFRESH event is received
			RenderCarImage(eventData1);
			break;
	}
	return 0;
}

int CVICALLBACK PanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2)
{
	if(event == EVENT_CLOSE)
		QuitUserInterface(0);
	return 0;
}
