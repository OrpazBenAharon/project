//----------------------------------------------------------------------------
//  Contains positioning UIR callbacks for each of the joystick controls.
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include <windows.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include <ansi_c.h>
#include <userint.h>

#include "oglcarsim.h"
#include "cviogl.h"

//----------------------------------------------------------------------------
// Extern Variables
//----------------------------------------------------------------------------
// Define other useful constants
#define ANGLE_INCREMENT         4.0
#define CLAW_OPEN               0
#define CLAW_CLOSED             1

// Define panel reference vars
extern int mainPanel, OGLControlPanel;

									     
// Define vars used for rotating arm object and view
extern double shoulderRotation,elbowRotation,baseRotation,clawAngle,wristRotation,
              viewLongitude,viewLatitude,viewDistance,viewPointX,viewPointY,viewPointZ;
extern double clawState;

//----------------------------------------------------------------------------
//  OpenCloseClaw():  This function controls the opening and closing of the
//  arm's claw
//----------------------------------------------------------------------------
int CVICALLBACK OpenCloseClaw (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    int width, height, color, holdButtonDown, xJoyCoord, yJoyCoord;

    switch (event) {
        case EVENT_LEFT_CLICK:
        case EVENT_LEFT_DOUBLE_CLICK:
            holdButtonDown = TRUE;

            GetCtrlAttribute( panel, control, ATTR_WIDTH, &width);
            GetCtrlAttribute( panel, control, ATTR_HEIGHT, &height);

            // Invert backgroud color of picture control
            GetPanelAttribute (panel, ATTR_BACKCOLOR, &color);
            SetCtrlAttribute (panel, control, ATTR_PICT_BGCOLOR, ((~color) & 0x00FFFFFF));
            ProcessDrawEvents();

            // Loop until the mouse button is released or leaves the joystick
            while (holdButtonDown)
            {
                GetRelativeMouseState (panel, control, &xJoyCoord,
                                       &yJoyCoord, &holdButtonDown, NULL, NULL);

                if ((!holdButtonDown) || (xJoyCoord>width) || (xJoyCoord<0) ||
                    (yJoyCoord>height) || (yJoyCoord<0))
                    break;

                // Check mouse location for appropriate action
                switch (control)
                {
                    case MAINPNL_CLAW_CLOSE:
                            clawState += 0.2;
                        break;
                    case MAINPNL_CLAW_OPEN:
                            clawState -= 0.2;
                        break;
                }

                // Check for bounds
                if (clawState<CLAW_OPEN)
                {
                    clawState = CLAW_OPEN;
                    holdButtonDown = FALSE;
                }
                else if (clawState>CLAW_CLOSED)
                {
                    clawState = CLAW_CLOSED;
                    holdButtonDown = FALSE;
                }

                // Re-render the image on the OGL graph control
                OGLRefreshGraph (mainPanel, OGLControlPanel);
            }
            // Set picture control background color back
            SetCtrlAttribute (panel, control, ATTR_PICT_BGCOLOR, color);
            break;
    }
    return 0;
}

