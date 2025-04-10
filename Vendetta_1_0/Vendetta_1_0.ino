//This program handles sequencial interactions for the project 
//Reminder: Test servo code for first story frame + create code architecture for tracking story events.

#include <Servo.h>//Servo library 

//variables
Servo servo1, servo2;//We can add more as needed
int servPin1 = 9;//Pin for servo1
int switch1 = 7;//Pin for switch 1 
int frame = 0;//Track the active story frame

void setup()
{
	Serial.begin(9600);//Talk to serial display 
	  servo1.attach(servPin1); //Attach servo to pin
}

void loop()
{
	//Initialize positions and states
	if (frame == 0){
		servo1.write(0);
		frame++;//Advance tracker 
	}
	
	//Fingerman 
	if (frame == 1){
		if (digitalRead(switch1) == HIGH){//When V closes the circuit 
			servo1.write(90);//Set servo to 90°
			frame++;//Advance frame tracker 
		}//End of if switch closed 
	}//End of fingerman 
}//End of loop 
