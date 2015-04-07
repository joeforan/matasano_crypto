#include "matasano.h"
#include "hex2base64.h"
#include "fixedXor.h"
#include "challenge3.h"

int main(int argc, char ** argv)
{
    test_hex2base64();
    test_fixedXor();
    doChallenge3();
    
    return 0;
}
