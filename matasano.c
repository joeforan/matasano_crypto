#include "matasano.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
    str = *hexStr = malloc(hexLen+1);
    
    str[hexLen] = 0;
    for(i=0,j=0; i<hexLen; i++,j++){
        uint8_t v;
        if(j %2 == 0){  
            v = (val->bytes[i/2] & 0xF0) >> 4;
        }else{
            v = val->bytes[i/2] & 0x0F;
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
	memset(str,0,l+1);

    for(ob=0,ib=0,s=0; ob<l; ob++)
    {
        uint16_t mask = 0xFC00 >> s;
        uint16_t hb = bi->bytes[ib];
        uint16_t lb = (ib < (bi->n)-1 ? bi->bytes[ib+1] : 0);
        uint16_t v = ((hb << 8) | (lb & 0xFF));
        str[ob] = val2base64char((v & mask) >> (10-s));
        s += 6;
        if(s >= 8){
            s -= 8;
            ib++;
        }
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
        if((bi->bytes[i] >= 0x20 && bi->bytes[i] < 0x80) || bi->bytes[i] == '\n'){
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

void str2val(const char * str, struct bigint * bi)
{
    int i;
    bi->n = strlen(str);
    bi->bytes = malloc(bi->n);

    for(i=0; *str; i++,str++){
        bi->bytes[i] = *str;
    }
}

static uint8_t base64char2byte(uint8_t b)
{
    if(b >= 'A' && b <= 'Z'){
        return b-'A';
    }
    if(b >= 'a' && b <= 'z'){
        return 26+(b-'a');
    }
    if(b >= '0' && b <= '9'){
        return 52+(b-'0');
    }
    if(b == '+'){
        return 62;
    }
    if(b == '/'){
        return 63;
    }
    return 0xFF;
}



void base642val(const char * str, struct bigint * val)
{
    int i,j,s,n;
    const char * end;
    for(end = str,n=0; *end && isB64char(*end); ++end,n++);
        
    val->n = (n*6)/8;
    val->bytes = malloc(val->n);
    memset(val->bytes,0,val->n);

    for(i=0,j=0,s=0;i<n;i++){
        uint16_t mask = 0xFC00 >> s;
        uint8_t lm = mask & 0x00FF;
        uint8_t hm = (mask & 0xFF00)>>8;
        uint16_t v = base64char2byte(str[i]);
        uint16_t sv = ((uint16_t)v) << (10-s);

        val->bytes[j] = (val->bytes[j] & ~hm) | ((sv>>8) & hm);    
        
        if(s >= 2){
            j++;
            if(j < val->n){
                val->bytes[j] = (sv & lm);
            }
        }
        s = (s + 6)%8;
    }
        
}

int isB64char(uint8_t c)
{
    return (c >= 'A' && c <= 'Z') ||
        (c >= 'a' && c <= 'z') ||
        (c >= '0' && c<= '9') ||
        c == '+' || c == '/';
}

int read_int(const char * filename, struct bigint * bi)
{
    FILE * f = fopen(filename,"r");

    char * fullString = NULL;
    ssize_t bufSize = 0;
    char * line = NULL;
    ssize_t l;
    size_t n = 0;

    int ret = -1;

    if(f == NULL){
        fprintf(stderr,"Could not open %s for reading\n",filename);
        goto out;
    }

    while((l = getline(&line,&n,f)) > 0){   
        char * newString;
        int newSize;
        if(line[l-1] == '\n'){
            l--;
            line[l] = '\0';
        }   
        newSize = bufSize + l + (bufSize ? 0 : 1);
        newString = realloc(fullString,newSize);
        if(newString == NULL){
            fprintf(stderr,"Failure allocating memory\n");
            free(fullString);
            free(line);
            goto out;
        }
        memcpy(&newString[(bufSize ? bufSize-1: 0)],line,l);
        newString[newSize-1] = '\0';
        fullString = newString;
        bufSize = newSize;
    }
    free(line);
    line = NULL;
    n = 0;
    
    base642val(fullString,bi);

    ret = 0;

out:
    if(f){
        fclose(f);
    }
    if(fullString){
        free(fullString);
    }
    return ret;
}

