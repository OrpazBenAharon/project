#include <windows.h> 
#include "toolbox.h"
#include <ansi_c.h>
#include <rs232.h>
#include <utility.h>
#include <cvirte.h>
#include <userint.h>
#include "locatecom.h"
#include "Controller.h"

#define TRUE                1

//----------------------------------------------------------------------------
// Global variables
//----------------------------------------------------------------------------
static int panelHandle;
int portNumber;
int RS232err;

//----------------------------------------------------------------------------
// functions
//----------------------------------------------------------------------------
void DisplayRS232Error (int err);
void PushButton(int panel, int control);
void FindComPorts();
int IntCompareWrapper(const void *item1, const void *item2);
//----------------------------------------------------------------------------
//  Main
//----------------------------------------------------------------------------
int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "Controller.uir", PANEL)) < 0)
		return -1;

	DisplayPanel (panelHandle);
	RunUserInterface ();
	//Close the open COM port before exiting
	if (portNumber)RS232err = CloseCom (portNumber);
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
                case PANEL_JOYSTICK_UP:
                    SendBuff = 'U';
					break;
				case PANEL_RST:
                    SendBuff = 'S';
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
            ComWrt(portNumber, &SendBuff, 1);

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
void FindComPorts()
{
	int maxNum = 255;
	int portlist[maxNum];
	int nPorts, i;
	char deviceName[16];

	// Enumerate all available ports
	nPorts = LocateCom("", portlist, maxNum);

	// Sort the port numbers in ascending order
	qsort (portlist, nPorts, sizeof(int), IntCompareWrapper);
	
	// Display the found ports on the ring control
	if (nPorts)
	{
		for (i = 0; i < nPorts; i++)
		{
			sprintf(deviceName, "COM%d", portlist[i]);
			InsertListItem(panelHandle, PANEL_COMCONNECT, -1, deviceName, portlist[i]);
		}
	}
}

/*******************************************************************/  
// Wrapper function with the correct signature for the qsort
int IntCompareWrapper(const void *item1, const void *item2)
{
	// Call the original IntCompare function
	return IntCompare((void *)item1, (void *)item2);
}

/*******************************************************************/  
int CVICALLBACK RingCallback (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_RING_BEGIN_MENU: // Ring menu oppened
			ClearListCtrl (panelHandle, PANEL_COMCONNECT); // Clear the ring menu
			FindComPorts();	 // Find com ports and display on the ring menu
			break;

		case EVENT_COMMIT:
			 GetCtrlVal (panelHandle, PANEL_COMCONNECT, &portNumber);
			//Ignore the error and continue running
			DisableBreakOnLibraryErrors ();
			RS232err = OpenComConfig (portNumber, "", 256000, 0, 8, 1, 4096,4096);
			if (RS232err)DisplayRS232Error(RS232err);
			else
			{
				//dimmed connect com button
				SetCtrlAttribute (panelHandle, PANEL_COMCONNECT, ATTR_DIMMED, 1);
				// Release the other buttons
				SetCtrlAttribute (panelHandle, PANEL_JOYSTICK_RIGHT, ATTR_DIMMED, 0);
				SetCtrlAttribute (panelHandle, PANEL_JOYSTICK_LEFT, ATTR_DIMMED, 0);
				SetCtrlAttribute (panelHandle, PANEL_JOYSTICK_UP, ATTR_DIMMED, 0);
				SetCtrlAttribute (panelHandle, PANEL_RST, ATTR_DIMMED, 0);
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
