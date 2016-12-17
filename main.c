#include "matasano.h"
#include "hex2base64.h"
#include "fixedXor.h"
#include "singleCharXor.h"
#include "detectSingleCharXor.h"
#include "repeatingKeyXor.h"
#include "hammingDistance.h"
#include "challenge6.h"
#include "challenge7.h"

int main(int argc, char ** argv)
{
#if 0
    test_hex2base64();
    test_fixedXor();
    try_singleCharXor();
    detectSingleCharXor();
    test_repeatingKeyXor();
    test_hammingDistance();
    try_challenge6();
#else
	try_challenge7();
#endif
    
    return 0;
}
