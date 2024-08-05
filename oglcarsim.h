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
#define  MAINPNL_JOYSTICK_LEFT            2       /* control type: picture, callback function: AdjustSteerAgnle */
#define  MAINPNL_JOYSTICK_UP              3       /* control type: picture, callback function: AdjustPush */
#define  MAINPNL_JOYSTICK_RIGHT           4       /* control type: picture, callback function: AdjustSteerAgnle */
#define  MAINPNL_JOYSTICK_DOWN            5       /* control type: picture, callback function: AdjustPush */
#define  MAINPNL_ARM_IMAGE                6       /* control type: picture, callback function: OGLCallback */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK AdjustPush(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK AdjustSteerAgnle(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OGLCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif