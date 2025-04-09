//This program handles sequencial interactions for the project 

#include <Servo.h>//Servo library 

//variables
Servo servoBailey, servoDance, servoTrain, servoFire;//We can add more servo objects as needed
int switchDoor = 7, switchBailey, switchRoses, switchDance, switchTrain;//Pins for switches
int ledDoor, ledBailey, ledRain;//Pin for led1 
int frame = 0;//Track the active story frame

void setup()
{
	Serial.begin(9600);//Talk to serial display 
	servoDoor.attach(9); //Attach servo to pin
	pinMode(ledDoor, OUTPUT);
}

void loop()
{
	//Initialize positions and states
	if (frame == 0){
		servoDoor.write(0);
		if (digitalRead(switchDoor) == HIGH){//Token is in starting place 
			digitalWrite(ledDoor, HIGH);
			frame++;}//Advance tracker
	}//End of reset 
	
	//Larkhill 
	if (frame == 1){
		if (digitalRead(switchDoor) == LOW){//Once token is removed
			digitalWrite(ledDoor, LOW);//Turn off led 
			frame++;//Advance frame tracker 
		}//End of if token removed 
	}//End of Larkhill 
	
	//Add full story completion checking code here
}//End of loop 