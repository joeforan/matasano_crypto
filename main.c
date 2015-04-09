#include "matasano.h"
#include "hex2base64.h"
#include "fixedXor.h"
#include "singleCharXor.h"
#include "detectSingleCharXor.h"
#include "repeatingKeyXor.h"

int main(int argc, char ** argv)
{
    test_hex2base64();
    test_fixedXor();
    try_singleCharXor();
    detectSingleCharXor();
    test_repeatingKeyXor();

    return 0;
}
