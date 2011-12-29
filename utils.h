#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int read_16_bits_low_high(FILE * fp);
int read_32_bits_low_high(FILE * fp);
int read_32_bits_high_low(FILE * fp);
int read_16_bits_high_low(FILE * fp);

void write_16_bits_low_high(FILE * fp, int val);
void write_32_bits_low_high(FILE * fp, int val);

#endif//__UTILS_H__
