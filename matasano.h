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
void bytesToCharStr(const struct bigint* bi, char ** str);
void str2val(const char * str, struct bigint * val);
void base642val(const char * str, struct bigint * val);
int isB64char(uint8_t c);
int read_int(const char * filename, struct bigint * bi);

#endif // _MATASANO_H_
