#include <SPI.h>
#include "pin.h"
#include "SpiEncoder.h"
#include "Calibration.h"
#include "Move.h"
#include "Position_tracking.h"
volatile long pre_encodey ;
volatile  long encodey;
int ylimit = 0;
long int  ymax=45496,ymin=0;
bool YStatus = 0;

volatile long pre_encodez ;
volatile long encodez;
int zlimit = 0;
long int  zmax,zmin;
int pre_z=0;
bool ZStatus =0;

volatile long pre_encodex1;
volatile long encodex1;
long int x1max, x1min;
bool X1Status = 0;

volatile long pre_encodex2;
volatile long encodex2;
long int x2max, x2min;
bool X2Status = 0;




void setup() {
  // Enables the motor to move in a particular direction,Y=LOW,left
  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_ENABLE_PIN, OUTPUT);
  digitalWrite(Y_DIR_PIN, LOW);

  // Enables the motor to move in a particular direction,Z=LOW,down
  pinMode(Z_STEP_PIN,OUTPUT); 
  pinMode(Z_DIR_PIN,OUTPUT);
  pinMode(Z_ENABLE_PIN,OUTPUT);
  
 // Enables the motor to move in a particular direction,forward X1 = HIGH,X2= LOW
  pinMode(X1_STEP_PIN,OUTPUT); 
  pinMode(X1_DIR_PIN,OUTPUT);
  pinMode(X1_ENABLE_PIN,OUTPUT);
  digitalWrite(X1_ENABLE_PIN,LOW);
  pinMode(X2_STEP_PIN,OUTPUT); 
  pinMode(X2_DIR_PIN,OUTPUT);
  pinMode(X2_ENABLE_PIN,OUTPUT);
  digitalWrite(X1_DIR_PIN,HIGH); 
  digitalWrite(X2_DIR_PIN,LOW);
   pinMode(LED_PIN,OUTPUT); 
  
  
  pinMode(READ_ENA_PIN, INPUT_PULLUP);
  pinMode(NSS_PIN, OUTPUT);
  digitalWrite(NSS_PIN, HIGH);
  Serial.begin(9600);
  SPI.begin();
  /* TCCR5A = 0;
  TCCR5B = 0;
  TCNT5  = 0;
  // Set the compare match register for 100Hz
  OCR5A = 24999;  // (16,000,000 / (64 * 100)) - 1
  // Set CTC mode (Clear Timer on Compare Match)
  TCCR5B |= (1 << WGM52);
  TCCR5B |= (1 << CS51) | (1 << CS50);
  // Enable Timer5 compare interrupt
  TIMSK5 |= (1 << OCIE5A);

  calibrateZ();
  OCR5A = 2499;
  calibrateY();
  calibrateX();
   TIMSK5 &= ~(1 << OCIE5A);  // Disable Timer3 compare match interrupt
   Movement(50,50,500,1);*/
   digitalWrite(LED_PIN, HIGH);

  
}

void loop() {
 if (Serial.available()) {
    // Read the incoming message as a string
    String input = Serial.readStringUntil('\n');  // Read until newline character

    // Remove possible whitespace or newline characters
    input.trim();

    // Call the function to extract values
    extractAndPrintValues(input);

    // Send "Done" message back to the 
    Serial.println("Done");
  }
  
}
void extractAndPrintValues(String input) {
  int xIndex = input.indexOf('(') + 1;
  int comma1 = input.indexOf(',', xIndex);
  int comma2 = input.indexOf(',', comma1 + 1);
  int comma3 = input.indexOf(',', comma2 + 1);
  int comma4 = input.indexOf(',', comma3 + 1);
  int tIndex = input.indexOf(')', comma4);

  // Parse x, y, z, t
  int x = input.substring(xIndex, comma1).toInt();
  int y = input.substring(comma1 + 1, comma2).toInt();
  int z = input.substring(comma2 + 1, comma3).toInt();
  int esp_pos = input.substring(comma3 + 1, comma4).toInt();
  int t = input.substring(comma4 + 1, tIndex).toInt();

  if(t==0)
  {
     // Stop Timer5 and reset settings
  TCCR5A = 0;
  TCCR5B = 0;
  TCNT5  = 0;
  // Set the compare match register for 100Hz
  OCR5A = 24999;  // (16,000,000 / (64 * 100)) - 1
  // Set CTC mode (Clear Timer on Compare Match)
  TCCR5B |= (1 << WGM52);
  TCCR5B |= (1 << CS51) | (1 << CS50);
  // Enable Timer5 compare interrupt
  TIMSK5 |= (1 << OCIE5A);

  calibrateZ();
  OCR5A = 12499;
  calibrateY();
  OCR5A = 24999;
  calibrateX();
   TIMSK5 &= ~(1 << OCIE5A);  // Disable Timer3 compare match interrupt
   Movement(50,50,400,0,t);
   return;
  }
  Movement(x,y,z,esp_pos,t);
  // Output the extracted values
}
ISR(TIMER5_COMPA_vect)//Interrupt service routine
{
  if (ZStatus) {
    pre_encodez= encodez;
    encodez = ReadEncoder(_Z);
  }
  if (YStatus) {
    pre_encodey= encodey;
    encodey = ReadEncoder(_Y);
  }
  if (X1Status) {
    pre_encodex1= encodex1;
    encodex1 = ReadEncoder(_X1);
  }
  if (X2Status) {
    pre_encodex2= encodex2;
    encodex2 = ReadEncoder(_X2);
  }
}
