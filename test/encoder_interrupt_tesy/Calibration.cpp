#include "Calibration.h"
#include  "SpiEncoder.h"

void calibrateY()
{
    digitalWrite(Y_DIR_PIN, LOW);
    pre_encodey=ReadEncoder(_Y);
    /*Serial.print("Initial");
    Serial.println(pre_encodey);*/
    for(int x =0; x<250;x++) {
    digitalWrite(Y_STEP_PIN,HIGH); 
    delayMicroseconds(1000); 
    digitalWrite(Y_STEP_PIN,LOW); 
    delayMicroseconds(1000);
  }
  YStatus = 1 ;
    while( ylimit ==0 & YStatus){
    digitalWrite(Y_STEP_PIN,HIGH); 
    delayMicroseconds(1000); 
    digitalWrite(Y_STEP_PIN,LOW); 
    delayMicroseconds(1000);
  }
  /*Serial.print("Ymin");
  Serial.println(ymin);*/
  noInterrupts();
  digitalWrite(Y_DIR_PIN, HIGH);
  for(int x =0; x<500;x++) {
    digitalWrite(Y_STEP_PIN,HIGH); 
    delayMicroseconds(1000); 
    digitalWrite(Y_STEP_PIN,LOW); 
    delayMicroseconds(1000);
  }
  encodey = ReadEncoder(_Y); 
  YStatus =1;
  interrupts();
  /*Serial.println("Second Stage");
  Serial.println(pre_encodey);*/
   while( ylimit ==1 & YStatus){
    digitalWrite(Y_STEP_PIN,HIGH); 
    delayMicroseconds(1000); 
    digitalWrite(Y_STEP_PIN,LOW); 
    delayMicroseconds(1000);
  }
  YStatus = 0;
//  Serial.print("Ymax");
//  Serial.println(ymax);
  digitalWrite(Y_DIR_PIN, LOW);
  for(int x =0; x<2500;x++) {
    digitalWrite(Y_STEP_PIN,HIGH); 
    delayMicroseconds(1000); 
    digitalWrite(Y_STEP_PIN,LOW); 
    delayMicroseconds(1000);
  }
}
void calibrateZ()
{
    digitalWrite(Z_DIR_PIN, LOW);
    pre_encodez = ReadEncoder(_Z);
    for(int x = 0; x < 1000; x++) {
        digitalWrite(Z_STEP_PIN, HIGH); 
        delayMicroseconds(500); 
        digitalWrite(Z_STEP_PIN, LOW); 
        delayMicroseconds(500);
    }
    ZStatus = 1;
    while (zlimit == 0 && ZStatus) {
        digitalWrite(Z_STEP_PIN, HIGH); 
        delayMicroseconds(500); 
        digitalWrite(Z_STEP_PIN, LOW); 
        delayMicroseconds(500);
    }
    digitalWrite(Z_DIR_PIN, HIGH);
    noInterrupts();
    for(int x = 0; x < 500; x++) {
        digitalWrite(Z_STEP_PIN, HIGH); 
        delayMicroseconds(500); 
        digitalWrite(Z_STEP_PIN, LOW); 
        delayMicroseconds(500);
    }
    
    encodez = ReadEncoder(_Z);
    ZStatus = 1;
    interrupts();
    while (zlimit == 1 && ZStatus) {
        digitalWrite(Z_STEP_PIN, HIGH); 
        delayMicroseconds(500); 
        digitalWrite(Z_STEP_PIN, LOW); 
        delayMicroseconds(500);
    }
    ZStatus = 0;
//    Serial.print("Zmax");
//    Serial.println(zmax);
}
void calibrateX()
{
    digitalWrite(X1_DIR_PIN, HIGH);
    pre_encodex1 = ReadEncoder(_X1);
    digitalWrite(X2_DIR_PIN, LOW);
    pre_encodex2 = ReadEncoder(_X2);
    /*Serial.print("Initial");
    Serial.println(pre_encodex1);*/
    
    for(int x = 0; x < 250; x++) {
        digitalWrite(X1_STEP_PIN, HIGH); 
        digitalWrite(X2_STEP_PIN, HIGH); 
        delayMicroseconds(1000); 
        digitalWrite(X1_STEP_PIN, LOW); 
        digitalWrite(X2_STEP_PIN, LOW); 
        delayMicroseconds(1000);
    }
    X1Status = 1;
    //X2Status = 1;
    while (x1limit == 0 ) {
        digitalWrite(X1_STEP_PIN, HIGH); 
        digitalWrite(X2_STEP_PIN, HIGH); 
        delayMicroseconds(1000); 
        digitalWrite(X1_STEP_PIN, LOW); 
        digitalWrite(X2_STEP_PIN, LOW);
        delayMicroseconds(1000);
    }
    noInterrupts();
    digitalWrite(X1_DIR_PIN, LOW);
    digitalWrite(X2_DIR_PIN, HIGH);
    for(int x = 0; x < 1000; x++) {
        digitalWrite(X1_STEP_PIN, HIGH); 
        digitalWrite(X2_STEP_PIN, HIGH);
        delayMicroseconds(1000); 
        digitalWrite(X1_STEP_PIN, LOW); 
        digitalWrite(X2_STEP_PIN, LOW);
        delayMicroseconds(1000);
    }
    encodex1 = ReadEncoder(_X1); 
    X1Status = 1;
    interrupts();
    /*Serial.println("Second Stage");
    Serial.println(pre_encodex1);*/
    while (x1limit==1) {
        digitalWrite(X1_STEP_PIN, HIGH); 
        digitalWrite(X2_STEP_PIN, HIGH);
        delayMicroseconds(1000); 
        digitalWrite(X1_STEP_PIN, LOW); 
        digitalWrite(X2_STEP_PIN, LOW); 
        delayMicroseconds(1000);
    }
    X1Status = 0;
//    Serial.print("X1max");
//    Serial.println(x1max);
    X2Status = 0;
//    Serial.print("X2max");
//    Serial.println(x2max);
}