#pragma config(Motor,  port1,           driveLeft,     tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port6,           armMotor,      tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           clawMotor,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          driveRight,    tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)


#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)


#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

#define DRIVESPEED 0.5 //ranges between 0 to 1
#define ARCADE true //controls whether the drive is tank style (false) or arcade style (true)

int filter (int input){
	if (input > 20){
		return input;
	}
	if (input < -20){
		return input;
	}
	return 0;
}

task drive{
	while (true){
		if (!ARCADE){
			motor[driveLeft] = DRIVESPEED*((float) filter(vexRT[Ch3]));
			motor[driveRight] = DRIVESPEED*((float) filter(vexRT[Ch2]));
		}
		else {
			float forward = (float) filter(vexRT[Ch3]);
			float turn = (float) filter(vexRT[Ch1]);
			motor[port1] = DRIVESPEED*(forward + turn);
			motor[port10] = DRIVESPEED*(forward - turn);
		}
	}
}

task arm{
	while (true){
		if (vexRT[Btn6U] == 1){
			motor[port7] = 127;
			while (vexRT[Btn6U] == 1){

			}
			motor[port7] = 10;
		}
		else if (vexRT[Btn6D] == 1){
			while (vexRT[Btn6D] == 1){
				motor[port7] = -127;
			}
			motor[port7] = 0;
		}

		wait1Msec(100);
	}
}

task claw{
	while (true){
		if (vexRT[Btn5U] == 1){
			while (vexRT[Btn5U] == 1){
				motor[clawMotor] = 127;
			}
			motor[port6] = 10;
		}
		else if (vexRT[Btn5D] == 1){
			while (vexRT[Btn5D] == 1){
				motor[port6] = -127;
			}
			motor[port6] = 0;
		}
		wait1Msec(100);
	}
}

void assignArmMotor(int power){
	motor[port7] = power;
}

void assignClawMotor(int power){
	motor[port6] = power;
}

void assignDriveMotors(int leftPower, int rightPower){
	motor[port1] = leftPower;
	motor[port10] = rightPower;
}

void pre_auton(){

}

task autonomous(){

}

task usercontrol()
{
	startTask(drive);
	startTask(arm);
	startTask(claw);
}
