#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define TEST_HEX_STR "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d"
#define TEST_B64_STR "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t"

struct bigint
{
    uint8_t * bytes;
    uint32_t n;
};

static void hex2base64(const char * hexStr, char ** b64);
static void hex2val(const char * hexStr, struct bigint * bi);
static void val2base64str(const struct bigint * bi, char ** b64str);
static uint8_t hexChar2int(char c);

int main(int argc, char ** argv)
{
    char * b64 = NULL;

    hex2base64(TEST_HEX_STR,&b64);
    if(strcmp(b64,TEST_B64_STR) != 0){
        printf("Sorry! Not correct.\n  Expected: %s\n       Got: %s\n",
               TEST_B64_STR,b64);
    }else{
        printf("Ok!");
    }

    if(b64){
        free(b64);
    }

    return 0;
}


static void hex2base64(const char * hexStr, char ** b64str)
{
    struct bigint bi;
    hex2val(hexStr,&bi);
    val2base64str(&bi,b64str);
 
    free(bi.bytes);
}

static void hex2val(const char * hexStr, struct bigint * bi)
{
    int hexLen = strlen(hexStr);
    int i,j;
    bi->n = (hexLen+1)/2;
    bi->bytes = (uint8_t*)malloc(bi->n);

    printf("In hex2Val. Hexlen: %d, bi->n: %d\n",
           hexLen,bi->n);

    for(i=hexLen-1,j=0; i>=0; i--,j++){
        uint8_t hexVal = hexChar2int(hexStr[i]);
        int bIndex = bi->n - 1 - j/2;
        if(j%2 == 0){
            bi->bytes[bIndex] = hexVal;
        }else{
            bi->bytes[bIndex] += hexVal<<4;
        }
        printf("i: %d, char: %c, hexVal: 0x%x, bIndex: %d, byteVal: 0x%x\n",
               i,hexStr[i],hexVal,bIndex,bi->bytes[bIndex]);
    }
}

static char val2base64char(uint8_t val)
{
    if(val < 26){
        return 'A' + val;
    }else if(val < 52){
        return 'a' + (val-26);
    }else if(val < 62){
        return '0' + (val-52);
    }else if(val == 62){
        return '+';
    }
    return '/';
}

static void val2base64str(const struct bigint * bi, char ** b64str)
{
    int l = (bi->n *8 + 5)/6;
    char * str;
    int ib,s,ob;
    *b64str = (char *)malloc(l+1);
    str = *b64str;
    str[l] = 0;

    printf("In val2base64str. bi->n: %d, outLen: %d\n",
           bi->n,l);

    for(ob=l-1,ib=bi->n-1,s=0; ob>=0; ob--)
    {
        uint16_t mask = 0x003F << s;
        uint16_t l = bi->bytes[ib];
        uint16_t h = (ib > 0 ? bi->bytes[ib-1] : 0);
        uint16_t v = ((h << 8) | (l & 0xFF));
        str[ob] = val2base64char((v & mask) >> s);
        printf("ib: %d, s: %d, mask: 0x%02x, hl: 0x%02x%02x, v: 0x%04x, str[ob]: %c\n",
               ib,s,mask,h,l,v,str[ob]);
        s += 6;
        if(s >= 8){
            s -= 8;
            ib--;
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

    
    
