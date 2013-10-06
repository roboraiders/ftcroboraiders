#pragma config(Hubs,  S1, HTMotor,  HTServo,  none,     none)
#pragma config(Motor,  mtr_S1_C1_1,     motorD,        tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorNormal, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

///////////////////////////////////////////////////////////////////////////////////////////////
//
//                                FTC Sample Application
//
// A heavily commented program that forms a great template for teams to modify to provide
// customized control of their FTC robot.
//
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////
//
//                        PC Joystick Control over a FTC Robot
//
// During an FTC competition, in the tele-operated phase, messages are received over Bluetooth
// from the PC containing the current joystick settings.
//
// There's a separate ROBOTC task in the file "JoystickDriver" that completely handles the
// processing of these messages transparently to the end user program. All you have to do is
// use the preprocessor "#include" statement below to have this "driver" working in your
// program!
//
///////////////////////////////////////////////////////////////////////////////////////////////

#include "JoystickDriver.c"


///////////////////////////////////////////////////////////////////////////////////////////////
//
//                                 Joystick settings
//
// A variable of the structure "TJoystick" is used to hold the current settings of the joystick.
//
// Since the joystick is dynamically updated asynchronous to the user program, the settings read
// from the PC should be copied to a local variable so that they don't change while user program
// is working on them. The variable "joystick" is used to hold this local copy.
//
// User programs obtain the last values read from the PC via function call:
//           getJoystickSettings(joystick);
//
// "TJoystick" is a "struct" definition containing information on all the joystick settings. It is
// defined in the file "JoystickDriver.c" but a copy is included here (as comments) for readability
// of the user program.
//
//
//typedef struct
//{
//	bool		TeamColor;			   // Team Color (true - blue / false - red)
//	bool    UserMode;          // Autonomous or Telep-Operated mode
//	bool    StopPgm;           // Stop program
//
//	short   joy1_x1;           // -128 to +127
//	short   joy1_y1;           // -128 to +127
//	short   joy1_x2;           // -128 to +127
//	short   joy1_y2;           // -128 to +127
//	short   joy1_Buttons;      // Bit map for 12-buttons
//	short   joy1_TopHat;       // value -1 = not pressed, otherwise 0 to 7 for selected "octant".
//
//	short   joy2_x1;           // -128 to +127
//	short   joy2_y1;           // -128 to +127
//	short   joy2_x2;           // -128 to +127
//	short   joy2_y2;           // -128 to +127
//	short   joy2_Buttons;      // Bit map for 12-buttons
//	short   joy2_TopHat;       // value -1 = not pressed, otherwise 0 to 7 for selected "octant".
//} TJoystick;
//
// It's really easy to access the variables within the structure. For example:
//      joystick.joy1_x1       contains the X-axis value of joystick 1
//      joystick.joy1_y2       contains the Y-axis value of joystick 2
//
//////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////
//
//                            FTC Joystick LCD Information Display
//
// A collection of subroutines for displaying input from a remote joystick on the NXT LCD
// screen. This is not essential for your program but it does provide nice visual feedback
// of your robot.
//
// Two joysticks are displayed in a pair or rectangles. The joystick value is represented by
// a solid rectangle whose position corresponds to the joystick settings.
//
///////////////////////////////////////////////////////////////////////////////////////////////

void drawJoystick(int nLeft, int nOldX, int nOldY, int nNewX, int nNewY)
{
  //
  // Routine to draw a point (small rectangle) representing current joystick settings within
  // a rectangle on the NXT LCD screen.
  //
  // nLeft            the position of the rectangle
  // nOldX, nOldY     the previous joystick settings
  // nNewX, nNewY     the current  joystick settings.
  //
  const int kRectSize = 40;
  int nJoyXOld;
  int nJoyYOld;
  int nJoyXNew;
  int nJoyYNew;

  if ((nOldX == nNewX) && (nOldY == nNewY))
    return; // minimize LCD "flicker" by avoiding redrawing when there is no change

  // Normalize the joystick values to fit within the NXT LCD.

  nJoyXOld = (nLeft + (kRectSize - 4) / 2) + 1 + (nOldX * (kRectSize - 4) / 256);
  nJoyYOld = (10    + (kRectSize - 4) / 2) + 1 + (nOldY * (kRectSize - 4) / 256);

  nJoyXNew = (nLeft + (kRectSize - 4) / 2) + 1 + (nNewX * (kRectSize - 4) / 256);
  nJoyYNew = (10    + (kRectSize - 4) / 2) + 1 + (nNewY * (kRectSize - 4) / 256);

  // Errase rectangle for old setting and fill rectangle for new setting

  nxtEraseRect(nJoyXOld, nJoyYOld,  nJoyXOld + 3, nJoyYOld + 3);
  nxtFillRect(nJoyXNew, nJoyYNew,  nJoyXNew + 3, nJoyYNew + 3);

  // Draw the outline rectangle

  nxtDrawRect(nLeft, 10, nLeft + kRectSize, 10 + kRectSize);
}


