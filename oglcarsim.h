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
#define  MAINPNL_COMCONNECT               2       /* control type: ring, callback function: ConnectButtonCallback */
#define  MAINPNL_ARM_IMAGE                3       /* control type: picture, callback function: OGLCallback */
#define  MAINPNL_TEXTMSG                  4       /* control type: textMsg, callback function: (none) */
#define  MAINPNL_STRING                   5       /* control type: string, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK ConnectButtonCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OGLCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
