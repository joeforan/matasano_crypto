#include "challenge6.h"
#include "matasano.h"
#include "hammingDistance.h"
#include "detectSingleCharXor.h"
#include "repeatingKeyXor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>

#define FILENAME "challenge6.txt"

//#define DEBUG_CHALLENGE_6

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

    read_int(FILENAME,&bi);
    printf("In challenge6. Bi Size: %d\n",bi.n);

    int maxScore = 0;
    struct bigint bestKey = {0};
    for(keysize = 2; keysize <= 50; keysize++){
        int i,j;
        double keyScore = 0;
        struct bigint code;
        code.n = keysize;   
        code.bytes = malloc(keysize);
#ifdef DEBUG_CHALLENGE_6
		printf("KeySize: %d\n",keysize);
#endif
        for(i=0; i<keysize; i++){
            int sz = (bi.n/keysize);
            struct bigint local;
            int thisScore = 0;
            if(i+keysize*sz < bi.n){
                sz++;
            }
            local.n = sz;
            local.bytes = malloc(sz);

            for(j=0; j<sz; j++){    
                local.bytes[j] = bi.bytes[i+j*keysize];
            }
            
            code.bytes[i] = getBestXorScore(&local,&thisScore);
#ifdef DEBUG_CHALLENGE_6
			printf(" [%02d]: Code: 0x%02x(%c), Score: %d, sz: %d\n",i,code.bytes[i],((code.bytes[i] >= 0x20 && code.bytes[i] < 0x80)?code.bytes[i]:'_'),thisScore,sz);
#endif
            free(local.bytes);
            keyScore += thisScore;
        }   
#ifdef DEBUG_CHALLENGE_6
		printf("KeyScore: %f, Normalised: %f\n",keyScore,keyScore/keysize);
#endif
        keyScore /= keysize;
        if(keyScore > maxScore){
#ifdef DEBUG_CHALLENGE_6
			printf("New Max Score!");
#endif
            maxScore = keyScore;
            if(bestKey.bytes){
                free(bestKey.bytes);
            }
            bestKey.n = keysize;    
            bestKey.bytes = malloc(keysize);
            memcpy(bestKey.bytes,code.bytes,keysize);
        }
        free(code.bytes);
    }
    applyRepeatingKeyXor(&bi,&bestKey,&decoded);
    bytesToCharStr(&decoded,&decodedStr);
	printf("BestKey Size: %d\n",bestKey.n);
	
    printf("Decoded: \n%s\n",decodedStr);

#ifdef DEBUG_CHALLENGE_6
	{
		char * rev;
		val2base64str(&bi,&rev);
		printf("Sanity check:\n%s\n",rev);
		free(rev);
	}
#endif
    
    free(bi.bytes);
}
