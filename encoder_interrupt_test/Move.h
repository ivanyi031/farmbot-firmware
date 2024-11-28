#ifndef MOVE_H
#define MOVE_H


extern long ymax,ymin;
extern long zmax,zmin;
extern long x1max, x1min;
extern long x2max, x2min;
extern int pre_z;

void Movement(int x, int y, int z, int esp_pos, int t);
void adjustValue(long *var, long max_var);

#endif
