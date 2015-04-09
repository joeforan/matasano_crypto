#include "repeatingKeyXor.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TEST_STRING "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal"
#define TEST_KEY "ICE"
//                 I C E I C E I C E I C E I C E I C E I C E I C E I C E I C E I C E I C E I C E I C E I C E I C E I C E I C E I C E I C E I C E I C E I C E I C E I C E 
//                 B u r n i n g   ' e m ,   i f   y o u   a i n ' t   q u i c k   a n d   n i m b l e\n I   g o   c r a z y   w h e n   I   h e a r   a   c y m b a l 
#define TEST_ENC "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f"


void test_repeatingKeyXor()
{
    struct bigint key;
    struct bigint str;
    struct bigint enc;
    char * encryptedStr = NULL;

    str2val(TEST_STRING,&str);
    str2val(TEST_KEY,&key);

    applyRepeatingKeyXor(&str,&key,&enc);

    val2hex(&enc,&encryptedStr);

    if(strcmp(TEST_ENC,encryptedStr) == 0){
        printf("Passed Repeated Xor Challenge\n");
    }else{
        printf("Failure In Repeated Xor Challenge.\nExpected:\n%s\nGot:\n%s\n",
               TEST_ENC,encryptedStr);
    }
    free(key.bytes);
    free(str.bytes);
    free(enc.bytes);
    free(encryptedStr);
}

void applyRepeatingKeyXor(const struct bigint * str, const struct bigint * key, struct bigint * encrypted)
{
    int key_len = key->n;
    int i,k;
    encrypted->n = str->n;
    encrypted->bytes = (uint8_t*)malloc(encrypted->n);
    
    for(i=0,k=0; i<str->n; i++,k=(k+1)%key_len){
        encrypted->bytes[i] = key->bytes[k] ^ str->bytes[i];
    }
}
    
    

