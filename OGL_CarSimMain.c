#include "OGL_CarSimHeader.h"

// Global variables
int mainPanel, OGLControlPanel;
CarState car = {0};
GLUquadricObj *object;

int main(int argc, char *argv[])
{
    if (InitCVIRTE(0, argv, 0) == 0)
        return -1;

    if ((mainPanel = LoadPanel(0, "OGL_CarSim.uir", MAINPNL)) < 0)
        return -1;

    OGLControlPanel = OGLConvertCtrl(mainPanel, MAINPNL_IMAGE);

    InitOGLControl();
    InitializeObstacles();

    OGLRefreshGraph(mainPanel, OGLControlPanel);
    DisplayPanel(mainPanel);

    RingCallback(0, 0, EVENT_COMMIT, 0, 0, 0);
    
    SetCtrlAttribute(mainPanel, MAINPNL_TIMER, ATTR_INTERVAL, FRAME_TIME);
    SetCtrlAttribute(mainPanel, MAINPNL_TIMER, ATTR_ENABLED, 1);

    RunUserInterface();

    // Cleanup
    HidePanel(mainPanel);
    if (portNumber) CloseCom(portNumber);
    OGLDiscardCtrl(mainPanel, OGLControlPanel);
    DiscardPanel(mainPanel);
    return 0;
}

void InitOGLControl(void)
{
    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_PROJECTION_TYPE, OGLVAL_PERSPECTIVE);
    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_VIEW_DIRECTION, OGLVAL_USER_DEFINED);
    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_VIEW_AUTO_DISTANCE, OGLVAL_FALSE);

    UpdateCameraPosition();

    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_LIGHTING_ENABLE, 1);
    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_LIGHT_SELECT, 1);
    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_LIGHT_ENABLE, 1);
    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_LIGHT_DISTANCE, 2.0);
    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_LIGHT_LATITUDE, 30.0);

    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_PLOTTING_ENABLE, 0);
    OGLSetCtrlAttribute(mainPanel, OGLControlPanel, OGLATTR_ENABLE_PAN_ZOOM_ROTATE, 0);
}

void ResetCarPosition(void)
{
    car = (CarState){0};
    ResetGridCenter(car.positionX);
    UpdateCameraPosition();
    InitializeObstacles();
}

int CVICALLBACK PanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2)
{
    if (event == EVENT_CLOSE)
        QuitUserInterface(0);
    return 0;
}