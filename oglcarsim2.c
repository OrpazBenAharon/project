//----------------------------------------------------------------------------
//  Contains positioning UIR callbacks for the joystick controls.
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include <windows.h>
#include <utility.h>
#include <rs232.h>
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
#define INCREMENT       10.0

// Define panel reference vars
extern int mainPanel, OGLControlPanel;

// Define vars used for maniputlating object and view
extern double steeringAngle, forwardPush;

static int CVICALLBACK ThreadFunction (void *functionData);
void CVICALLBACK ComCallback (int MyCounter, int eventMask, void *callbackData);
void CVICALLBACK DeferredCallbackFunction (void *callbackData);
static int CVICALLBACK ThreadFunctionTimer (void *functionData);
void DisplayRS232Error (int err);
//----------------------------------------------------------------------------
// Variables
//----------------------------------------------------------------------------
int portNumber;
int start=0;
static CmtThreadLockHandle threadLock;
/*---------------------------------------------------------------------------*/
/* Connect to the COM port in a separate thread 					         */
/*---------------------------------------------------------------------------*/
int CVICALLBACK ConnectButtonCallback (int panel, int control, int event,
									   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			// Disable Connect and Ring controls
			SetCtrlAttribute (mainPanel, MAINPNL_COMCONNECT, ATTR_DIMMED, 1);

			// Get the port number from the Ring control
			GetCtrlVal (mainPanel, MAINPNL_COMCONNECT, &portNumber);

			// Initialize the lock
			CmtNewLock(NULL, 0, &threadLock);

			// Schedule the thread function to receive data
			CmtScheduleThreadPoolFunction(DEFAULT_THREAD_POOL_HANDLE, ThreadFunction, 0, 0);

			// Schedule the thread function for timer
			CmtScheduleThreadPoolFunction(DEFAULT_THREAD_POOL_HANDLE, ThreadFunctionTimer, 0, 0);
			break;
	}
	return 0;
}

static int CVICALLBACK ThreadFunction (void *functionData)
{
	// Open the COM port
	//Ignore the error and continue running
	DisableBreakOnLibraryErrors ();
	int RS232err = OpenComConfig (portNumber, "", 256000, 0, 8, 1, 4096, 4096);
	if (RS232err)DisplayRS232Error(RS232err);
	else
	{
		//dimmed connect com
		SetCtrlAttribute (mainPanel, MAINPNL_COMCONNECT, ATTR_DIMMED, 1);
		Delay(0.1); // Delay 100ms
		FlushInQ (portNumber);
		InstallComCallback (portNumber, LWRS_RXCHAR, 0, 0, ComCallback, 0);
	}
	return 0;
}

void CVICALLBACK ComCallback (int portNumber, int eventMask, void *callbackData)
{
	char RecBuff;
	char RecBuffStr[2];  // Create a string to hold the char and null terminator
	ComRdTerm (portNumber, &RecBuff, 1, 0);
	// Convert the char to a string
	RecBuffStr[0] = RecBuff;
	RecBuffStr[1] = '\0';  // Null-terminate the string
	// Set the value of the text message control
	SetCtrlVal (mainPanel, MAINPNL_TEXTMSG, RecBuffStr);
	switch (RecBuff)
	{
		case 'L':
			steeringAngle -= INCREMENT;
			break;
		case 'R':
			steeringAngle += INCREMENT;
			break;
		case 'U':
			CmtGetLock(threadLock);
			start = 1;  // Start the timer
			CmtReleaseLock(threadLock);
			break;
		case 'D':
			forwardPush += INCREMENT;
			break;
		default:
			break;
	}
	// Check for bounds
	if (steeringAngle >= 30)
		steeringAngle = 30;
	if (steeringAngle <= -30)
		steeringAngle = -30;
	// Tell the main thread to refresh the OGL graph
	PostDeferredCall (DeferredCallbackFunction, 0);
}

void CVICALLBACK DeferredCallbackFunction (void *callbackData)
{

	// Re-render the image to the OGL control
	OGLRefreshGraph (mainPanel, OGLControlPanel);

}

void DisplayRS232Error (int err)
{
	char *ErrorMessage;
	ErrorMessage = GetRS232ErrorString (err);
	MessagePopup ("RS232 Message", ErrorMessage);
	SetCtrlAttribute (mainPanel, MAINPNL_COMCONNECT, ATTR_DIMMED, 0);
}


static int CVICALLBACK ThreadFunctionTimer(void *functionData)
{
    static double interval = 0.08;  // Start with a 0.1 second interval
    static int dt = 0;             // Initialize dt
    static int level = 1;          // Initialize level
    char levelMessage[50];         // Buffer to hold the level-up message

    while (1)  // Infinite loop to keep the thread alive
    {
        // Wait until 'start' is set to 1
        CmtGetLock(threadLock);
        int localStart = start;
        CmtReleaseLock(threadLock);

        if (localStart)
        {
            // Timer loop: execute while 'start' is 1
            while (localStart)
            {
                Delay(interval);  // Wait for the interval

                // Check if it's time to level up
                if (dt % 100 == 0 && dt > 0)  // Avoid leveling up at dt = 0
                {
                    level++;  // Increase the level

                    // Decrease interval to speed up the game as levels increase
                    if (interval > 0.05)
                    {
                        interval -= 0.005;
                    }

                    // Format the level-up message
                    sprintf(levelMessage, "Level Up! New Level: %d", level);

                    // Display the message in the UI
                    SetCtrlVal(mainPanel, MAINPNL_STRING, levelMessage);
                }

                dt += 1;

                CmtGetLock(threadLock);
                forwardPush -= INCREMENT;
                localStart = start;  // Recheck start in case it was changed
                CmtReleaseLock(threadLock);

                // Tell the main thread to refresh the OGL graph
                PostDeferredCall(DeferredCallbackFunction, NULL);
            }
        }

        // Add a small delay to avoid busy-waiting
        Delay(0.1);  // Poll every 100ms
    }

    return;
}
