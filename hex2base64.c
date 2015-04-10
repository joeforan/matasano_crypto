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
    struct bigint biHex,biB64;
    int i,same;

    biHex.bytes = NULL;
    biB64.bytes = NULL;
    hex2base64(hex,&b64);
    if(strcmp(b64,expected) != 0){
        printf("Sorry! Not correct.\n  Expected: %s\n       Got: %s\n",
               expected,b64);
        goto finish;
    }

    hex2val(hex,&biHex);
    base642val(b64,&biB64);

    same = 1;
    if(biHex.n != biB64.n){
        same = 0;
    }
    for(i=0; same && i<biHex.n; i++){
        if(biHex.bytes[i] != biB64.bytes[i]){
            same = 0;
        }
    }
    if(same){
        printf("    Hex: %s\n Base64: %s\nOk!\n",
               hex,b64);
    }else{
        char * hexconv;
        char * b64conv;
        val2hex(&biHex,&hexconv);
        val2hex(&biB64,&b64conv);
        
        printf("Conversion failed \n  hex: %s\n  b64: %s\n  Original Hex: %s\n  Expected b64: %s\n",
               hexconv,b64conv,hex,expected);
        free(hexconv);
        free(b64conv);
    }

    
finish:
    if(b64){
        free(b64);
    }   
    if(biHex.bytes){
        free(biHex.bytes);
    }
    if(biB64.bytes){
        free(biB64.bytes);
    }
}   

void hex2base64(const char * hexStr, char ** b64str)
{
    struct bigint bi;
    hex2val(hexStr,&bi);
    val2base64str(&bi,b64str);

    free(bi.bytes);
}
