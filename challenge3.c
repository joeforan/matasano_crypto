#include "challenge3.h"
#include "matasano.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define ENCRYPTED_MSG "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736"

//Taken from https://en.wikipedia.org/wiki/Letter_frequency#Relative_frequencies_of_letters_in_the_English_language
static const int LetterFreq[27] = { 
    8167,   // a
    1492,   // b
    2782,   // c
    4253,   // d
    12702,  // e
    2228,   // f
    2015,   // g
    6094,   // h
    6966,   // i
    153,    // j
    772,    // k
    4025,   // l
    2406,   // m
    6749,   // n       
    7507,   // o
    1929,   // p
    95,     // q
    5987,   // r
    6327,   // s
    9056,   // t    
    2758,   // u
    978,    // v
    2360,   // w
    150,    // x
    1974,   // y
    74,     // z
    12800   // SPC
};
    
struct charFreq{
    uint32_t freq;
    uint8_t c;
};

void doChallenge3()
{
    struct bigint enc,dec;
    int maxScore = 0;
    uint8_t bestKey = 0;
    char * decryptedStr = NULL;
    int i,j;

    hex2val(ENCRYPTED_MSG,&enc);

    dec.n = enc.n;
    dec.bytes = (uint8_t*)malloc(dec.n);
    for(i=0; i<256; i++){   
        uint8_t x = i;  
        int count[27] = {0};
        int score = 0;
#ifdef DEBUG_CHG3
        char * debugStr = NULL;
#endif
        

        for(j=0; j<enc.n; j++){
            uint8_t c = x ^ enc.bytes[j];
            dec.bytes[j] = c;
            if(c >= 'a' && c <= 'z'){
                count[c-'a']++;
            }else if(c >= 'A' && c<= 'Z'){
                count[c-'A']++;
            }else if(c == ' '){
                count[26]++;
            }

        }
        for(j=0; j<27; j++){
            score += count[j]*LetterFreq[j];
        }
        if(score > maxScore){
            maxScore = score;
            bestKey = x;
        }
#ifdef DEBUG_CHG3
        bytesToCharStr(&dec,&debugStr);
        printf("key: 0x%x,(%c). String: %s. Score: %d\n",
               x, (x >= 0x20 && x < 0x80 ? (char)x : '?'), 
               debugStr,score);
#endif
    }

    for(j=0; j<enc.n; j++){
        dec.bytes[j] = bestKey ^ enc.bytes[j];
    }

    bytesToCharStr(&dec,&decryptedStr);
    
    printf("Best key: 0x%x,(%c). String: %s\n",
           bestKey, (bestKey >= 0x20 && bestKey < 0x80 ? (char)bestKey : '?'), 
           decryptedStr);

    free(decryptedStr);

    free(enc.bytes);
    free(dec.bytes);
}

