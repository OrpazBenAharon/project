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

#define  PANEL                            1
#define  PANEL_SPEEDIND                   2       /* control type: scale, callback function: (none) */
#define  PANEL_QUITBUTTON                 3       /* control type: command, callback function: QuitCallback */
#define  PANEL_COMCONNECT                 4       /* control type: ring, callback function: ConnectComCb */
#define  PANEL_SPLITTER                   5       /* control type: splitter, callback function: (none) */
#define  PANEL_TEXTMSG                    6       /* control type: textMsg, callback function: (none) */
#define  PANEL_JOYSTICK_LEFT              7       /* control type: pictButton, callback function: AdjustPush */
#define  PANEL_JOYSTICK_RIGHT             8       /* control type: pictButton, callback function: AdjustPush */
#define  PANEL_JOYSTICK_DOWN              9       /* control type: pictButton, callback function: AdjustPush */
#define  PANEL_JOYSTICK_UP                10      /* control type: pictButton, callback function: AdjustPush */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK AdjustPush(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ConnectComCb(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
