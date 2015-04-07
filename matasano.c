#include "matasano.h"
#include <string.h>
#include <stdlib.h>

static char val2base64char(uint8_t val);
static uint8_t hexChar2int(char c);

void hex2val(const char * hexStr, struct bigint * bi)
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

void val2hex(const struct bigint * val, char ** hexStr)
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

void val2base64str(const struct bigint * bi, char ** b64str)
{
    int l = (bi->n *8 + 5)/6;
    char * str;
    int ib,s,ob;
    *b64str = (char *)malloc(l+1);
    str = *b64str;
    str[l] = 0;

    for(ob=l-1,ib=bi->n-1,s=0; ob>=0; ob--)
    {
        uint16_t mask = 0x003F << s;
        uint16_t l = bi->bytes[ib];
        uint16_t h = (ib > 0 ? bi->bytes[ib-1] : 0);
        uint16_t v = ((h << 8) | (l & 0xFF));
        str[ob] = val2base64char((v & mask) >> s);
        s += 6;
        if(s >= 8){
            s -= 8;
            ib--;
        }
    }
    int newLen = l;
    for(ob = 0; ob < l-1 && str[ob] == 'A';ob++){
        newLen--;
    }
    if(newLen < l){ 
        char * newStr = malloc(newLen+1);
        memcpy(newStr,&str[l - newLen],newLen+1);

        free(str);
        *b64str = newStr;
    }
}

void bytesToCharStr(const struct bigint* bi, char ** str)
{
    int l = bi->n +1;
    int i;
    *str = (char*)malloc(l);
    (*str)[l-1] = 0;
    for(i = 0; i<l-1; i++){ 
        char c = '_';
        if(bi->bytes[i] >= 0x20 && bi->bytes[i] < 0x80){
            c = bi->bytes[i];
        }
        (*str)[i] = c;
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

