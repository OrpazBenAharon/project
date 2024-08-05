/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  MAINPNL                          1       /* callback function: PanelCallback */
#define  MAINPNL_BASE_JOYSTICK_LEFT       2       /* control type: picture, callback function: AdjustShoulderPosition */
#define  MAINPNL_BASE_JOYSTICK_UP         3       /* control type: picture, callback function: AdjustShoulderPosition */
#define  MAINPNL_BASE_JOYSTICK_RIGHT      4       /* control type: picture, callback function: AdjustShoulderPosition */
#define  MAINPNL_BASE_JOYSTICK_DOWN       5       /* control type: picture, callback function: AdjustShoulderPosition */
#define  MAINPNL_ELBOW_JOYSTICK_UP        6       /* control type: picture, callback function: AdjustElbowPosition */
#define  MAINPNL_ELBOW_JOYSTICK_DOWN      7       /* control type: picture, callback function: AdjustElbowPosition */
#define  MAINPNL_WRIST_JOYSTICK_LEFT      8       /* control type: picture, callback function: AdjustWristPosition */
#define  MAINPNL_WRIST_JOYSTICK_RIGHT     9       /* control type: picture, callback function: AdjustWristPosition */
#define  MAINPNL_CLAW_OPEN                10      /* control type: picture, callback function: OpenCloseClaw */
#define  MAINPNL_CLAW_CLOSE               11      /* control type: picture, callback function: OpenCloseClaw */
#define  MAINPNL_CAMERA_JOYSTICK_LEFT     12      /* control type: picture, callback function: AdjustCameraPosition */
#define  MAINPNL_CAMERA_JOYSTICK_UP       13      /* control type: picture, callback function: AdjustCameraPosition */
#define  MAINPNL_CAMERA_JOYSTICK_RIGHT    14      /* control type: picture, callback function: AdjustCameraPosition */
#define  MAINPNL_CAMERA_JOYSTICK_DOWN     15      /* control type: picture, callback function: AdjustCameraPosition */
#define  MAINPNL_BASE_LABEL               16      /* control type: textMsg, callback function: (none) */
#define  MAINPNL_ELBOW_LABEL              17      /* control type: textMsg, callback function: (none) */
#define  MAINPNL_WRIST_LABEL              18      /* control type: textMsg, callback function: (none) */
#define  MAINPNL_CLAW_LABEL               19      /* control type: textMsg, callback function: (none) */
#define  MAINPNL_CAMERA_LABEL             20      /* control type: textMsg, callback function: (none) */
#define  MAINPNL_SHOULDER_BACK            21      /* control type: picture, callback function: (none) */
#define  MAINPNL_ARM_IMAGE                22      /* control type: picture, callback function: OGLCallback */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK AdjustCameraPosition(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK AdjustElbowPosition(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK AdjustShoulderPosition(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK AdjustWristPosition(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OGLCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OpenCloseClaw(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif