#include "detectSingleCharXor.h"
#include "matasano.h"
#include "singleCharXor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "challenge4_words.txt"

void detectSingleCharXor()
{
    FILE * f = fopen(FILENAME,"r");
    char * line = NULL;
    size_t n = 0;
    int maxScore = 0;
    char * bestLine=NULL;
    uint8_t bestCode;
    ssize_t l;
    int i;

    if(f == NULL){
        fprintf(stderr,"Could not open %s for reading\n",FILENAME);
        return;
    }

    while((l = getline(&line,&n,f)) > 0){   
        int score;  
        struct bigint bi;
        int thisBestCode;

        if(line[l-1] == '\n'){
            l--;
            line[l] = '\0'; 
        }
        hex2val(line,&bi);

        thisBestCode = getBestXorScore(&bi,&score);
        if(score > maxScore){
            maxScore = score;
            bestLine = realloc(bestLine,n);
            memcpy(bestLine,line,n);
            bestCode = thisBestCode;
        }
            
        free(bi.bytes);
        bi.bytes = NULL;
        bi.n = 0;
    }

    if(bestCode > 0){
        struct bigint bi;
        char * decryptedStr = NULL;
        hex2val(bestLine,&bi);
        for(i=0; i<bi.n; i++){
            bi.bytes[i] = bi.bytes[i]^bestCode;
        }
        bytesToCharStr(&bi,&decryptedStr);
    
        printf("Best key: 0x%x,(%c). String: %s\n",
               bestCode, (bestCode >= 0x20 && bestCode < 0x80 ? (char)bestCode : '?'), 
               decryptedStr);
        free(bi.bytes);
        bi.bytes = NULL;
        bi.n = 0;
        free(decryptedStr);
    }
}

int getBestXorScore(const struct bigint * bi, int * score)
{
    int maxscore = 0;
    int bestCode = -1;
    int i;
    
    for(i=0; i<256; i++){
        int thisScore = singleCharXor(bi,(uint8_t)i);
        if(thisScore > maxscore){
            maxscore = thisScore;   
            bestCode = i;
        }
    }
    *score = maxscore;
    return bestCode;
    
}

    
