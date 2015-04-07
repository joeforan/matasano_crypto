#ifndef _MATASANO_H_
#define _MATASANO_H_

#include <stdint.h>

struct bigint
{
    uint8_t * bytes;
    uint32_t n;
};

void hex2val(const char * hexStr, struct bigint * bi);
void val2hex(const struct bigint * val, char ** hexStr);
void val2base64str(const struct bigint * bi, char ** b64str);

#endif
