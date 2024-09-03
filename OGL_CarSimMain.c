#include "OGL_CarSimHeader.h"

// Global variables
int mainPanel, OGLControlPanel;
CarState car = {0};
GLUquadricObj *object;

int main(int argc, char *argv[])
{
    // Initialize CVI Runtime Engine
    if (InitCVIRTE(0, argv, 0) == 0)
        return -1;

    // Load the main panel
    if ((mainPanel = LoadPanel(0, "OGL_CarSim.uir", MAINPNL)) < 0)
        return -1;

    // Convert the CVI picture control to an OGL control
    OGLControlPanel = OGLConvertCtrl(mainPanel, MAINPNL_IMAGE);

    // Initialize OpenGL control and obstacles
    InitOGLControl();
    InitializeObstacles();

    // Refresh the OpenGL graph and display the panel
    OGLRefreshGraph(mainPanel, OGLControlPanel);
    DisplayPanel(mainPanel);

    // Set up the timer for animation
    SetCtrlAttribute(mainPanel, MAINPNL_TIMER, ATTR_INTERVAL, FRAME_TIME);
    SetCtrlAttribute(mainPanel, MAINPNL_TIMER, ATTR_ENABLED, 1);

    // Start the user interface
    RunUserInterface();

    // Cleanup
    HidePanel(mainPanel);
    if (portNumber) CloseCom(portNumber);
    OGLDiscardCtrl(mainPanel, OGLControlPanel);
    DiscardPanel(mainPanel);
    return 0;
}

// Initialize OpenGL control settings
void InitOGLControl(void)
{
    // Set up projection and view attributes
    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_PROJECTION_TYPE, OGLVAL_PERSPECTIVE);
    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_VIEW_DIRECTION, OGLVAL_USER_DEFINED);
    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_VIEW_AUTO_DISTANCE, OGLVAL_FALSE);

    UpdateCameraPosition();

    // Set up lighting
    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_LIGHTING_ENABLE, 1);
    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_LIGHT_SELECT, 1);
    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_LIGHT_ENABLE, 1);
    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_LIGHT_DISTANCE, 2.0);
    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_LIGHT_LATITUDE, 30.0);

    // Disable 3D plotting and pan/zoom/rotate
    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_PLOTTING_ENABLE, 0);
    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_ENABLE_PAN_ZOOM_ROTATE, 0);
}

// Reset the car's position and obstacles
void ResetCarPosition(void)
{
    car = (CarState){0};
    ResetGridCenter(car.positionX);
    UpdateCameraPosition();
    InitializeObstacles();
}

// Handle panel events (e.g., closing the application)
int CVICALLBACK PanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2)
{
    if (event == EVENT_CLOSE)
        QuitUserInterface(0);
    return 0;
}