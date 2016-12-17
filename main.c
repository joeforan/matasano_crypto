#include "matasano.h"
#include "hex2base64.h"
#include "fixedXor.h"
#include "singleCharXor.h"
#include "detectSingleCharXor.h"
#include "repeatingKeyXor.h"
#include "hammingDistance.h"
#include "openssl_wrapper.h"
#include "challenge6.h"
#include "challenge7.h"
#include "challenge8.h"

int main(int argc, char ** argv)
{
	openssl_init();

#if 0
    test_hex2base64();
    test_fixedXor();
    try_singleCharXor();
    detectSingleCharXor();
    test_repeatingKeyXor();
    test_hammingDistance();
    try_challenge6();
	try_challenge7();
#else
	try_challenge8();
#endif

	openssl_cleanup();
    return 0;
}
