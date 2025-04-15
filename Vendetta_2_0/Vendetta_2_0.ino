//This program handles sequencial interactions for the project 

#include <Servo.h>//Servo library 

//variables
Servo servoBailey, servoDance, servoTrain, servoFire;//We can add more servo objects as needed
int switchDoor = 7, switchBailey = 8, switchRoses = 12, switchDance = 11, switchTrain = 13;//Pins for switches
int ledDoor = 2, ledBailey = 3, ledRain = 4;//Pin for led1 
int frame = 0;//Track the active story frame
bool storyComplete = true;

void setup()
{
	Serial.begin(9600);
	servoBailey.attach(9); //Attach servo to pin
	servoDance.attach(10);
	servoTrain.attach(5);
	servoFire.attach(6);
	pinMode(ledDoor, OUTPUT);
	pinMode(ledBailey, OUTPUT);
	pinMode(ledRain, OUTPUT);
}

void loop()
{
	//Initialize positions and states
	if (frame == 0){
		servoBailey.write(0);
		servoDance.write(0);
		servoTrain.write(0);
		servoFire.write(0);
		if (digitalRead(switchDoor) == HIGH){//Token is in starting place 
			digitalWrite(ledDoor, HIGH);
			frame = 1;;}//Advance tracker
		storyComplete = false;
	}//End of reset 
	
	//Larkhill 
	if (frame == 1){
		if (digitalRead(switchDoor) == LOW){//Once token is removed
			digitalWrite(ledDoor, LOW);//Turn off led 
			frame = 2;;//Advance frame tracker 
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
			//piezo code 
		}//End of token trigger 
		if (digitalRead(switchRoses) == HIGH){//if roses are in place 
		frame = 3;}
	}//End of old bailey 
	
	//Roses
	if (frame == 3){
		digitalWrite(ledRain, HIGH);
		if (digitalRead(switchDance) == HIGH){//Advance to next frame 
		frame = 4;}
	}//End of Roses 
	
	//Dance 
	if (frame == 4){
		digitalWrite(ledRain, LOW);
		//servo code 
		//Piezo code
		if (digitalRead(switchTrain) == HIGH){//Advance story upon next switch being closed 
			frame = 5;}
}//End of Dance 
	
	//Parliament
	if (frame == 5){
		servoTrain.write(180);
		servoFire.write(180);
		//Piezo
		if (servoFire.read() == 180){
		frame == 6;}
	}//End of parliament 
	
	
	//Reset Checker
	if (frame == 6 && digitalRead(switchBailey) == LOW && digitalRead(switchRoses) == LOW && digitalRead(switchDance) == LOW && digitalRead(switchTrain) == LOW){
	frame = 0;
	storyComplete = true;}//Restart 
	
}//End of loop           