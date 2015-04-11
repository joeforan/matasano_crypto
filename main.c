#include "matasano.h"
#include "hex2base64.h"
#include "fixedXor.h"
#include "singleCharXor.h"
#include "detectSingleCharXor.h"
#include "repeatingKeyXor.h"
#include "hammingDistance.h"
#include "challenge6.h"

int main(int argc, char ** argv)
{
    test_hex2base64();
    test_fixedXor();
    try_singleCharXor();
    detectSingleCharXor();
    test_repeatingKeyXor();
    test_hammingDistance();
    try_challenge6();
    
    return 0;
}
