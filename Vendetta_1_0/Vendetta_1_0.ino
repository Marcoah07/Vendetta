//This program handles sequencial interactions for the project 

#include <Servo.h>//Servo library 

//variables
Servo servo1, servo2;//We can add more as needed
int servPin1 = 9;//Pin for servo1
int switch1 = 7;//Pin for switch 1 
int ledPin1;//Pin for led1 
int frame = 0;//Track the active story frame

void setup()
{
	Serial.begin(9600);//Talk to serial display 
	servo1.attach(servPin1); //Attach servo to pin
	pinMode(ledPin1, OUTPUT);
}

void loop()
{
	//Initialize positions and states
	if (frame == 0){
		servo1.write(0);
		if (digitalRead(switch1) == HIGH){//Token is in starting place 
			digitalWrite(ledPin1, HIGH);
			frame++;}//Advance tracker
	}//End of reset 
	
	//Larkhill 
	if (frame == 1){
		if (digitalRead(switch1) == LOW){//Once token is removed
			digitalWrite(ledPin1, LOW);//Turn off led 
			frame++;//Advance frame tracker 
		}//End of if token removed 
	}//End of Larkhill 
	
	//Add full story completion checking code here
}//End of loop 