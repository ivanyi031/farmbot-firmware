#include "SpiEncoder.h"
#include "Move.h"

void Movement(int x, int y, int z, int esp_pos, int t)
{
  int ystep=1 ;
  int x1step=1 ;
  int x2step=1 ;
  int zstep=1 ;
  long Yen = ReadEncoder(_Y);
  long X1en = -ReadEncoder(_X1);
  long X2en = ReadEncoder(_X2);
  long Zen = ReadEncoder(_Z);
  adjustValue(&Yen, ymax);
  adjustValue(&X1en, x1max);
  adjustValue(&X2en, x2max);
  adjustValue(&Zen, zmax);
  int Ypos = map(Yen, 0, ymax, 0, 1265);
  int X1pos = map(X1en, 0, x1max, 0, 1246);
  int X2pos = map(X2en, 0, x2max, 0, 1245);
  int Zpos = map(Zen, 0, zmax, 0, 580);

  unsigned long timerXY;
  unsigned long timerZ;

  int target_ystep = abs(Ypos - y) * 10;
  int target_x1step = abs(X1pos - x) * 10;
  int target_x2step = abs(X2pos - x) * 10;
  int target_zstep = abs(Zpos - (580-z)) * 25 * 2;

  if (Ypos > y) {
    digitalWrite(Y_DIR_PIN, LOW);
  }
  else {
    digitalWrite(Y_DIR_PIN, HIGH);
  }
  if (X2pos > x) {

    digitalWrite(X1_DIR_PIN, HIGH);
    digitalWrite(X2_DIR_PIN, LOW);
  }
  else {
    digitalWrite(X1_DIR_PIN, LOW);
    digitalWrite(X2_DIR_PIN, HIGH);
  }
  if (pre_z > z) {
    digitalWrite(Z_DIR_PIN, HIGH);
  }
  else {
    digitalWrite(Z_DIR_PIN, LOW);
  }
  if(pre_z==z && t!=0){
    zstep=target_zstep;
  }  
  digitalWrite(Y_STEP_PIN, HIGH);
  digitalWrite(X1_STEP_PIN, HIGH);
  digitalWrite(X2_STEP_PIN, HIGH);
  digitalWrite(X2_STEP_PIN, HIGH);
  timerXY = micros();
  timerZ = micros();
  if(esp_pos ==4){
    while(zstep < target_zstep){
    unsigned long currentTime = micros();
    if (currentTime - timerZ >= 750)
    {
      if (zstep < target_zstep)
      {
        digitalWrite(Z_STEP_PIN, !digitalRead(Z_STEP_PIN));
        zstep++;
      }
      timerZ = currentTime;
    }
    }
  }


  
  while (ystep < target_ystep || x1step < target_x1step || x2step < target_x2step || zstep < target_zstep)
  {
    unsigned long currentTime = micros();
    if (currentTime - timerXY >= 1000)
    {
      if (ystep < target_ystep)
      {
        digitalWrite(Y_STEP_PIN, !digitalRead(Y_STEP_PIN));
        ystep++;
      }
      if (x1step < target_x1step)
      {
        digitalWrite(X1_STEP_PIN, !digitalRead(X1_STEP_PIN));
        x1step++;
        //Serial.println(x1step);
      }
      if (x2step < target_x2step)
      {
        digitalWrite(X2_STEP_PIN, !digitalRead(X2_STEP_PIN));
        x2step++;
        //Serial.println(x2step);
      }
      timerXY = currentTime;
    }
    if (currentTime - timerZ >= 750)
    {
      if (zstep < target_zstep)
      {
        digitalWrite(Z_STEP_PIN, !digitalRead(Z_STEP_PIN));
        zstep++;
      }
      timerZ = currentTime;
    }
  
  }
  int ypos = map(ReadEncoder(_Y), 0, ymax, 0, 1265);
  int x1pos = map(ReadEncoder(_X1), 0, x1max, 0, 1246);
  int x2pos = map(ReadEncoder(_X2), 0, x2max, 0, 1245);
  int zpos = map(ReadEncoder(_Z), 0, zmax, 0, 580);
  /*
  adjustValue(&Yen, ymax);
  adjustValue(&X1en, x1max);
  adjustValue(&X2en, x2max);
  adjustValue(&Zen, zmax);*/
  String pos ="("+String(ypos)+","+String(x1pos)+","+String(x2pos)+","+String(z)+")";
  Serial.println(pos);
  target_ystep = 0 ;
  target_x1step = 0 ;
  target_x2step = 0;
  target_zstep = 0 ;
  
//  if(abs(ypos-y)>5 || abs(x1pos-x)>5 ||abs(x2pos-x)>5 || abs(zpos-z)>5)
//     Movement(x, y, z, esp_pos, t);
//  else{
//
//    ystep = 1;
//    x1step = 1;
//    x2step = 1;
//    zstep = 1;
//    pre_z=z;
//  } 
   pre_z=z;
}
void adjustValue(long *var, long max_var) {
  if (*var > max_var) {
    *var = max_var;  // Set to constant if greater
  } else if (*var < 0) {
    *var = 0;          // Set to 0 if negative
  }
}
