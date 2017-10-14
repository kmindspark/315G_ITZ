
#include "helperfunctions.c"

string autons[5] = {"No Auton", "20 Pt Cone", "5 Pt Cone", "1 Cone", "iMaDeFeNsEbOt"};

void autonomousConeIn20Pt(bool reverse, bool stopAfterTime, bool fivept)
{
	closeClaw();
	forward(127,300);
	//turnRight(127,250,reverse);
	//turnLeft(127,250,reverse);
	assignArmMotors(127);
	assignMogoMotors(-127);
	forward(127,400);
	assignArmMotors(10);
	forward(127,400);
	wait1Msec(400);
	assignMogoMotors(0);
	forward(127, 650);
	assignMogoMotors(127);
	wait1Msec(1300);
	assignMogoMotors(0);
	assignArmMotors(-65);
	wait1Msec(600);
	assignArmMotors(0);
	wait1Msec(500);
	openClaw();
	if (stopAfterTime){
		return;
	}
	assignArmMotors(127);
	wait1Msec(300);
	assignArmMotors(10);
	//backward(127,1150);
	backward(127,1050);
	turnRight(127,90,reverse);
	backward(127,800);
	if (fivept)
	{
		turnRight(127,90,reverse);
		assignMogoMotors(-127);
		wait1Msec(1000);
		assignMogoMotors(0);
		backward(127,500);
		return;
}
	forward(127, 200);
	turnRight(127,50,reverse);
	forward(127, 1200);
	turnRight(127, 85, reverse);
	forward(100, 700);
	assignMogoMotors(-127);
	//
	forward(60, 700);
	wait1Msec(500);
	assignMogoMotors(0);
	backward(127, 300);
	assignMogoMotors(127);
	backward(127,1000);
	assignMogoMotors(0);
}

void autonDefense(){
	closeClaw();
	backward(127, 3000);
}
