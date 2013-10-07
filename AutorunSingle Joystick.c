#pragma config(Hubs,  S1, HTMotor,  HTServo,  none,     none)
#pragma config(Motor,  mtr_S1_C1_1,     ,              tmotorNone, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     ,              tmotorNone, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//
// Single joystick control over left and right motors

//    Y-Axis determines robot speed and direction.
//       +127    full speed "forward"
//          0    stopped.
//       -128    full speed "reverse".
//
//    X-Axis determines steering
//        -128   turn left with left motor same speed as right but in opposite direction
//         -64   turn left with left motor stopped.
//           0   straight line. Both motors at same speed.
//         +64   turn right with right motor stopped.
//        +127   turn right with right motor same speed as left but in opposite direction
//
#include "JoystickDriver.c"

task main()
{
  int nMotorPower;
  int nDirection;

  while (true)
  {
    getJoystickSettings(joystick); // Get current joystick settings
    nMotorPower = joystick.joy1_y1;
    nDirection = joystick.joy1_x1;
    if (nDirection < 0)
    {
	    // Turn left

    	motor[motorD] = nMotorPower; // use power settings from Y-axis
    	motor[motorE] = nMotorPower * (64 + nDirection) / 64;
    }
    else
    {
	    // Turn right

    	motor[motorD] = nMotorPower; // use power settings from Y-axis
    	motor[motorE] = nMotorPower * (64 - nDirection) / 64;
    }
  }
}
