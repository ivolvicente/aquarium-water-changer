/**
   @licence - GPL
   @author Ivo Vicente <ivolvicente@gmail.com>
   @description SOON ADD SCHEME
*/

// PROJECT VARS
const boolean devMode = true;   // When Dev Mode is true, Serial comms are avaiable

const int AQ_MAX_LVL = 2; // Interrupt (pin 2, but 0 for interrupt) Aquarium Max level
const int AQ_MIN_LVL = 3; // Aquarium Min level
const int WT_MAX_LVL = 4; // Dirty Water Storage Max level
const int WT_MIN_LVL = 5; // Clean Water Storage Min Level
const int PUMP1 = 7;      // Relay 1 for Pump

int aq_max_state = 0;
int aq_min_state = 0;
int wt_max_state = 0;
int wt_min_state = 0;

volatile int pump1Status = 0; // Pump 0/Off 1/On

/**
   Arduino Setup Function
*/
void setup() {

  pinMode(AQ_MAX_LVL, INPUT);
  pinMode(AQ_MIN_LVL, INPUT);
  pinMode(WT_MAX_LVL, INPUT);
  pinMode(WT_MIN_LVL, INPUT);
  pinMode(PUMP1, OUTPUT);

  // Serial for Debug Proposes only in Development mode
  if (devMode) {
    Serial.begin(9600);
  }
}

/**
   Arduino Main Loop Function
*/
void loop() {    
  aq_max_state = digitalRead(AQ_MAX_LVL);
  aq_min_state = digitalRead(AQ_MIN_LVL);
  wt_max_state = digitalRead(WT_MAX_LVL);
  wt_min_state = digitalRead(WT_MIN_LVL);

  swl("AQ MAX STATE: " + String(aq_max_state));
  swl("AQ MIN STATE: " + String(aq_min_state));
  swl("WT MAX STATE: " + String(wt_max_state));
  swl("WT MIN STATE: " + String(wt_min_state));
  swl("--------------------------");  
  
  // Test  
  swl("PUMP1 MAX: "+ String(aq_max_state) +" - PUMP1 STATUS: "+ String(pump1Status));
  if(aq_max_state == HIGH && pump1Status==0){    
    pump1Status =1;
    
    
    pump1Attach(AQ_MAX_LVL, stopRefilling, FALLING);        
    swl("Interrupt Attach");  
    swl("Pump 1 Is:"+String(pump1Status));
    digitalWrite(PUMP1, LOW);
  }
  swl("--------------------------");    

  // Delay for next turn - 1 second
  delay(3000);
}

// 
void stopRefilling() {      
  pump1Status = 0;
  swl("STOP THE MADNESS");
  digitalWrite(PUMP1, HIGH); // Stop PUMP
  pumpDetach(AQ_MAX_LVL);
}

// For easy attach @param {int} pnum - Reference for Interrupt pin number
void pump1Attach(int pnum, void(*fnISR), int mode){
    // attachInterrupt(digitalPinToInterrupt(pnum), stopRefilling, FALLING);
    attachInterrupt(digitalPinToInterrupt(pnum), fnISR, mode);
}

// For easy detach @param {int} pnum - Reference for Interrupt pin number
void pumpDetach(int pnum){
    detachInterrupt(digitalPinToInterrupt(pnum));
}

// Communication Fuction to send data using Serial port
void swl (String txt) {
  if (devMode) {
    Serial.println(txt);
  }
}
