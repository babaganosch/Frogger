//
//  util.h
//  Frogger
//
//  Created by Lars Andersson on 2020-02-25.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef util_h
#define util_h

#include <math.h>
#include <stdlib.h>

/* Clamp: Helper function to clamp values into an interval */
double clamp(double x, double lower, double upper);

/* PercentChance: Returns true or false, based on a percentage */
bool percentChance(float percent);

/* Random: Returns a value between 0 and the parameter */
float random(float value);
int  irandom(int value);

#endif /* util_h */
