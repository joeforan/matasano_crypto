#include "hammingDistance.h"
#include <stdio.h>

static uint32_t byteHammingDist(uint8_t a, uint8_t b);

void test_hammingDistance()
{
    const char * Test1 = "this is a test";
    const char * Test2 = "wokka wokka!!!";
    struct bigint a,b;
    int32_t hdist;

    str2val(Test1,&a);
    str2val(Test2,&b);
    
    hdist = hammingDistance(&a,&b);

    if(hdist == 37){
        printf("Passed Hamming Distance Test\n");
    }else{
        printf("Failed Hamming Distance Test. Expected 37. Measured: %d\n",hdist);
    }
}

uint32_t hammingDistance(const struct bigint * a, const struct bigint * b)
{
    uint32_t dist = 0;
    int i;
    for(i=0; i<a->n && i < b->n; i++){
        dist += byteHammingDist(a->bytes[i],b->bytes[i]);
    }
    for(;i<a->n; i++){
        dist += byteHammingDist(a->bytes[i],0);
    }
    for(;i<b->n; i++){
        dist += byteHammingDist(b->bytes[i],0);
    }
    
    return dist;
}

static uint32_t byteHammingDist(uint8_t a, uint8_t b)
{
    uint8_t c;
    uint32_t res;

    for(res=0,c=a^b; c>0; res += c%2,c/=2);

    return res;
}