void displayJoystickOnLCD()
{
  //
  // Draws the joystick settings on the NXT LCD screen
  //
  int nButton;
  string sButtons;
  string sStatus;
  static int nOldX1 = -200;
  static int nOldY1 = -200;
  static int nOldX2 = -200;
  static int nOldY2 = -200;
  static int nLastButtons = 0x100;

  sStatus = "";
  if (joystick.UserMode)
    sStatus = "User";
  else
    sStatus = "Automous";
  if (joystick.StopPgm)
    sStatus += " Paused";
  else
    sStatus += " Running";
  nxtDisplayTextLine(1, sStatus);

  // Draw Joystick 1 on the screen

  drawJoystick(0, nOldX1, nOldY1, joystick.joy1_x1, joystick.joy1_y1);
  nOldX1 = joystick.joy1_x1;
  nOldY1 = joystick.joy1_y1;

  // Draw joystick 2 on the screen

  drawJoystick(50, nOldX2, nOldY2, joystick.joy1_x2, joystick.joy1_y2);
  nOldX2 = joystick.joy1_x2;
  nOldY2 = joystick.joy1_y2;

  //
  // Display Button Settings on bottom text line.
  //
  // For each of the 12 buttons, display on a single line the character:
  //     '-' (not pressed) or
  //     'N' (i.e. char '0' to '7')
  //
  if (nLastButtons != joystick.joy1_Buttons)
  {
	  nLastButtons = joystick.joy1_Buttons;
	  sButtons = "";
	  for (nButton = 0; nButton < 12; ++nButton)
	  {
	    if (nLastButtons & (1 << nButton))
	    {
	      char cChar;

	      sButtons += " ";
	      cChar = (char) ('0' + nButton);
	      sButtons += cChar;
	    }
	  }
	  nxtDisplayTextLine(7, sButtons);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////
//
//                               proportionalJoystickDrive
//
// Very simply program to drive two FTC motors from a single joystick. Y-axis is used for
// speed and X-axis for steering.
//
//    Y-Axis determines robot speed and direction.
//       +127    full power "forward"
//          0    stopped.
//       -128    full power "reverse".
//
//    X-Axis determines steering. The further the joystick value is from 'zero' the faster
//    the rate of steering.
//        -128   turn left with left motor same speed as right but in opposite direction
//         -64   turn left with left motor stopped.
//           0   straight line. Both motors at same speed.
//         +64   turn right with right motor stopped.
//        +127   turn right with right motor same speed as left but in opposite direction
//
// This function is not used in this sample program. The 'dualJoystickDrive' is used instead.
// However, it is very simple to use this version. Simply replace the function call to
// 'dualJoystickDrive' in the main task with 'proportionalJoystickDrive'!
//
///////////////////////////////////////////////////////////////////////////////////////////////

void motorControlProportional()
{
  int nMotorPower;
  int nTurnPower;
  int nDirection;

  nMotorPower = joystick.joy1_y1;
  nDirection = joystick.joy1_x1;
  if (nDirection < 0)
  {
    // Turn left

    motor[motorE] = nMotorPower; // use power settings from Y-axis
    nTurnPower = (nMotorPower * (64 + nDirection)) / 64;
    motor[motorD] = nTurnPower;
  }
  else
  {
    // Turn right

    motor[motorE] = nMotorPower;
    nTurnPower = nMotorPower * (64 - nDirection) / 64;
    motor[motorD] = nMotorPower; // use power settings from Y-axis
  }
}



///////////////////////////////////////////////////////////////////////////////////////////////
//
//                                      dualJoystickDrive
//
// Very simply program to drive two FTC motors from the Y-axis values from two joysticks. The
// motor control is simply set to the value of the joysticks.
//
// Full motor reverse is -100, zero is stopped and full power formward is =100. Joystick values
// that are out of range are automatically coerced to the closest valid value
//
///////////////////////////////////////////////////////////////////////////////////////////////

void tankbotJoystickDrive()
{
  motor[motorD] = joystick.joy1_y1;  // Assign motor 0 power to Y-axis of joystick 1
  motor[motorE] = joystick.joy1_y2;  // Assign motor 1 power to Y-axis of joystick 2
}


///////////////////////////////////////////////////////////////////////////////////////////////
//
//                                      processJoystickButtons
//
// There are eight buttons on the joystick. Here's a simpler handler to process the
// buttons.
//
///////////////////////////////////////////////////////////////////////////////////////////////

//
// convenient declarations to isolate a single bit in the bit-map of pressed buttons
//
enum
{
  btn1mask       = 1 << 0,
  btn2mask       = 1 << 1,
  btn3mask       = 1 << 2,
  btn4mask       = 1 << 3,
  btn5mask       = 1 << 4,
  btn6mask       = 1 << 5,
  btn7mask       = 1 << 6,
  btn8mask       = 1 << 7
};

//
// convenient declarations for the "top-hat" (left most game pad)
//
enum
{
  topHatUnpressed  = -1,  // Tophat value when the north button is pushed.
  topHatNorth      =  0,  // Tophat value when the north button is pushed.
  topHatNorthEast  =  1,  // Tophat value when the northeast button is pushed.
  topHatEast       =  2,  // Tophat value when the east button is pushed.
  topHatSouthEast  =  3,  // Tophat value when the southeast button is pushed.
  topHatSouth      =  4,  // Tophat value when the south button is pushed.
  topHatSouthWest  =  5,  // Tophat value when the southwest button is pushed.
  topHatWest       =  6,  // Tophat value when the west button is pushed.
  topHatNorthWest  =  7,  // Tophat value when the northwest button is pushed.
};

task openAndRaiseArm();
task closeAndLowerArm();

void processJoystickButtons()
{
  static int nLastValue = 0;
  int nChanges;
  int nNewPresses;


  //
  // Servo 1 might be a lifting arm. Two buttons used to push to 'bottom' or 'top' position
  //
  if (joystick.joy1_Buttons & btn5mask)
    servo[servo1] = 0;         // Move servo 1 to extreme "left" on button 5 down
  if (joystick.joy1_Buttons & btn7mask)
    servo[servo1] = 255;       // Move servo 1 to extreme "left" on button 7 down

  //
  // Get a mask of new key presses, This is for one-time actions when button transitions to pressed
  //
  nChanges    = joystick.joy1_Buttons ^ nLastValue;
  nLastValue  = joystick.joy1_Buttons;
  nNewPresses = nChanges & joystick.joy1_Buttons;

  //
  // Play some tones when buttons transition to pressed state
  //
  if (nNewPresses & btn6mask)
    PlaySound(soundUpwardTones);    // button 6 pressed transition
  if (nNewPresses & btn8mask)
    PlaySound(soundDownwardTones);  // button 8 pressed transition

  //
  // Initiate tasks when button pushed
  //
  if (nNewPresses & btn4mask)
  {
    StopTask(openAndRaiseArm);
    StartTask(closeAndLowerArm);
  }
  if (nNewPresses & btn2mask)
  {
    StopTask(closeAndLowerArm);
    StartTask(openAndRaiseArm);
  }

  //
  // increment / decrement servo position based on button 1 & 3 transitions
  //
  int nNewServo;

  nNewServo = servo[servo2]; // read exising value
  if (nNewPresses & btn3mask)
  {
    if (nNewServo < 255) // Don't increment too big, otherwise decrement won't take effect until back in range
      nNewServo += 16;
  }
  if (nNewPresses & btn1mask)
  {
    if (nNewServo > 0)   // Don't decrement too big, otherwise inccrement won't take effect until back in range
      nNewServo -= 16;
  }
  servo[servo2] = nNewServo; // Now update the servo setting
}


task openAndRaiseArm()
{
  servo[servo3] = 50;     // close Claw
  wait1Msec(150);
  servo[servo4] = 200;    // raise Arm
  wait1Msec(200);
  return;
}

task closeAndLowerArm()
{
  servo[servo4] = 50;     // lower Arm
  wait1Msec(200);
  servo[servo3] = 250;    // open Claw
  wait1Msec(150);
  return;
}



///////////////////////////////////////////////////////////////////////////////////////////////
//
// Simple program to use during tele-operated mode. It simply loops forever getting the
// latest joystick settings and then using these to update the motors and servos.
//
///////////////////////////////////////////////////////////////////////////////////////////////

long nNumbCycles = 0;  // A counter of the number of cycles the program has executed
// Only purpose is an indicator to show how fast your program is running

task debuggingTask();

task main()
{
  PlaySound(soundBeepBeep);
  eraseDisplay();                   // Erase the LCD screen
  bNxtLCDStatusDisplay = true;      // Keep the system generated top line status display.
  //
  // Loop forever processing Joystick messages from PC
  //
  StartTask(debuggingTask);

  while (true)
  {
    getJoystickSettings(joystick);  // retrieve the latest joystick settings
    displayJoystickOnLCD();         // display joystick settings on the NXT LCD for debugging
    motorControlProportional();     // use joystick value to drive motors
    processJoystickButtons();       // handler for the 8-buttons on the joystick
    ++nNumbCycles;
  }
}


task debuggingTask()
{

	while (true)
	{
		wait1Msec(500);
	}
}
