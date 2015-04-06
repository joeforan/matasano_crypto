#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct bigint
{
    uint8_t * bytes;
    uint32_t n;
};

static void test(const char * str1, const char * str2, const char * expected);
static void hex2val(const char * hexStr, struct bigint * bi);
static void xor(const struct bigint * a, const struct bigint * b,
                struct bigint * res);
static void val2hex(const struct bigint * val, char ** hexStr);
static uint8_t hexChar2int(char c);

int main(int argc, char ** argv)
{
    test("1c0111001f010100061a024b53535009181c",
         "686974207468652062756c6c277320657965",
         "746865206b696420646f6e277420706c6179");

    return 0;
}

static void test(const char * str1, const char * str2, const char * expected)
{
    struct bigint a;
    struct bigint b;
    struct bigint c;
    char * res = NULL;
    hex2val(str1,&a);
    hex2val(str2,&b);
    xor(&a,&b,&c);
    
    val2hex(&c,&res);
    
    if(strcmp(expected,res) != 0){
        printf("Failure Xoring\n           %s  with\n           %s\n   Result: %s\n Expected: %s\n",
               str1,str2,res,expected);
    }else{
        printf("Xored\n           %s  with\n           %s\n   Result: %s\nOK!\n",
               str1,str2,res);
    }

    free(a.bytes);
    free(b.bytes);
    free(c.bytes);
    free(res);
}

static void hex2val(const char * hexStr, struct bigint * bi)
{
    int hexLen = strlen(hexStr);
    int i,j;
    bi->n = (hexLen+1)/2;
    bi->bytes = (uint8_t*)malloc(bi->n);

    for(i=hexLen-1,j=0; i>=0; i--,j++){
        uint8_t hexVal = hexChar2int(hexStr[i]);
        int bIndex = bi->n - 1 - j/2;
        if(j%2 == 0){
            bi->bytes[bIndex] = hexVal;
        }else{
            bi->bytes[bIndex] += hexVal<<4;
        }
    }
}

static uint8_t hexChar2int(char c)
{
    if(c >= '0' && c <= '9'){
        return c-'0';
    }
    if(c >= 'A' && c <= 'F'){
        return 10 + c - 'A';
    }

    return 10 + c - 'a';
}

static void xor(const struct bigint * a, const struct bigint * b,
                struct bigint * res)
{   
    int i;
    res->n = a-> n > b->n ? a->n : b->n;

    res->bytes = (uint8_t*)malloc(res->n);

    for(i=res->n-1; i>=0; i--){
        uint8_t aval = i < a->n ? a->bytes[i] : 0;
        uint8_t bval = i < b->n ? b->bytes[i] : 0;
        
        res->bytes[i] = aval ^ bval;
    }
}
        
static void val2hex(const struct bigint * val, char ** hexStr)
{
    int hexLen = val->n * 2;
    char * str;
    int i,j;
    if(val->bytes[0] < 16){
        hexLen --;
    }
    str = *hexStr = malloc(hexLen+1);
    
    str[hexLen] = 0;
    for(i=hexLen-1,j=0; i>=0; i--,j++){
        uint8_t v;
        if(j %2 == 0){  
            v = val->bytes[i/2] & 0x0F;
        }else{
            v = (val->bytes[i/2] & 0xF0) >> 4;
        }
        if(v < 10){
            str[i] = '0' + v;
        }else{
            str[i] = 'a' + (v-10);
        }
    }
}
    

