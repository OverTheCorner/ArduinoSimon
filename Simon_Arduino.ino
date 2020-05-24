//Simon Game for Arduino
// 4 Colors
#include "pitches.h"

const int BUZZER=10;
const char COLORS[]={'R', 'Y', 'G', 'B'};  
const int NOTESMAP[]={NOTE_E4, NOTE_CS4, NOTE_A4, NOTE_E5};
const int LED_PINS[]={9, 7, 5, 3};
const int SW_PINS[]={8, 6, 4, 2};
const int NUM_OF_LEDS=4;
const int MAX_LEVEL=25;
const int TIME_LIMIT=5; //in seconds

int MELODY[MAX_LEVEL];

void setup() {
  pinMode(BUZZER, OUTPUT);
  for (int i=0; i<NUM_OF_LEDS; i++){
    pinMode(LED_PINS[i], OUTPUT);
  }
  
  for (int i=0; i<NUM_OF_LEDS; i++){
    pinMode(SW_PINS[i], INPUT);
  }
  
  Serial.begin(9600);
  randomSeed(analogRead(A0));
}


void loop() {
  int level=1;
  bool inKey=0;
  bool key=1;
  int score=0;
  unsigned long oldTime=0;
  unsigned long timeElapsed=0;

  Serial.println("Starting Game");
  playIntro();
  delay(1000);

   while (level<=MAX_LEVEL && key){
    score=level-1;
    Serial.println();
    Serial.print("Level: ");
    Serial.print(level);
    Serial.println();
    
    MELODY[level-1]=generateNote();
    playMelody(level);
    delay(1000);

    //Your Turn
    Serial.println("Your Turn");
    toggleAll(1);
    timeElapsed=0;

    for (int i=0; i<level; i++){
      inKey=0;
      oldTime=millis();
      
      while(timeElapsed<TIME_LIMIT*1000){
        timeElapsed=millis()-oldTime;

        //correct button press
        if (checkBttn(SW_PINS[MELODY[i]])){
          inKey=1;
          toggleAll(0);
          Serial.print(COLORS[MELODY[i]]);
          Serial.print(" ");
          toggleLED(LED_PINS[MELODY[i]], 1);
          playNote(LED_PINS[MELODY[i]]);
          delay(500);
          break;
        }
        
        if (checkBttn(SW_PINS[0]) && MELODY[i]!=0){
          toggleAll(LOW);
          toggleLED(LED_PINS[MELODY[i]], HIGH);
          Serial.println("RX");
          playWrong();
          toggleAll(LOW);
          break;
        }

        if (checkBttn(SW_PINS[1]) && MELODY[i]!=1){
          toggleAll(LOW);
          toggleLED(LED_PINS[MELODY[i]], HIGH);
          Serial.println("YX");
          playWrong();
          toggleAll(LOW);
          break;
        }

        if (checkBttn(SW_PINS[2]) && MELODY[i]!=2){
          toggleAll(LOW);
          toggleLED(LED_PINS[MELODY[i]], HIGH);
          Serial.println("GX");
          playWrong();
          toggleAll(LOW);
          break;
        }
        
        if (checkBttn(SW_PINS[3]) && MELODY[i]!=3){
          toggleAll(LOW);
          toggleLED(LED_PINS[MELODY[i]], HIGH);
          Serial.println("BX");
          playWrong();
          toggleAll(LOW);
          break;
        }
        
        continue; 
      }
      
      if (inKey==0){
        //Incorrect
        break;
        //exits level loop
      }
      
      else{
        //correct button press
        continue;
        //continue to next note
      }
    }
    
    if (inKey==0){
      key=0;
    }
    
    toggleAll(1);
    delay(500);
    toggleAll(0);
    delay(1000);
    level++;
  }
  Serial.print("Score:");
  Serial.print(score);
  Serial.println("");
  playLose();
  delay(1000);
}

void playMelody(int level){
  Serial.print("Melody:");
  
  for (int i=0; i<level; i++){
    Serial.print(COLORS[MELODY[i]]);
    Serial.print(" ");
    playNote(LED_PINS[MELODY[i]]);
    toggleLED(LED_PINS[MELODY[i]], 1);
    delay((500)*1.3);
    toggleLED(LED_PINS[MELODY[i]], 0);
    delay(250);
  }
  Serial.println();
}

void playWrong(){
  int NOTES[]={NOTE_GS4, NOTE_G4, NOTE_FS4, NOTE_E4};

  for (int i=0; i<(sizeof(NOTES)/sizeof(NOTES[0])); i++){

     if (i!=(sizeof(NOTES)/sizeof(NOTES[0]))-1){
       tone(BUZZER, NOTES[i], 1000/4);
       delay(250+100);
     }
     else{
       tone(BUZZER, NOTES[i], 1000/2);
       delay(500+100);
     }
  }
}

void playLose(){
  for (int n=0; n<5; n++){
    for (int i=0; i<NUM_OF_LEDS; i++){
      toggleAll(0);
      toggleLED(LED_PINS[i], 1);
      delay(100);
    }
  }
  for (int i=3; i>=0; i--){
    toggleAll(1);
    playNote(LED_PINS[i]);
    delay(500*1.3);
    toggleAll(0);
  }
}

int generateNote(){
  int i;
  i=random(0, 4);
  return i;
}

void playIntro(){

  for (int i=0; i<NUM_OF_LEDS; i++){
    toggleLED(LED_PINS[i], 1);
    playNote(LED_PINS[i]);
    delay((1000/4)*1.3);
  }

  for (int i=0; i<NUM_OF_LEDS; i++){
    toggleLED(LED_PINS[i], 0);
  }

  delay(1000);
}

void playNote(int led){
  if (led==LED_PINS[0]){
    tone(BUZZER, NOTESMAP[0], 1000/4);
  }
  if (led==LED_PINS[1]){
    tone(BUZZER, NOTESMAP[1], 1000/4);
  }
  if (led==LED_PINS[2]){
    tone(BUZZER, NOTESMAP[2], 1000/4);
  }
  if (led==LED_PINS[3]){
    tone(BUZZER, NOTESMAP[3], 1000/4);
  }
}

bool checkBttn(int bttn){
  if (digitalRead(bttn)==1) {
    return 0;
  }

  if (digitalRead(bttn)==0) {
    return 1;
  }
}

void toggleLED(int led, int state){
  digitalWrite(led, state);
}

void toggleAll(int state){
  for (int i=0; i<NUM_OF_LEDS; i++){
    toggleLED(LED_PINS[i], state);
  }
}
 /* Piano Mode
    for (int i = BLUE_LED; i<=RED_LED; i+=2){
    toggleLED(i, checkBttn(i-1));
    if (checkBttn(i-1)){
      playNote(i);
    }
  }
  */

 /* Button Counter
 int count;
  
 while (true) {
    int bttn=SW_YELLOW;
        
    while (!checkBttn(bttn)){
      continue;
    }
  
    count++;
    Serial.println(count);
  
    while(checkBttn(bttn)){
      continue;
    }
 }
 */
 //GFVA
