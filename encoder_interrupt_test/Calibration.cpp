#include "Calibration.h"
#include  "SpiEncoder.h"


void calibrateY()
{
    digitalWrite(Y_DIR_PIN, LOW);
    encodey=ReadEncoder(_Y);
    for(int x =0; x<500;x++) {
    digitalWrite(Y_STEP_PIN,HIGH); 
    delayMicroseconds(1000); 
    digitalWrite(Y_STEP_PIN,LOW); 
    delayMicroseconds(1000);
  }
  YStatus = 1 ;
    while(abs(encodey-pre_encodey)>10){//){
    digitalWrite(Y_STEP_PIN,HIGH); 
    delayMicroseconds(1000); 
    digitalWrite(Y_STEP_PIN,LOW); 
    delayMicroseconds(1000);
  }
  YStatus=0;
  digitalWrite(Y_DIR_PIN, HIGH);
  ResetEncoder(_Y);
  encodey=0;
  for(int x =0; x<500;x++) {
    digitalWrite(Y_STEP_PIN,HIGH); 
    delayMicroseconds(1000); 
    digitalWrite(Y_STEP_PIN,LOW); 
    delayMicroseconds(1000);
  } 
  YStatus =1;
   while( abs(encodey-pre_encodey)>10){
    digitalWrite(Y_STEP_PIN,HIGH); 
    delayMicroseconds(1000); 
    digitalWrite(Y_STEP_PIN,LOW); 
    delayMicroseconds(1000);
  }
  YStatus = 0;
  ymax=ReadEncoder(_Y);
//  Serial.print("Ymax");
//  Serial.println(ymax);
  
}
void calibrateZ()
{
    digitalWrite(Z_DIR_PIN, LOW);
    encodez = ReadEncoder(_Z);
    for(int x = 0; x < 500; x++) {
        digitalWrite(Z_STEP_PIN, HIGH); 
        delayMicroseconds(500); 
        digitalWrite(Z_STEP_PIN, LOW); 
        delayMicroseconds(500);
    }
    ZStatus = 1;
    while (abs(encodez-pre_encodez)>200) {
        digitalWrite(Z_STEP_PIN, HIGH); 
        delayMicroseconds(500); 
        digitalWrite(Z_STEP_PIN, LOW); 
        delayMicroseconds(500);
    }
    ZStatus =0;
    ResetEncoder(_Z);
    encodez = 0;
    digitalWrite(Z_DIR_PIN, HIGH);
    for(int x = 0; x < 500; x++) {
        digitalWrite(Z_STEP_PIN, HIGH); 
        delayMicroseconds(500); 
        digitalWrite(Z_STEP_PIN, LOW); 
        delayMicroseconds(500);
    }
    ZStatus = 1;
    while (abs(encodez-pre_encodez)>200) {
        digitalWrite(Z_STEP_PIN, HIGH); 
        delayMicroseconds(500); 
        digitalWrite(Z_STEP_PIN, LOW); 
        delayMicroseconds(500);
    }
    ZStatus = 0;
    zmax=ReadEncoder(_Z);
//    Serial.print("Zmax");
//    Serial.println(zmax);
}
void calibrateX()
{
    digitalWrite(X1_DIR_PIN, HIGH);
    encodex1 = ReadEncoder(_X1);
    digitalWrite(X2_DIR_PIN, LOW);
    encodex2 = ReadEncoder(_X2);
    for(int x = 0; x < 500; x++) {
        digitalWrite(X1_STEP_PIN, HIGH); 
        digitalWrite(X2_STEP_PIN, HIGH); 
        delayMicroseconds(1000); 
        digitalWrite(X1_STEP_PIN, LOW); 
        digitalWrite(X2_STEP_PIN, LOW); 
        delayMicroseconds(1000);
    }
    X1Status = 1;
    X2Status = 1;
    while (abs(encodex1-pre_encodex1)>100 || abs(encodex2-pre_encodex2>100)) {
      
        digitalWrite(X1_STEP_PIN, HIGH); 
      
        digitalWrite(X2_STEP_PIN, HIGH); 
        delayMicroseconds(1000); 
        digitalWrite(X1_STEP_PIN, LOW); 
        digitalWrite(X2_STEP_PIN, LOW);
        delayMicroseconds(1000);
    }
    X1Status=0;
    X2Status=0;
    ResetEncoder(_X1);
    encodex1=0;
    ResetEncoder(_X2);
    encodex2=0;
    digitalWrite(X1_DIR_PIN, LOW);
    digitalWrite(X2_DIR_PIN, HIGH);
    for(int x = 0; x < 500; x++) {
        digitalWrite(X1_STEP_PIN, HIGH); 
        digitalWrite(X2_STEP_PIN, HIGH);
        delayMicroseconds(1000); 
        digitalWrite(X1_STEP_PIN, LOW); 
        digitalWrite(X2_STEP_PIN, LOW);
        delayMicroseconds(1000);
    }
    X1Status = 1;
    X2Status = 1;
    while (abs(encodex1-pre_encodex1)>100 || abs(encodex2-pre_encodex2>100)) {
       
        digitalWrite(X1_STEP_PIN, HIGH); 
      
        digitalWrite(X2_STEP_PIN, HIGH); 
        delayMicroseconds(1000); 
        digitalWrite(X1_STEP_PIN, LOW); 
        digitalWrite(X2_STEP_PIN, LOW); 
        delayMicroseconds(1000);
    }
    X1Status = 0;
    X2Status = 0;
    x1max=-ReadEncoder(_X1);
    x2max=ReadEncoder(_X2);
    
}
