#pragma config(Sensor, in2,    potArm,         sensorPotentiometer)
#pragma config(Sensor, in3,    potFlipFlop,    sensorPotentiometer)
#pragma config(Sensor, in4,    gyro,           sensorGyro)
#pragma config(Sensor, dgtl1,  leftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  rightEncoder,   sensorQuadEncoder)
#pragma config(Motor,  port1,           mogoL,         tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           rollers,          tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           dl,            tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           armL1,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           armL2,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           armR1,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           armR2,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           dr,            tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           flipflop,      tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          mogoR,         tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

#define vexCompetitionState (nVexRCReceiveState & (vrDisabled | vrAutonomousMode))
#define LEFTBUTTON 1
#define CENTERBUTTON 2
#define RIGHTBUTTON 4
#define SLEWCONST 0.2

#define DRIVEA 0.0000000032169
#define DRIVEM 0.447196
#define DRIVEB -6.71039

int goalDrivePowerL = 0;
int goalDrivePowerR = 0;
int currentDrivePowerL = 0;
int currentDrivePowerR = 0;

TVexReceiverState competitionState;
int numCones = 0;
int autonChoice = 0;
bool left = true;
bool brake = false;
bool reverseStack = false;
int autonForwardBrake = 0;
int goalDriveValue;
int autonAngleBrake = 0;
int goalDriveAngle;

int song = 3;

#include "autons.c"

task slew{
	int previousGoalPowerL = 0;
	int previousGoalPowerR = 0;
	int currentChangeL = 0;
	int currentChangeR = 0;
	int count = 0;
	while (true){
		if (previousGoalPowerL != goalDrivePowerL || previousGoalPowerR != goalDrivePowerR){
			previousGoalPowerL = goalDrivePowerL;
			previousGoalPowerR = goalDrivePowerR;
			currentChangeL = goalDrivePowerL - currentDrivePowerL;
			currentChangeR = goalDrivePowerR - currentDrivePowerR;
			count = 0;
		}
		if (count <= 5){
			count = count + 1;
			currentDrivePowerL = currentDrivePowerL + currentChangeL*SLEWCONST;
			currentDrivePowerR = currentDrivePowerR + currentChangeR*SLEWCONST;
		}
		else{
			currentDrivePowerL = goalDrivePowerL;
			currentDrivePowerR = goalDrivePowerR;
		}
		wait1Msec(20);
	}
}

int computeActualValue(int input){

	float inVal = ((float) abs(input));

	if (abs(input) < 15){
		return 0;
	} else {
		return input;
	}
	/*
	else if (input > 125){
		return 127;
	}
	else if (input < -125){
		return -127;
	}

	int returnVal = (int) (inVal*inVal*inVal*inVal*inVal*DRIVEA + inVal*DRIVEM + DRIVEB);

	if (input < 0) {
		returnVal *= -1;
	}
	return returnVal;*/
}

task drive(){
	autonForwardBrake = 0;
	startTask(slew);
	int forward;
	int turn;
	while (true){
		if (abs(computeActualValue(vexRT[Ch3])) > 0 || abs(computeActualValue(vexRT[Ch1])) > 0){
			brake = false;
			stopTask(brakeWheels);
			forward = computeActualValue(vexRT[Ch3]);
			turn = computeActualValue(vexRT[Ch1]);
		}
		else{
			forward = computeActualValue(vexRT[Ch3Xmtr2]);
			turn = computeActualValue(vexRT[Ch1Xmtr2]);
			if(SensorValue[potArm] >= LOADERARMPOS + 300)
			{
				turn /= 2;
				forward /= 2;
			}
		}



		goalDrivePowerL = forward + turn;
		goalDrivePowerR = forward - turn;
		motor[dl] = currentDrivePowerL;
		motor[dr] = currentDrivePowerR;

		if (vexRT[Btn8L]){
			brake = !brake;
			if (brake){
				SensorValue[leftEncoder] = 0;
				SensorValue[rightEncoder] = 0;
				SensorValue[gyro] = 0;
				goalDriveValue = 0;
				goalDriveAngle = 0;
				startTask(brakeWheels);
				} else {
				stopTask(brakeWheels);
			}
			wait1Msec(200);
		}
	}
}

task arm(){
	while(true){
		if(vexRT[Btn6U] == 1){
			stopTask(maintainArmPos);
			assignArmMotors(127);
			while(vexRT[Btn6U] == 1)
			{

			}
			assignArmMotors(10);
		}
		if(vexRT[Btn6D] == 1){
			stopTask(maintainArmPos);
			assignArmMotors(-127);
			while(vexRT[Btn6D] == 1)
			{
			}
			assignArmMotors(-10);
		}
		if (vexRT[Btn7R] == 1){
			stopTask(maintainArmPos);
			autoStackCones();
			numCones++;
		}
	}
}

task mogo(){
	while(true){
		if(vexRT[Btn8U]){
			assignMogoMotors(127);
			while(vexRT[Btn8U])
			{

			}
			assignMogoMotors(15);
		}
		if(vexRT[Btn8D]){
			assignMogoMotors(-127);
			while(vexRT[Btn8D] == 1)
			{

			}
			assignMogoMotors(-10);
		}
	}
}

task flipfloptask {
	while (true) {
		if (vexRT[Btn5U]){
			motor[flipflop] = 127;
			while (vexRT[Btn5U]){

			}
			motor[flipflop] = 10;
		}
		if (vexRT[Btn5D]){
			motor[flipflop] = -127;
			while (vexRT[Btn5D]){

			}
			motor[flipflop] = -15;
		}
	}
}

