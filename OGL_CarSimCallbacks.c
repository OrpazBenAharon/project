#include "OGL_CarSimHeader.h"

int portNumber = 6;
int isRunning = 0;

int CVICALLBACK RingCallback(int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_RING_BEGIN_MENU:
			ClearListCtrl(mainPanel, MAINPNL_COMCONNECT);
			FindComPorts();
			break;

		case EVENT_COMMIT:
			DisableBreakOnLibraryErrors();
			int RS232err = OpenComConfig(portNumber, "", 256000, 0, 8, 1, 4096, 4096);
			if (RS232err)
				DisplayRS232Error(RS232err);
			else
			{
				SetCtrlAttribute(mainPanel, MAINPNL_COMCONNECT, ATTR_DIMMED, 1);
				GetCtrlVal(mainPanel, MAINPNL_COMCONNECT, &portNumber);
				CmtScheduleThreadPoolFunction(DEFAULT_THREAD_POOL_HANDLE, ThreadFunction, 0, 0);
				CmtScheduleThreadPoolFunction(DEFAULT_THREAD_POOL_HANDLE, CarPhysicsLoop, 0, 0);
			}
			break;
	}
	return 0;
}

void DisplayRS232Error(int err)
{
    char *errorMessage;
    errorMessage = GetRS232ErrorString(err);
    MessagePopup("RS232 Message", errorMessage);
    SetCtrlAttribute(mainPanel, MAINPNL_COMCONNECT, ATTR_DIMMED, 0);
}

void FindComPorts(void)
{
    int maxNum = 255;
    int portlist[255];
    int nPorts, i;
    char deviceName[16];

    // Enumerate all available ports
    nPorts = LocateCom("", portlist, maxNum);

    // Sort the port numbers in ascending order
    qsort(portlist, nPorts, sizeof(int), IntCompareWrapper);

    // Display the found ports on the ring control
    if (nPorts)
    {
        for (i = 0; i < nPorts; i++)
        {
            sprintf(deviceName, "COM%d", portlist[i]);
            InsertListItem(mainPanel, MAINPNL_COMCONNECT, -1, deviceName, portlist[i]);
        }
    }
}

int IntCompareWrapper(const void *item1, const void *item2)
{
    return IntCompare((void *)item1, (void *)item2);
}

static int CVICALLBACK ThreadFunction(void *functionData)
{
	DisableBreakOnLibraryErrors();
	int RS232err = OpenComConfig(portNumber, "", 256000, 0, 8, 1, 4096, 4096);
	if (RS232err)
		DisplayRS232Error(RS232err);
	else
	{
		SetCtrlAttribute(mainPanel, MAINPNL_COMCONNECT, ATTR_DIMMED, 1);
		Delay(0.1);
		FlushInQ(portNumber);
		InstallComCallback(portNumber, LWRS_RXCHAR, 0, 0, ComCallback, 0);
	}
	return 0;
}

void CVICALLBACK ComCallback(int portNumber, int eventMask, void *callbackData)
{
	char RecBuff;
	char RecBuffStr[2];
	ComRdTerm(portNumber, &RecBuff, 1, 0);
	RecBuffStr[0] = RecBuff;
	RecBuffStr[1] = '\0';
	SetCtrlVal(mainPanel, MAINPNL_TEXTMSG, RecBuffStr);
	switch (RecBuff)
	{
		case 'L':
			car.steeringAngle -= INCREMENT_ANGLE;
			break;
		case 'R':
			car.steeringAngle += INCREMENT_ANGLE;
			break;
		case 'U':
			isRunning = 1;
			break;
		case 'S':
			isRunning = 0;
			break;
	}
}

static int CVICALLBACK CarPhysicsLoop(void *functionData)
{
	while (1)
	{
		while (isRunning)
		{
			car.steeringAngle = Max(-MAX_ANGLE, Min(MAX_ANGLE, car.steeringAngle));
			car.forwardMove -= INCREMENT_FORWARD;
			Delay(FRAME_TIME/20);
		}
	Delay(0.1);
	}
}

int CVICALLBACK TimerCB(int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			OGLRefreshGraph(mainPanel, OGLControlPanel);
			break;
	}
	return 0;
}