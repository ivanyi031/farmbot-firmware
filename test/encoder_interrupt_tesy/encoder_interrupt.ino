#include <SPI.h>
#include "pin.h"
#include "SpiEncoder.h"
#include "Calibration.h"

long pre_encodey ;
long encodey;
int ylimit = 0;
long int  ymax=45496,ymin=0;
bool YStatus =0;
bool Yreach =0;


//target step for movement
int target_ystep ;
int target_x1step ;
int target_x2step ;
int target_zstep ;

//acculmulate step for movement
int ystep=1 ;
int x1step=1 ;
int x2step=1 ;
int zstep=1 ;

long pre_encodez ;
long encodez;
int zlimit = 0;
long int  zmax=103486,zmin=0;
bool ZStatus =0;
bool Zreach =0;


long pre_encodex1;
long encodex1;
int x1limit = 0;
long int x1max=45099, x1min=0;
bool X1Status = 0;


long pre_encodex2;
long encodex2;
int x2limit = 0;
long int x2max=43558, x2min=0;
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

  pinMode(LED_PIN,OUTPUT); 
  digitalWrite(LED_PIN, HIGH);
  
  
  pinMode(READ_ENA_PIN, INPUT_PULLUP);
  pinMode(NSS_PIN, OUTPUT);
  digitalWrite(NSS_PIN, HIGH);
  Serial.begin(9600);
  SPI.begin();

  // Timer5 for calibration
  TCCR5A = 0;// Clear Timer5 control register A
  TCCR5B = 0;// Clear Timer5 control register B
  TCNT5  = 0;// Initialize Timer3 counter
  //OCR5A = 2499;  // (16,000,000 / (64 * 100)) - 1
  TCCR5B |= (1 << WGM52);// Set CTC mode (Clear Timer on Compare Match)
  TCCR5B |= (1 << CS51) | (1 << CS50);// Enable Timer5 compare interrupt
  TIMSK5 |= (1 << OCIE5A);// Enable Timer5 compare match interrupt

  //calibrateY();
  OCR5A = 24999;
  calibrateZ();
//  OCR5A = 4999;
//  calibrateX();
  
  TIMSK5 &= ~(1 << OCIE5A);  // Disable Timer5 compare match interrupt
  Serial.println("Ready");
}

void loop() {
  /*long  EncoderData1 = ReadEncoder(_Y);
    Serial.print("EncoderDataY: ");
    Serial.println(EncoderData1);*/
  
  if (Serial.available()) {
    // Read the incoming message as a string
    String input = Serial.readStringUntil('\n');  // Read until newline character

    // Remove possible whitespace or newline characters
    input.trim();

    // Call the function to extract values
    extractAndPrintValues(input);

    // Send "Done" message back to the computer
    Serial.println("Done");
  }
}
void extractAndPrintValues(String input) {
  int xIndex = input.indexOf('(') + 1;
  int comma1 = input.indexOf(',', xIndex);
  int comma2 = input.indexOf(',', comma1 + 1);
  int comma3 = input.indexOf(',', comma2 + 1);
  int tIndex = input.indexOf(')', comma3);

  // Parse x, y, z, t
  int x = input.substring(xIndex, comma1).toInt();
  int y = input.substring(comma1 + 1, comma2).toInt();
  int z = input.substring(comma2 + 1, comma3).toInt();
  int t = input.substring(comma3 + 1, tIndex).toInt();

//  Serial.print("x: "); Serial.println(x);
//  Serial.print("y: "); Serial.println(y);
//  Serial.print("z: "); Serial.println(z);
//  Serial.print("t: "); Serial.println(t);
  Movement(x,y,z,t);
  // Output the extracted values
 
}

