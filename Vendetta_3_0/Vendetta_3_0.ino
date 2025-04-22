//This program handles sequencial interactions for the project 

#include <Servo.h>//Servo library 
#include "melody.h" //Coded melodies 

//variables
Servo servoBailey, servoDance, servoTrain, servoFire;//We can add more servo objects as needed
int switchDoor = 7, switchBailey = 8, switchRoses = 12, switchDance = 11, switchTrain = 13;//Pins for switches
int ledDoor = 2, ledBailey = 4;//Pin for led1 
int frame = 0;//Track the active story frame
int buzzer = 3;//Pin for piezo 
bool storyComplete = true;
bool playing = true;//Decide whether music should play 
bool sweepUp = true;//Track the direction of a servo's movement (only for dance)
int currentNote = 0;//Track melody note
unsigned long prevNote = 0;//Millis timer for note duration 
unsigned long prevAngle = 0;//Millis timer for servo angles 

void setup()
{
	Serial.begin(9600);
	servoBailey.attach(9); //Attach servo to pin
	servoDance.attach(10);
	servoTrain.attach(5);
	servoFire.attach(6);
	pinMode(ledDoor, OUTPUT);
	pinMode(ledBailey, OUTPUT);
	pinMode(buzzer, OUTPUT);
	pinMode(switchDoor, INPUT);
	pinMode(switchBailey, INPUT);
	pinMode(switchRoses, INPUT);
	pinMode(switchDance, INPUT);
	pinMode(switchTrain, INPUT);
}

void loop()
{
	//Initialize positions and states
	if (frame == 0){
		if (storyComplete){
		servoBailey.write(69);
			servoDance.write(60);
			servoTrain.write(170);
			servoFire.write(0);
noTone(buzzer);
			playing = true;
			currentNote = 0;
			storyComplete = false;}
		if (digitalRead(switchDoor) == HIGH){//Token is in starting place 
			digitalWrite(ledDoor, HIGH);
			frame = 1;}//Advance tracker
	}//End of reset 
	
	//Larkhill 
	if (frame == 1){
		if (digitalRead(switchDoor) == LOW){//Once token is removed
			delay(2000);
			if (digitalRead(switchDoor) == LOW){//After delay 
				digitalWrite(ledDoor, LOW);//Turn off led 
				frame = 2;;//Advance frame tracker 
				playing = true;//Prepare to play music 
			}//End of after delay 
		}//End of if token removed 
	}//End of Larkhill
	
	//Old Bailey
	if (frame == 2){
		if (digitalRead(switchBailey) == HIGH){//Token is placed
			if (servoBailey.read() < 160){
				for(int pos = 75; pos <= 160; pos++){//Platform sweep 
					servoBailey.write(pos);//Move the servo 
					delay(11);//To complete 90° in roughly 1 second 
				}//End of sweep 
			}//End of if motion incomplete 
		}//End of token trigger 
		if (servoBailey.read() == 160){
			digitalWrite(ledBailey, HIGH);//Bailey explodes 
			play(melodyBailey, numNotesBailey, 30, false);
			Serial.println(currentNote);
			if (digitalRead(switchRoses) == HIGH){//if roses are in place 
				currentNote = 0;
				playing = true;
				frame = 3;}//Only read if interaction has triggered 
		}//End of if servo points up 
	}//End of old bailey 
	
	//Roses
	if (frame == 3){
		play(melodyRoses, numNotesRoses, 150, false);
		if (digitalRead(switchDance) == HIGH){//Advance to next frame 
			noTone(buzzer);
			frame = 4;
			playing = true;
			currentNote = 0;}
	}//End of Roses 
	
	//Dance 
	if (frame == 4){
		if (sweepUp){//Servo is sweeping in the positive direction 
			sweep(servoDance, 60, 120, 20);
			if (servoDance.read() == 120){//Turn 
				sweepUp = !sweepUp;}
		}//End of sweepUp
		else {//Sweep down 
			sweep(servoDance, 120, 60, 20);
			if (servoDance.read() == 60){//Turn	
				sweepUp = !sweepUp;}
		}//End of servo 
		play(melodyDance, numNotesDance, 160, true);
		if (digitalRead(switchTrain) == HIGH){//Advance story upon next switch being closed 
			noTone(buzzer);
			currentNote = 0;
			frame = 5;}
}//End of Dance 
	
	//Parliament
	if (frame == 5){
		sweep(servoTrain, 170, 120, 40);
		if (servoTrain.read() <= 120){//Wait for train to reach parliament for fire to spread 
			sweep(servoFire, 0, 120, 20);}
		if (servoTrain.read() < 145){//Wait for train to travel half way before music plays 
			play(melodyParli, numNotesParli, 45, false);}
		if (!playing){
			frame == 6;}
	}//End of parliament 
	
	
	//Reset Checker
	if (frame == 6 && digitalRead(switchBailey) == LOW && digitalRead(switchRoses) == LOW && digitalRead(switchDance) == LOW && digitalRead(switchTrain) == LOW){
	frame = 0;
	storyComplete = true;}//Restart 
	
	Serial.println(frame);
	
}//End of loop           

//This function plays a melody on the piezo 
void play(int melody[], int melodyLength, int noteLength, bool looping)
{
	if (playing){ 
	unsigned long currentMillis = millis();  //get the time the program has been running for in millis
		if (currentMillis - prevNote >= noteLength) {
			currentNote++;                   //move to next note
			prevNote = currentMillis;  //update note length timer
		}                                  //end of if note length timer concludes
		
		if (melody[currentNote] == 0)
			noTone(buzzer);  //don't play anything at rests
		else
			tone(buzzer, melody[currentNote]);  //piezo output
		
		//restart melody if it is complete
if 	(currentNote > melodyLength- 1){
			currentNote = 0;
			if (!looping){
				playing = false;}
		}//End of loop music 
	}//End of if playing 
}//End of play()

//This function sweeps the servo without delay()
void sweep(Servo servo, int angle1, int angle2, int delay)
{
	int pos = servo.read();//Get current angle 
	
	unsigned long currentMillis = millis();  //get the time the program has been running for in millis
	
	if (pos != angle2){//Only run if servo still needs to move 
		if (currentMillis - prevAngle >= delay) {
			if (angle1 > angle2){//Backward movement 
			servo.write(angle1-(angle1-pos)-1);}
			else{//Forward movement 
				servo.write(angle1+(pos-angle1)+1);}
			prevAngle = currentMillis; //update note length timer
		}//End of if interval completes 
	}//End of if servo has not reached angle2 
}//End of sweep