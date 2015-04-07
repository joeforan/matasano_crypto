#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "matasano.h"
#include "hex2base64.h"

#define TEST_HEX_STR "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d"
#define TEST_B64_STR "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t"

static void test(const char * hex, const char * expected);

void test_hex2base64()
{
    test(TEST_HEX_STR,TEST_B64_STR);
    test("0","A");
    test("1","B");
    test("FF","D/");
    test("380ABCDEF","OAq83v");
}

static void test(const char * hex, const char * expected)
{
    char * b64 = NULL;

    hex2base64(hex,&b64);
    if(strcmp(b64,expected) != 0){
        printf("Sorry! Not correct.\n  Expected: %s\n       Got: %s\n",
               expected,b64);
    }else{
        printf("    Hex: %s\n Base64: %s\nOk!\n",
               hex,b64);
    }

    if(b64){
        free(b64);
    }
}

void hex2base64(const char * hexStr, char ** b64str)
{
    struct bigint bi;
    hex2val(hexStr,&bi);
    val2base64str(&bi,b64str);

    free(bi.bytes);
}
