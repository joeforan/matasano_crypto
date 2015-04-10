#include "singleCharXor.h"
#include "matasano.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define ENCRYPTED_MSG "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736"
//#define DEBUG_CHG3

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

void try_singleCharXor()
{
    struct bigint enc,dec;
    int maxScore = 0;
    uint8_t bestKey = 0;
    char * decryptedStr = NULL;
    int i;

    hex2val(ENCRYPTED_MSG,&enc);

    for(i=0; i<256; i++){   
        int score = singleCharXor(&enc,(uint8_t)i);
        
        if(score > maxScore){
            maxScore = score;
            bestKey = i;
        }
    }

    dec.n = enc.n;
    dec.bytes = (uint8_t*)malloc(dec.n);
    for(i=0; i<enc.n; i++){
        dec.bytes[i] = bestKey ^ enc.bytes[i];
    }

    bytesToCharStr(&dec,&decryptedStr);
    
    printf("Best key: 0x%x,(%c). String: %s\n",
           bestKey, (bestKey >= 0x20 && bestKey < 0x80 ? (char)bestKey : '?'), 
           decryptedStr);

    free(decryptedStr);

    free(enc.bytes);
    free(dec.bytes);
}

int singleCharXor(const struct bigint * bytes, uint8_t x)
{
    int count[27] = {0};
    int score = 0;
    int i;
#ifdef DEBUG_CHG3
    char * debugStr = NULL;
    int other = 0;
    struct bigint dec;  
    dec.n = bytes->n;
    dec.bytes = malloc(dec.n);
#endif
    for(i=0; i<bytes->n; i++){
        uint8_t c = x ^ bytes->bytes[i];
#ifdef DEBUG_CHG3
        dec.bytes[i] = c;
#endif
        if(c >= 'a' && c <= 'z'){
            count[c-'a']++;
        }else if(c >= 'A' && c<= 'Z'){
            count[c-'A']++;
        }else if(c == ' '){
            count[26]++;
#ifdef DEBUG_CHG3
        }else{
            other++;
#endif
        }
    }

    for(i=0; i<27; i++){
        score += count[i]*LetterFreq[i];
    }
#ifdef DEBUG_CHG3
    bytesToCharStr(&dec,&debugStr);
    printf("key: 0x%x,(%c). String: %s. Score: %d. Other: %d\n",
           x, (x >= 0x20 && x < 0x80 ? (char)x : '?'), 
           debugStr,score,other);
    free(dec.bytes);
#endif
    return score;
}
