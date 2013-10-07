///////////////////////////////////////////////////////////////////////////////////////////
//
//                                        displayDiagnostics
//
// THis program will display diagnostic information about a FTC robot on the NXT LCD.
//
// If you want to use the LCD for your own debugging use, call the function
// "disableDiagnosticsDisplay()
//
///////////////////////////////////////////////////////////////////////////////////////////

#pragma systemFile            // this eliminates warning for "unreferenced" functions
#pragma autoStartTasks
#include "JoystickDriver.c"

void getUserControlProgram(string& sFileName);

void disableDiagnosticsDisplay()
{
  bDisplayDiagnostics = false;   // Disable diagnostic display
}

///////////////////////////////////////////////////////////////////////////////////////////
//
//                                        getUserControlProgram
//
// This function returns the name of the FTC User Control program. It reads the file
// "FTCConfig.txt" and builds the name of the file from the contents.
//
// Note that the filename includes the ".rxe" (robot executable file) file extension.
//
///////////////////////////////////////////////////////////////////////////////////////////

void getUserControlProgram(string& sFileName)
{
  byte   nParmToReadByte[2];
  int    nFileSize;
	TFileIOResult nIoResult;
	TFileHandle hFileHandle;

  sFileName = "";
  nParmToReadByte[1] = 0;
  hFileHandle = 0;
  OpenRead(hFileHandle, nIoResult, kConfigName, nFileSize);
  if (nIoResult == ioRsltSuccess)
  {
    for (int index = 0; index < nFileSize; ++index)
    {
      strncat(sFileName, (char) ReadByte(hFileHandle, nIoResult, nParmToReadByte[index]), 1);
    }

    //
    // Delete the ".rxe" file extension
    //
    int nFileExtPosition;

    nFileExtPosition = strlen(sFileName) - 4;
    if (nFileExtPosition > 0)
      StringDelete(sFileName, nFileExtPosition, 4);
  }
  Close(hFileHandle, nIoResult);
  return;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                    waitForStart
//
// Wait for the start of either the autonomous or tele-op phase. User program is running on the NXT
// but the phase has not yet started. The FMS (Field Management System) is continually (every 50 msec)
// sending information to the NXT. This program loops getting the latest value of joystick settings.
// When it finds that the FMS has started the  phase, it immediately returns.
//
// Perhaps this function should be moved into the "JoystickDriver.c" file.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

void waitForStart()
{
  while (true)
  {
    getJoystickSettings(joystick);
    if (!joystick.StopPgm)
      break;
  }
  return;
}