task rollerstask {
	while (true) {
		if (vexRT[Btn7U] == 1){
			while (vexRT[Btn7U] == 1){
				motor[rollers] = 127;
			}
			motor[rollers] = 30;
		}
		if (vexRT[Btn7D] == 1){
			motor[rollers] = -127;
			wait1Msec(300);
			motor[rollers] = 0;
		}
	}
}

task coneCounter(){
	while (true){
		if (vexRT[Btn8DXmtr2]){
			numCones = 0;
		}
		if (vexRT[Btn8LXmtr2]){
			numCones--;
		}
		if (vexRT[Btn8RXmtr2]){
			numCones++;
		}
	}
}


task playMusic{
	while (true){
		switch (song){
		case 1:
			//playSoundFile("life_1.wav");
			break;
		case 2:
			//playSoundFile("stars_4.wav");
			break;
		case 3:
			//playSoundFile("sandstorm_3.wav");
			break;
		}
		wait1Msec(2000);
	}
}

void pre_auton(){
	bLCDBacklight = true;
	//bPlaySounds = true;
	//nVolume = 4;
	displayLCDCenteredString(0, "Init. gyro");
	SensorType[gyro] = sensorNone;
	wait1Msec(2000);
	SensorType[gyro] = sensorGyro;
	wait1Msec(2000);
	competitionState = vexCompetitionState;
	clearLCDLine(0);
	clearLCDLine(1);
	bool chosen = false;
	displayLCDCenteredString(0, autons[autonChoice]);
	clearTimer(T1);
	if (nLCDButtons != LEFTBUTTON){
		startTask(playMusic);
		return;
	}
	while (!chosen && vexCompetitionState == competitionState){
		waitForPress();
		if (nLCDButtons == RIGHTBUTTON){
			waitForRelease();
			if (autonChoice < (sizeof(autons)/2) - 1){
				autonChoice++;
			}
			clearLCDLine(0);
			displayLCDCenteredString(0, autons[autonChoice]);
		}
		else if (nLCDButtons == LEFTBUTTON){
			waitForRelease();
			if (autonChoice > 0){
				autonChoice--;
			}
			clearLCDLine(0);
			displayLCDCenteredString(0, autons[autonChoice]);
		}
		else if (nLCDButtons == CENTERBUTTON){
			waitForRelease();
			clearLCDLine(0);
			displayLCDCenteredString(0, autons[autonChoice]);
			chosen = true;
		}
	}
	chosen = false;
	clearLCDLine(0);
	displayLCDCenteredString(0, "left  or  right");
	while (!chosen && vexCompetitionState == competitionState){
		waitForPress();
		if (nLCDButtons == RIGHTBUTTON){
			waitForRelease();
			left = false;
			chosen = true;
		}
		else if (nLCDButtons == LEFTBUTTON){
			waitForRelease();
			left = true;
			chosen = true;
		}
	}

	clearLCDLine(0);
	clearLCDLine(1);

	wait1Msec(500);

	displayLCDCenteredString(0, "Choose song");
	chosen = false;
	while (!chosen && vexCompetitionState == competitionState){
		waitForPress();
		if (nLCDButtons == LEFTBUTTON){
			waitForRelease();
			chosen = true;
			song = 1;
		}
		else if (nLCDButtons == CENTERBUTTON){
			waitForRelease();
			chosen = true;
			song = 2;
		}
		else if (nLCDButtons == RIGHTBUTTON){
			waitForRelease();
			chosen = true;
			song = 3;
		}
	}

	startTask(playMusic);
}


task autonomous()
{
	//turnLeft(127, 90, true);
	//return;
	switch (autonChoice){
	case 1: autonomousConeIn20Pt(!left, false, false, false, 3); break;
	case 2: autonomousConeIn20Pt(!left, false, false, false, 1); break;
	case 3: autonomousConeIn20Pt(!left, false, false, false, 0); break;
	case 4: autonomousConeIn20Pt(!left, false, true, false, 3); break;
	case 5: autonomousConeIn20Pt(!left, false, true, false, 1); break;
	case 6: autonomousConeIn20Pt(!left, false, true, false, 0); break;
	case 7: autonomousConeIn20Pt(!left, true, false, false, 3); break;
	case 8: autonomousStationary(!left, true); break;
	case 9: autonomousStationary(!left, false); break;
	case 10: autonDefense(); break;
	default: break;
	}

}

task usercontrol(){
	SensorValue[leftEncoder] = 0;
	SensorValue[rightEncoder] = 0;

	stopTask(playMusic);
	clearSounds();
	endAutoStackEarly = false;
	/*forwardDistance(127, 200);
	wait1Msec(3000);
	turnRight(127, 90, false);*/
	startTask(drive);
	startTask(mogo);
	startTask(arm);
	startTask(rollerstask);
	startTask(flipfloptask);
	startTask(mogo);
	startTask(coneCounter);
	while (true){
		while (vexRT[Btn8R]!=1){

		}
		stopTask(drive);
		stopTask(mogo);
		stopTask(arm);
		stopTask(rollerstask);
		stopTask(flipfloptask);
		stopTask(mogo);
		stopTask(coneCounter);
		wait1Msec(200);
		assignArmMotors(0);
		assignDriveMotors(0,0);
		assignFlipFlop(0);
		assignMogoMotors(0);
		motor[rollers] = 0;
		while (vexRT[Btn8R]!=1){

		}
		startTask(drive);
		startTask(mogo);
		startTask(arm);
		startTask(rollerstask);
		startTask(flipfloptask);
		startTask(mogo);
		startTask(coneCounter);
		wait1Msec(200);
	}
}
