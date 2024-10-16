#include <SPI.h>
#include "pin.h"
#include "SpiEncoder.h"
#include "Calibration.h"

long pre_encodey ;
long encodey;
int ylimit = 0;
long int  ymax=45496,ymin=0;
bool YStatus =0;

long pre_encodez ;
long encodez;
int zlimit = 0;
long int  zmax,zmin;
bool ZStatus =0;

long pre_encodex1;
long encodex1;
int x1limit = 0;
long int x1max, x1min;
bool X1Status = 0;

long pre_encodex2;
long encodex2;
int x2limit = 0;
long int x2max, x2min;
bool X2Status = 0;


void setup() {
  // Enables the motor to move in a particular direction,Y=LOW,left
  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_ENABLE_PIN, OUTPUT);

  // Enables the motor to move in a particular direction,Z=LOW,down
  pinMode(Z_STEP_PIN,OUTPUT); 
  pinMode(Z_DIR_PIN,OUTPUT);
  pinMode(Z_ENABLE_PIN,OUTPUT);
  
 // Enables the motor to move in a particular direction,forward X1 = HIGH,X2= LOW
  pinMode(X1_STEP_PIN,OUTPUT); 
  pinMode(X1_DIR_PIN,OUTPUT);
  pinMode(X1_ENABLE_PIN,OUTPUT);
  pinMode(X2_STEP_PIN,OUTPUT); 
  pinMode(X2_DIR_PIN,OUTPUT);
  pinMode(X2_ENABLE_PIN,OUTPUT);
  digitalWrite(X1_DIR_PIN,HIGH); 
  digitalWrite(X2_DIR_PIN,LOW);
  
  pinMode(READ_ENA_PIN, INPUT_PULLUP);
  pinMode(NSS_PIN, OUTPUT);
  digitalWrite(NSS_PIN, HIGH);
  Serial.begin(9600);
  SPI.begin();

   // Stop Timer5 and reset settings
  TCCR5A = 0;
  TCCR5B = 0;
  TCNT5  = 0;
  // Set the compare match register for 100Hz
  OCR5A = 2499;  // (16,000,000 / (64 * 100)) - 1
  // Set CTC mode (Clear Timer on Compare Match)
  TCCR5B |= (1 << WGM52);

  // Don't start the timer yet (do not set CS bits)
  TCCR5B |= (1 << CS51) | (1 << CS50);

  // Enable Timer5 compare interrupt
  TIMSK5 |= (1 << OCIE5A);

  // Enable global interrupts
  //interrupts();
  //calibrateY();
  OCR5A = 24999;
  calibrateZ();
//  OCR5A = 2499;
//  calibrateX();
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
ISR(TIMER5_COMPA_vect)//Interrupt service routine
{
  if (YStatus) {
    encodey = ReadEncoder(_Y);
    if(abs(encodey-pre_encodey)<500)
    {
      Serial.println("Reach limit");
      YStatus = 0;
      ylimit ++;
      if(ylimit==1){
        ResetEncoder(_Y);
        ymin = ReadEncoder(_Y);
        encodey = ymin;
      }
      else if(ylimit ==2){
        ymax = ReadEncoder(_Y);
      }
    }
    pre_encodey = encodey;
    Serial.println(ylimit);
  }
  if (ZStatus) {
    encodez = ReadEncoder(_Z);
    Serial.print("encodez");
    Serial.println(encodez);
    Serial.print("pre_encodez");
    Serial.println(pre_encodez);
    if(abs(encodez-pre_encodez)<500)
    {
      Serial.println("Reach limit");
      ZStatus = 0;
      zlimit ++;
      if(zlimit==1){
        ResetEncoder(_Z);
        zmin = ReadEncoder(_Z);
        encodez = zmin;
      }
      else if(zlimit ==2){
        zmax = ReadEncoder(_Z);
      }
    }
    pre_encodez = encodez;
    Serial.println(zlimit);
  }
 if (X1Status || X2Status) {
    encodex1 = ReadEncoder(_X1);
    if (abs(encodex1 - pre_encodex1) < 10) {
        Serial.println("Reach limit");
        X1Status = 0;
        x1limit++;
        if (x1limit == 1) {
            ResetEncoder(_X1);
            x1min = ReadEncoder(_X1);
            encodex1 = x1min;
        } else if (x1limit == 2) {
            x1max = ReadEncoder(_X1);
        }
    }
    pre_encodex1 = encodex1;
    Serial.println(x1limit);
}
if (X2Status) {
    encodex2 = ReadEncoder(_X2);
    if (abs(encodex2 - pre_encodex2) < 10) {
        Serial.println("Reach limit");
        X2Status = 0;
        x2limit++;
        if (x2limit == 1) {
            ResetEncoder(_X2);
            x2min = ReadEncoder(_X2);
            encodex2 = x2min;
        } else if (x2limit == 2) {
            x2max = ReadEncoder(_X2);
        }
    }
    pre_encodex2 = encodex2;
    Serial.println(x2limit);
}
}