void Movement(int x, int y, int z, int t)
{
  long Yen = ReadEncoder(_Y);
  long X1en = -ReadEncoder(_X1);
  long X2en = ReadEncoder(_X2);
  long Zen = ReadEncoder(_Z);
  adjustValue(&Yen,ymax);
  adjustValue(&X1en,x1max);
  adjustValue(&X2en,x2max);
  adjustValue(&Zen,zmax);
  int Ypos = map(Yen, 0,ymax,0,1280);
  int X1pos = map(X1en, 0,x1max,0,1280);
  int X2pos = map(X2en, 0,x2max,0,1280);
  int Zpos = map(Zen, 0,zmax,0,739);
//  Serial.print("Ypos");
//  Serial.println(Ypos);
//  Serial.print("X1pos");
//  Serial.println(X1pos);
//  Serial.print("X2pos");
//  Serial.println(X2pos);
//  Serial.print("Zpos");
//  Serial.println(Zpos);
//  delay(5000);
  unsigned long timerXY;
  unsigned long timerZ;

  target_ystep = abs(Ypos-y)*10;
  target_x1step = abs(X1pos-x)*10;
  target_x2step = abs(X2pos-x)*10;
  target_zstep = abs(Zpos-z)*25*2;
  
  if (Ypos>y){
    digitalWrite(Y_DIR_PIN, LOW);
  }
  else{
    digitalWrite(Y_DIR_PIN, HIGH);
  }
  if (X2pos>x){
    
    digitalWrite(X1_DIR_PIN, HIGH);
    digitalWrite(X2_DIR_PIN, LOW);
  }
  else{
    digitalWrite(X1_DIR_PIN, LOW);
    digitalWrite(X2_DIR_PIN, HIGH);
  }
  if (Zpos>z){
    digitalWrite(Z_DIR_PIN, LOW);
  }
  else{
    digitalWrite(Z_DIR_PIN, HIGH);
  }
  digitalWrite(Z_STEP_PIN,HIGH);
  digitalWrite(Y_STEP_PIN,HIGH);
  digitalWrite(X1_STEP_PIN, HIGH);
  digitalWrite(X2_STEP_PIN, HIGH); 
  timerXY = micros();
  timerZ = micros();
  while(ystep<target_ystep || x1step< target_x1step || x2step<target_x2step || zstep<target_zstep)
  {
    unsigned long currentTime=micros();
    if(currentTime-timerXY>=1000)
    {
      if(ystep<target_ystep)
      {
        digitalWrite(Y_STEP_PIN,!digitalRead(Y_STEP_PIN));
        ystep++;
      }
      if(x1step<target_x1step)
      {
        digitalWrite(X1_STEP_PIN,!digitalRead(X1_STEP_PIN));
        x1step++;
        //Serial.println(x1step);
      }
      if(x2step<target_x2step)
      {
        digitalWrite(X2_STEP_PIN,!digitalRead(X2_STEP_PIN));
        x2step++;
        //Serial.println(x2step);
      }
      timerXY = currentTime;
    }
    if(currentTime-timerZ>=1000)
    {
      if(zstep<target_zstep)
      {
        digitalWrite(Z_STEP_PIN,!digitalRead(Z_STEP_PIN));
        zstep++;
      }
      timerZ = currentTime;
     }
    }
    target_ystep=0 ;
    target_x1step=0 ;
    target_x2step=0;
    target_zstep=0 ;

    ystep=1;
    x1step=1;
    x2step=1;
    zstep=1; 
}
void adjustValue(long *var, long max_var) {
    if (*var > max_var) {
        *var = max_var;  // Set to constant if greater
    } else if (*var < 0) {
        *var = 0;          // Set to 0 if negative
    }
}  

//Timer interrupt for calibration
ISR(TIMER5_COMPA_vect)//Interrupt service routine
{
  if (YStatus) {
    encodey = ReadEncoder(_Y);
    if(abs(encodey-pre_encodey)<10)
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
    //Serial.println(ylimit);
  }
  if (ZStatus) {
    encodez = ReadEncoder(_Z);
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
    //Serial.println(zlimit);
  }
  /*
  if (X2Status) {
    encodex2 = ReadEncoder(_X2);
    //Serial.println(encodex2);
    if (abs(encodex2 - pre_encodex2) < 50) {
        //Serial.println("Reach limit");
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
    //Serial.println(x2limit);
}*/
 if (X1Status) {
    encodex1 = ReadEncoder(_X1);
    Serial.println(encodex1);
    if (abs(encodex1 - pre_encodex1) < 10) {
        X1Status = 0;
        x1limit++;
        if (x1limit == 1) {
            ResetEncoder(_X1);
            x1min = ReadEncoder(_X1);
            encodex1 = x1min;
            ResetEncoder(_X2);
            x2min = 0;
        } else if (x1limit == 2) {
            x1max = -ReadEncoder(_X1);
            x2max = ReadEncoder(_X2);
        }
    }
    pre_encodex1 = encodex1;
    //Serial.println(x1limit);
}
}
