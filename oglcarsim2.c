//----------------------------------------------------------------------------
//  Contains positioning UIR callbacks for the joystick controls.
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
#define INCREMENT        1.0

// Define panel reference vars
extern int mainPanel, OGLControlPanel;
									     
// Define vars used for maniputlating object and view
extern double steeringAngle, forwardPush;

//----------------------------------------------------------------------------
//  AdjustPush():  Push the car forward or backward
//----------------------------------------------------------------------------
int CVICALLBACK AdjustPush (int panel, int control, int event,
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
                    case MAINPNL_JOYSTICK_DOWN:
                            forwardPush += INCREMENT;
                        break;
                    case MAINPNL_JOYSTICK_UP:
                            forwardPush -= INCREMENT;
                        break;
                }

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
//  AdjustSteerAgnle():  Adjusts the steer angle of the forward axle
//----------------------------------------------------------------------------
int CVICALLBACK AdjustSteerAgnle (int panel, int control, int event,
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
                    case MAINPNL_JOYSTICK_LEFT:
                            steeringAngle -= INCREMENT;
                        break;
                    case MAINPNL_JOYSTICK_RIGHT:
                            steeringAngle += INCREMENT;
                        break;
                }

                // Check for bounds
                if (steeringAngle >= 30)
                    steeringAngle = 30;
                if (steeringAngle <= -30)
                    steeringAngle = -30;

                // Re-render the image on the OGL control
                OGLRefreshGraph (mainPanel, OGLControlPanel);
            }
            // Set picture control background color back
            SetCtrlAttribute (panel, control, ATTR_PICT_BGCOLOR, color);
            break;
    }
    return 0;
}

