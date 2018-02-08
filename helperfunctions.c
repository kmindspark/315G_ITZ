#define FLIPFLOPDOWN 2600
#define FLIPFLOPUP 1230
#define BOTTOMARMPOS 500
#define LOADERARMPOS 1800
#define STATIONARYARMPOS 2650

int currentDownPos=BOTTOMARMPOS;
bool clawOpen = false;

int positions[13]={120, 260, 470, 540, 700, 890, 1010, 1140, 1270, 1400, 1530, 1660, 1760};

void turnLeft(int power, int degrees, bool reverse);
void turnRight(int power, int degrees, bool reverse);

int encoderAverage(int one, int two){
	return (abs(one) + abs(two))/2;
}

void closeClaw()
{
	clawOpen = false;
	motor[claw] = 127;
	wait1Msec(500);
	motor[claw] = 25;
}

void openClaw()
{
	clawOpen = true;
	motor[claw] = -127;
	wait1Msec(300);
	motor[claw] = -20;
}


void assignArmMotors(int power){
	motor[armL] = power;
	motor[armR] = power;
}

void assignDriveMotors(int lp, int rp){
	motor[df] = lp;
	motor[db] = lp;
	motor[pf] = rp;
	motor[pb] = rp;
}

void forwardDistance(int power, int distance){
	SensorValue[leftEncoder] = 0;
	SensorValue[rightEncoder] = 0;
	assignDriveMotors(power, power);
	while (encoderAverage(SensorValue[leftEncoder], SensorValue[rightEncoder]) < distance){
		//keep going
	}
	assignDriveMotors(-20, -20);
	wait1Msec(100);
	assignDriveMotors(0, 0);
}

void backwardDistance(int power, int distance){
	SensorValue[leftEncoder] = 0;
	SensorValue[rightEncoder] = 0;
	assignDriveMotors(-power, -power);
	while (encoderAverage(SensorValue[leftEncoder], SensorValue[rightEncoder]) < distance){
		//keep going
	}
	assignDriveMotors(20, 20);
	wait1Msec(100);
	assignDriveMotors(0, 0);
}

void forwardTime(int power, int time){
	assignDriveMotors(power, power);
	wait1Msec(time);
	assignDriveMotors(-10, -10);
	wait1Msec(100);
	assignDriveMotors(0, 0);
}

void backwardTime(int power, int time){
	assignDriveMotors(-power, -power);
	wait1Msec(time);
	assignDriveMotors(10, 10);
	wait1Msec(100);
	assignDriveMotors(0, 0);
}

void forward(int power, int time){
	forwardDistance(power, time*0.65);
	return;
	assignDriveMotors(power, power);
	wait1Msec(time);
	assignDriveMotors(-10, -10);
	wait1Msec(100);
	assignDriveMotors(0, 0);
}

void backward(int power, int time)
{
	backwardDistance(power, time*0.65);
	return;
	assignDriveMotors(-power,-power);
	wait1Msec(time);
	assignDriveMotors(10,10);
	wait1Msec(100);
	assignDriveMotors(0,0);
}

void turnRight(int power, int degrees, bool reverse)
{
	if (reverse)
	{
		turnLeft(power,degrees,false);
		return;
	}
	SensorValue[gyro] = 0;
	assignDriveMotors(power,-power);
	degrees = degrees * 10;
	while (abs(SensorValue[gyro]) < degrees){
		//do nothing
	}
	assignDriveMotors(-40,40);
	wait1Msec(180);
	assignDriveMotors(0,0);
}

void turnLeft(int power, int degrees, bool reverse)
{
	if (reverse)
	{
		turnRight(power,degrees,false);
		return;
	}
	SensorValue[gyro] = 0;
	assignDriveMotors(-power,power);
	degrees = degrees*10;
	while (abs(SensorValue[gyro]) < degrees){
		//do nothing
	}
	assignDriveMotors(40,-40);
	wait1Msec(180);
	assignDriveMotors(0,0);
}

void assignFlipFlop(int power)
{
	motor[flipflop] = power;
}

void autoStack(int numCones){
	closeClaw();
	assignFlipFlop(127);
	wait1Msec(200);
	assignFlipFlop(0);
	assignArmMotors(127);
	int goalPos = positions[numCones-1];
	while(SensorValue[potArm] < goalPos){
		if (SensorValue[potArm] > goalPos - 200){
			assignFlipFlop(127);
		}
	}
	assignArmMotors(10);
	assignFlipFlop(127);
	while(SensorValue[potFlipFlop] < FLIPFLOPUP){
		//wait
	}
	assignFlipFlop(0);
	assignArmMotors(-127);
	wait1Msec(300);
	openClaw();
	assignArmMotors(127);
	wait1Msec(200);
	assignFlipFlop(-127);
	wait1Msec(200);
	assignArmMotors(-127);
	while (SensorValue[potArm] > currentDownPos){
		//wait
	}
	assignArmMotors(-10);
	if (SensorValue[potFlipFlop] >= FLIPFLOPDOWN){
		assignFlipFlop(-127);
		while (SensorValue[potFlipFlop] >= FLIPFLOPDOWN){
			//wait
		}
	}
	assignFlipFlop(-5);
}

void assignMogoMotors(int power){
	motor[mogoL] = power;
	motor[mogoR] = -power;
}

void waitForPress(){
	while (nLCDButtons == 0 && vexCompetitionState == competitionState){
		wait1Msec(5);
	}
}

void waitForRelease(){
	while (nLCDButtons != 0){
		wait1Msec(5);
	}
}
