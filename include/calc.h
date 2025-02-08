#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "string.h"
#include "read_input.h"
#include "drivers.h"

int calculator(int argc, char* argv[]);
float str_to_float(const char* str);
void float_to_str(float num, char* str);


#endif