//----------------------------------------------------------------------------
//  AdjustShoulderPosition():  Adjusts the arm's shoulder joint
//----------------------------------------------------------------------------
int CVICALLBACK AdjustShoulderPosition (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    int width, height, color, holdButtonDown, xJoyCoord, yJoyCoord;

    switch (event) {
        case EVENT_LEFT_CLICK:
		case EVENT_LEFT_DOUBLE_CLICK:
            holdButtonDown = TRUE;

            GetCtrlAttribute( panel, control, ATTR_WIDTH, &width);
            GetCtrlAttribute( panel, control, ATTR_HEIGHT, &height);

            // Invert backgroud color of picture control
            GetPanelAttribute (panel, ATTR_BACKCOLOR, &color);
            SetCtrlAttribute (panel, control, ATTR_PICT_BGCOLOR, ((~color) & 0x00FFFFFF));
            ProcessDrawEvents();

            // Loop until the mouse button is released or leaves the joystick
            while (holdButtonDown)
            {
                GetRelativeMouseState (panel, control, &xJoyCoord,
                    &yJoyCoord, &holdButtonDown, NULL, NULL);

                if ((!holdButtonDown) || (xJoyCoord>width) || (xJoyCoord<0) ||
                    (yJoyCoord>height) || (yJoyCoord<0))
                    break;

                // Check mouse location for appropriate action
                switch (control)
                {
                    case MAINPNL_BASE_JOYSTICK_LEFT:
                            baseRotation += ANGLE_INCREMENT;
                        break;
                    case MAINPNL_BASE_JOYSTICK_RIGHT:
                            baseRotation -= ANGLE_INCREMENT;
                        break;
                    case MAINPNL_BASE_JOYSTICK_UP:
                            shoulderRotation += ANGLE_INCREMENT;
                        break;
                    case MAINPNL_BASE_JOYSTICK_DOWN:
                            shoulderRotation -= ANGLE_INCREMENT;
                        break;
                }

                // Make sure the angle values are in range
                if (abs(baseRotation) >= 360)
                    baseRotation -= 360;
                if (shoulderRotation >= 75)
                    shoulderRotation -= ANGLE_INCREMENT;
                if (shoulderRotation <= -75)
                    shoulderRotation += ANGLE_INCREMENT;

                // Re-render the image to the OGL control
                OGLRefreshGraph (mainPanel, OGLControlPanel);
            }
            // Set picture control background color back
            SetCtrlAttribute (panel, control, ATTR_PICT_BGCOLOR, color);
            break;
    }
    return 0;
}

//----------------------------------------------------------------------------
//  AdjustElbowPosition():  Adjusts the position of the elbow joint
//----------------------------------------------------------------------------
int CVICALLBACK AdjustElbowPosition (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
        int width, height, color, holdButtonDown, xJoyCoord, yJoyCoord;

    switch (event) {
        case EVENT_LEFT_CLICK:
		case EVENT_LEFT_DOUBLE_CLICK:
            GetCtrlAttribute( panel, control, ATTR_WIDTH, &width);
            GetCtrlAttribute( panel, control, ATTR_HEIGHT, &height);

            // Invert backgroud color of picture control
            GetPanelAttribute (panel, ATTR_BACKCOLOR, &color);
            SetCtrlAttribute (panel, control, ATTR_PICT_BGCOLOR, ((~color) & 0x00FFFFFF));
            ProcessDrawEvents();

            // Loop until the mouse button is released or leaves the joystick
            holdButtonDown = TRUE;
            while (holdButtonDown)
            {
                GetRelativeMouseState (panel, control, &xJoyCoord,
                   &yJoyCoord, &holdButtonDown, NULL, NULL);

                if ((!holdButtonDown) || (xJoyCoord>width) || (xJoyCoord<0) ||
                    (yJoyCoord>height) || (yJoyCoord<0))
                    break;

                // Check mouse location for appropriate action
                switch (control)
                {
                    case MAINPNL_ELBOW_JOYSTICK_UP:
                            elbowRotation -= ANGLE_INCREMENT;
                        break;
                    case MAINPNL_ELBOW_JOYSTICK_DOWN:
                            elbowRotation += ANGLE_INCREMENT;
                        break;
                }

                // Check for bounds
                if (elbowRotation >= 120)
                    elbowRotation = 120;
                if (elbowRotation <= -120)
                    elbowRotation = -120;

                // Re-render the image on the OGL control
                OGLRefreshGraph (mainPanel, OGLControlPanel);
            }
            // Set picture control background color back
            SetCtrlAttribute (panel, control, ATTR_PICT_BGCOLOR, color);
            break;
    }
    return 0;
}

