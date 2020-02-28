//
//  util.cpp
//  Frogger
//
//  Created by Lars Andersson on 2020-02-25.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//
#include <math.h>
#include <stdlib.h>
#include "util.h"

double clamp(double x, double lower, double upper)
{
    return fmin(fmax(x, lower), upper);
}

bool percentChance(int percent) {
    return (rand() % 100) < percent;
}

float random(float value) {
    return (rand() / (float)RAND_MAX) * value;
}

int irandom(int value) {
    return rand() % value;
}
