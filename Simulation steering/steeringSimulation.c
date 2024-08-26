#include <windows.h> 
#include <ansi_c.h>
#include <rs232.h>
#include <utility.h>
#include <cvirte.h>
#include <userint.h>
#include "steeringSimulation.h"

#ifndef TRUE
#define TRUE                1
#endif
//----------------------------------------------------------------------------
// Global variables
//----------------------------------------------------------------------------
static int panelHandle;
int biDirComPort;
int RS232err;

//----------------------------------------------------------------------------
// functions
//----------------------------------------------------------------------------
void DisplayRS232Error (int err);
void PushButton(int panel, int control);
//----------------------------------------------------------------------------
//  Main
//----------------------------------------------------------------------------
int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "steeringSimulation.uir", PANEL)) < 0)
		return -1;

	DisplayPanel (panelHandle);
	RunUserInterface ();
	//Close the open COM port before exiting
	if (biDirComPort)RS232err = CloseCom (biDirComPort);
	DiscardPanel (panelHandle);
	return 0;
}


//----------------------------------------------------------------------------
//  AdjustPush(): Send the Push button that was clicked
//----------------------------------------------------------------------------
int CVICALLBACK AdjustPush(int panel, int control, int event,
                           void *callbackData, int eventData1, int eventData2)
{
    char SendBuff = 0;
    char SendBuffStr[2];  // Create a string to hold the char and null terminator

    switch (event)
    {
        case EVENT_COMMIT:
            // Determine which control triggered the event
            switch (control)
            {
                case PANEL_JOYSTICK_LEFT:
                    SendBuff = 'L';
                    break;
                case PANEL_JOYSTICK_RIGHT:
                    SendBuff = 'R';
                    break;
                case PANEL_JOYSTICK_DOWN:
                    SendBuff = 'D';
                    break;
                case PANEL_JOYSTICK_UP:
                    SendBuff = 'U';
                    break;
                default:
                    return 0;  // If an unknown control, do nothing
            }

            // Convert the char to a string
            SendBuffStr[0] = SendBuff;
            SendBuffStr[1] = '\0';  // Null-terminate the string

            // Display the key command in the text message control
            SetCtrlVal(panel, PANEL_TEXTMSG, SendBuffStr);

            // Send the key command to the COM port
            ComWrt(biDirComPort, &SendBuff, 1);

            //delay
            Delay(0.05);

            break;

        default:
            break;
    }

    return 0;
}


/*******************************************************************/
int CVICALLBACK QuitCallback (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			QuitUserInterface (0);
			break;
	}
	return 0;
}

/*******************************************************************/
int CVICALLBACK ConnectComCb (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panelHandle, PANEL_COMCONNECT, &biDirComPort);
			//Ignore the error and continue running
			DisableBreakOnLibraryErrors ();
			RS232err = OpenComConfig (biDirComPort, "", 256000, 0, 8, 1, 4096,4096);
			if (RS232err)DisplayRS232Error(RS232err);
			else
			{
				
				//dimmed connect com button
				SetCtrlAttribute (panelHandle, PANEL_COMCONNECT, ATTR_DIMMED, 1);
				// Release the other buttons
				SetCtrlAttribute (panelHandle, PANEL_JOYSTICK_RIGHT, ATTR_DIMMED, 0);
				SetCtrlAttribute (panelHandle, PANEL_JOYSTICK_DOWN, ATTR_DIMMED, 0);
				SetCtrlAttribute (panelHandle, PANEL_JOYSTICK_LEFT, ATTR_DIMMED, 0);
				SetCtrlAttribute (panelHandle, PANEL_JOYSTICK_UP, ATTR_DIMMED, 0);
			}
			break;
	}
	return 0;
}

/*******************************************************************/
void DisplayRS232Error (int err)
{
	char *ErrorMessage;
	ErrorMessage = GetRS232ErrorString (err);
	MessagePopup ("RS232 Message", ErrorMessage);
}
