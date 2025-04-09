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
	servoBailey.attach(9); //Attach servo to pin
	pinMode(ledDoor, OUTPUT);
	pinMode(ledBailey, OUTPUT);
}

void loop()
{
	//Initialize positions and states
	if (frame == 0){
		servoDoor.write(0);
		if (digitalRead(switchDoor) == HIGH){//Token is in starting place 
			digitalWrite(ledDoor, HIGH);
d			frame++;}//Advance tracker
	}//End of reset 
	
	//Larkhill 
	if (frame == 1){
		if (digitalRead(switchDoor) == LOW){//Once token is removed
			digitalWrite(ledDoor, LOW);//Turn off led 
			frame++;//Advance frame tracker 
		}//End of if token removed 
	}//End of Larkhill 
	
	//Old Bailey
	if (frame == 2){
		if (digitalRead(switchBailey) == HIGH){//Token is placed
			for(int pos = 0; pos <= 90; pos++){//Platform sweep 
				servoBailey.write(pos);//Move the servo 
				delay(11);//To complete 90° in roughly 1 second 
			}//End of sweep 
			digitalWrite(ledBailey, HIGH);//Bailey explodes 
		}//End of token trigger 
	}//End of old bailey 
	
	//Add full story completion checking code here
}//End of loop 