//----------------------------------------------------------------------------
//  AdjustCameraPosition():  Adjusts the viewing (camera) position
//----------------------------------------------------------------------------
int CVICALLBACK AdjustCameraPosition (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
        int width, height, color, holdButtonDown, xJoyCoord, yJoyCoord;

    switch (event) {
        case EVENT_LEFT_CLICK:
        case EVENT_LEFT_DOUBLE_CLICK:
            GetCtrlAttribute( panel, control, ATTR_WIDTH, &width);
            GetCtrlAttribute( panel, control, ATTR_HEIGHT, &height);

            // Invert backgroud color of picture control
            GetPanelAttribute (panel, ATTR_BACKCOLOR, &color);
            SetCtrlAttribute (panel, control, ATTR_PICT_BGCOLOR, ((~color) & 0x00FFFFFF));
            ProcessDrawEvents();

            // Loop until the mouse button is released or leaves the joystick
            holdButtonDown = TRUE;
            while (holdButtonDown) {
                GetRelativeMouseState (panel, control, &xJoyCoord,
                                       &yJoyCoord, &holdButtonDown, NULL, NULL);

                if ((!holdButtonDown) || (xJoyCoord>width) || (xJoyCoord<0) ||
                    (yJoyCoord>height) || (yJoyCoord<0))
                    break;

                // Check mouse location for appropriate action
			   OGLGetCtrlAttribute (mainPanel, OGLControlPanel,
									 OGLATTR_VIEW_LATITUDE, &viewLatitude);
			   OGLGetCtrlAttribute (mainPanel, OGLControlPanel,
									 OGLATTR_VIEW_LONGITUDE, &viewLongitude);
               switch (control)
                {
                    case MAINPNL_CAMERA_JOYSTICK_LEFT:
                            viewLongitude -= ANGLE_INCREMENT;
                        break;
                    case MAINPNL_CAMERA_JOYSTICK_RIGHT:
                            viewLongitude += ANGLE_INCREMENT;
                        break;
                    case MAINPNL_CAMERA_JOYSTICK_UP:
                            viewLatitude -= ANGLE_INCREMENT;
                        break;
                    case MAINPNL_CAMERA_JOYSTICK_DOWN:
                            viewLatitude += ANGLE_INCREMENT;
                        break;
                }

                // Make sure the angle values are in range
                if (abs(viewLatitude) >= 360)
                    viewLatitude = 0;
                if (abs(viewLongitude) >= 360)
                    viewLongitude = 0;

                // Re-render the image to the OGL control with adjusted viewing
                OGLSetCtrlAttribute (mainPanel, OGLControlPanel,OGLATTR_VIEW_LATITUDE,viewLatitude);
                OGLSetCtrlAttribute (mainPanel, OGLControlPanel,OGLATTR_VIEW_LONGITUDE,viewLongitude);
                OGLRefreshGraph (mainPanel, OGLControlPanel);
            }
            // Set picture control background color back
            SetCtrlAttribute (panel, control, ATTR_PICT_BGCOLOR, color);
            break;
    }
    return 0;
}

//----------------------------------------------------------------------------
//  AdjustWristPosition():  Adjusts the position of the wrist joint
//----------------------------------------------------------------------------
int CVICALLBACK AdjustWristPosition (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
        int width, height, color, holdButtonDown, xJoyCoord, yJoyCoord;

    switch (event) {
        case EVENT_LEFT_CLICK:
        case EVENT_LEFT_DOUBLE_CLICK:
            holdButtonDown = TRUE;

            GetCtrlAttribute( panel, control, ATTR_WIDTH, &width);
            GetCtrlAttribute( panel, control, ATTR_HEIGHT, &height);

            // Invert backgroud color of picture control
            GetPanelAttribute (panel, ATTR_BACKCOLOR, &color);
            SetCtrlAttribute (panel, control, ATTR_PICT_BGCOLOR, ((~color) & 0x00FFFFFF));
            ProcessDrawEvents();

            // Loop until the mouse button is released or leaves the joystick
            while (holdButtonDown)
            {
                GetRelativeMouseState (panel, control, &xJoyCoord,
                                       &yJoyCoord, &holdButtonDown, NULL, NULL);

                if ((!holdButtonDown) || (xJoyCoord>width) || (xJoyCoord<0) ||
                    (yJoyCoord>height) || (yJoyCoord<0))
                    break;

                // Check mouse location for appropriate action
                switch (control)
                {
                    case MAINPNL_WRIST_JOYSTICK_LEFT:
                            wristRotation -= ANGLE_INCREMENT;
                        break;
                    case MAINPNL_WRIST_JOYSTICK_RIGHT:
                            wristRotation += ANGLE_INCREMENT;
                        break;
                }

                // Check for bounds
                if (abs(wristRotation) >= 360)
                    wristRotation -= 360;

                // Re-render the image on the OGL graph control
                OGLRefreshGraph (mainPanel, OGLControlPanel);
            }
            // Set picture control background color back
            SetCtrlAttribute (panel, control, ATTR_PICT_BGCOLOR, color);
            break;
    }
    return 0;
}
