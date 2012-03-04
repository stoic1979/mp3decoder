/*
 * Copyright (c) 2011 onwards by WeaveBytes InfoTech Pvt. Ltd.
 * 
 * Please reports bugs at weavebytes@gmail.com
 * 
 * This file may be distributed and/or modified under the terms of the 
 * GNU General Public License version 2 as published by the Free Software 
 * Foundation. (See COPYING.GPL for details.)
 * 
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 * 
 */
#include "app_utils.h"

int read_16_bits_low_high(FILE * fp) {
    unsigned char bytes[2] = { 0, 0 };
    fread(bytes, 1, 2, fp);
    {
        int32_t const low = bytes[0];
        int32_t const high = (signed char) (bytes[1]);
        return (high << 8) | low;
    }
}

int read_32_bits_low_high(FILE * fp) {
    unsigned char bytes[4] = { 0, 0, 0, 0 };
    fread(bytes, 1, 4, fp);
    {
        int32_t const low = bytes[0];
        int32_t const medl = bytes[1];
        int32_t const medh = bytes[2];
        int32_t const high = (signed char) (bytes[3]);
        return (high << 24) | (medh << 16) | (medl << 8) | low;
    }
}

int read_32_bits_high_low(FILE * fp) {
    unsigned char bytes[4] = { 0, 0, 0, 0 };
    fread(bytes, 1, 4, fp);
    {
        int32_t const low = bytes[3];
        int32_t const medl = bytes[2];
        int32_t const medh = bytes[1];
        int32_t const high = (signed char) (bytes[0]);
        return (high << 24) | (medh << 16) | (medl << 8) | low;
    }
}

int read_16_bits_high_low(FILE * fp) {
    unsigned char bytes[2] = { 0, 0 };
    fread(bytes, 1, 2, fp);
    {
        int32_t const low = bytes[1];
        int32_t const high = (signed char) (bytes[0]);
        return (high << 8) | low;
    }
}

void write_16_bits_low_high(FILE * fp, int val) {
    unsigned char bytes[2];
    bytes[0] = (val & 0xff);
    bytes[1] = ((val >> 8) & 0xff);
    fwrite(bytes, 1, 2, fp);
}

void write_32_bits_low_high(FILE * fp, int val) {
    unsigned char bytes[4];
    bytes[0] = (val & 0xff);
    bytes[1] = ((val >> 8) & 0xff);
    bytes[2] = ((val >> 16) & 0xff);
    bytes[3] = ((val >> 24) & 0xff);
    fwrite(bytes, 1, 4, fp);
}

unsigned int uint32_high_low(unsigned char *bytes) {
    uint32_t const hh = bytes[0];
    uint32_t const hl = bytes[1];
    uint32_t const lh = bytes[2];
    uint32_t const ll = bytes[3];
    return (hh << 24) | (hl << 16) | (lh << 8) | ll;
}

long make_even_number_of_bytes_in_length(long x) {
    if ((x & 0x01) != 0) {
        return x + 1;
    }
    return x;
}
