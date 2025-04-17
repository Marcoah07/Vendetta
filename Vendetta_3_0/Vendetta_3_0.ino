//This program handles sequencial interactions for the project 

#include <Servo.h>//Servo library 
#include "melody.h" //Coded melodies 

//variables
Servo servoBailey, servoDance, servoTrain, servoFire;//We can add more servo objects as needed
int switchDoor = 7, switchBailey = 8, switchRoses = 12, switchDance = 11, switchTrain = 13;//Pins for switches
int ledDoor = 2, ledBailey = 1, ledRain = 4;//Pin for led1 
int frame = 0;//Track the active story frame
int buzzer = 3;//Pin for piezo 
bool storyComplete = true;
bool playing = true;//Decide whether music should play 
int currentNote = 0;//Track melody note
unsigned long prevNote = 0;//Millis timer for note duration 

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
	pinMode(buzzer, OUTPUT);
}

void loop()
{
	//Initialize positions and states
	if (frame == 0){
		if (storyComplete){
			servoBailey.write(0);
			servoDance.write(0);
			servoTrain.write(0);
			servoFire.write(0);
			noTone(buzzer);
			playing = true;
			currentNote = 0;
			storyComplete = false;}
		if (digitalRead(switchDoor) == HIGH){//Token is in starting place 
			digitalWrite(ledDoor, HIGH);
			frame = 1;;}//Advance tracker
	}//End of reset 
	
	//Larkhill 
	if (frame == 1){
		if (digitalRead(switchDoor) == LOW){//Once token is removed
			digitalWrite(ledDoor, LOW);//Turn off led 
			frame = 2;;//Advance frame tracker 
			playing = true;//Prepare to play music 
		}//End of if token removed 
	}//End of Larkhill
	
	//Old Bailey
	if (frame == 2){
		if (digitalRead(switchBailey) == HIGH){//Token is placed
			if (servoBailey.read() < 90){
				for(int pos = 0; pos <= 90; pos++){//Platform sweep 
					servoBailey.write(pos);//Move the servo 
					delay(11);//To complete 90° in roughly 1 second 
				}//End of sweep 
			}//End of if motion incomplete 
			digitalWrite(ledBailey, HIGH);//Bailey explodes 
			play(melodyBailey, 15, numNotesBailey, true);
		}//End of token trigger 
		if (digitalRead(switchRoses) == HIGH){//if roses are in place 
			currentNote = 0;
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
		play(melodyDance, 160, numNotesDance, true);
		if (digitalRead(switchTrain) == HIGH){//Advance story upon next switch being closed 
			currentNote = 0;
			frame = 5;}
}//End of Dance 
	
	//Parliament
	if (frame == 5){
		servoTrain.write(180);
		servoFire.write(180);
		play(melodyParli, 15, numNotesParli, false);
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
		if (currentNote > melodyLength- 1){
			currentNote = 0;
			if (!looping){
				playing = false;}
		}//End of loop music 
	}//End of if playing 
}//End of play()