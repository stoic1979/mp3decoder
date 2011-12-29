#include "utils.h"

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
