#ifndef CALIBRATION_H
#define CALIBRATION_H

#include "SpiEncoder.h"
#include "pin.h"

extern int ylimit;
extern long ymax,ymin;
extern long pre_encodey,encodey;
extern bool YStatus;

extern int zlimit;
extern long zmax,zmin;
extern long pre_encodez,encodez;
extern bool ZStatus;

extern int x1limit;
extern long x1max, x1min;
extern long pre_encodex1, encodex1;
extern bool X1Status;

extern int x2limit;
extern long x2max, x2min;
extern long pre_encodex2, encodex2;
extern bool X2Status;

void calibrateY();
void calibrateZ();
void calibrateX();




#endif
