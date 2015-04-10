#include "challenge6.h"
#include "matasano.h"
#include "hammingDistance.h"
#include "detectSingleCharXor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>

#define FILENAME "challenge6.txt"

#define DEBUG_CHALLENGE_6

static int read_int(struct bigint * bi)
{
    FILE * f = fopen(FILENAME,"r");

    char * fullString = NULL;
    ssize_t bufSize = 0;
    char * line = NULL;
    ssize_t l;
    size_t n = 0;

    int ret = -1;

    if(f == NULL){
        fprintf(stderr,"Could not open %s for reading\n",FILENAME);
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
        free(line);
        line = NULL;
        n = 0;
    }
    
#ifdef DEBUG_CHALLENGE_6
    printf("Read string from file:\n%s\nLength: %d\n",fullString,(int)bufSize);
#endif

    base642val(fullString,bi);

#ifdef DEBUG_CHALLENGE_6
    printf("Converted to bigInt. Len: %d\n",bi->n);
    {
        char * rev, *p, *q;
        val2base64str(bi,&rev);
        for(p=rev,q=fullString; *p &&  *p == *q; p++,q++);
        if(*q && isB64char(*q)){
            printf(" Error converting big int back. Rev:\n%s\n",
                   rev);
            free(rev);
            goto out;
        }else{
            printf("Reverse conversion OK\n");

        }
        free(rev);
    }
#endif
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

int deduce_key_size(const struct bigint * bi)
{
    int max_len = 2*sqrt(bi->n);
    int k;
    double minScore=DBL_MAX;
    int minK = -1;
    
    for(k=2; k<=max_len; k++){
        int noStrs = bi->n/k;
        uint32_t total = 0;
        int c = 0;
        struct bigint a,b;
        int i,j;
        double score;
        a.n = b.n = k;
        for(i=0; i<noStrs; i++){
            a.bytes = &bi->bytes[i*k];
            for(j=i+1; j<noStrs; j++,c++){
                b.bytes = &bi->bytes[j*k];
                total += hammingDistance(&a,&b);
            }
        }   
        score = (total/(double)(c*k));
#ifdef DEBUG_CHALLENGE_6
        printf("K: %d, Total: %d, C: %d, Score: %f\n",k,total,c,score);
#endif
        if(score < minScore){
            minScore = score;
            minK = k;
        }
    }

    return minK;
}   

void try_challenge6()
{
    struct bigint bi;
    struct bigint decoded = {0};
    char * decodedStr;
    int keysize;

    read_int(&bi);
    printf("Bi Size: %d\n",bi.n);

    keysize = deduce_key_size(&bi);
    printf("Min Keysize: %d\n",keysize);

    if(keysize > 0){
        int sz = bi.n/keysize;
        int i,j;
        printf("Sz: %d\n",sz);
        fflush(stdout);
        decoded.n = bi.n;
        decoded.bytes = malloc(bi.n);
        for(i=0; i<keysize; i++){
            struct bigint local;
            int maxScore = 0;
            uint8_t c;
            local.n = sz;
            local.bytes = malloc(sz);
#ifdef DEBUG_CHALLENGE_6
            printf("Allocated %d bytes @ 0x%08lx\n",sz,(unsigned long)local.bytes);
            fflush(stdout);
#endif

            for(j=0; j<sz; j++){    
                local.bytes[j] = bi.bytes[i+j*keysize];
            }
#ifdef DEBUG_CHALLENGE_6
            printf("Calling getBestScore[%d]\n",i);
            fflush(stdout);
#endif
            
            c = getBestXorScore(&local,&maxScore);
#ifdef DEBUG_CHALLENGE_6
            printf("Code: 0x%02x\n",c);
            fflush(stdout);
#endif
            for(j=i; j<bi.n; j+=keysize){
                decoded.bytes[j] = c ^ bi.bytes[j];
            }
#ifdef DEBUG_CHALLENGE_6
            printf("Freeing local bytes: 0x%lx, j: %d\n",(unsigned long)local.bytes,j);
            fflush(stdout);
#endif
            free(local.bytes);

#ifdef DEBUG_CHALLENGE_6
            printf("Freed local bytes\n");
            fflush(stdout);
#endif
        }
        bytesToCharStr(&decoded,&decodedStr);
        printf("Decoded: \n%s\n",decodedStr);
#ifdef DEBUG_CHALLENGE_6
        fflush(stdout);
#endif
        
        free(decoded.bytes);
        free(decodedStr);
    }
    free(bi.bytes);
}
