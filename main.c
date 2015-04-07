#include "matasano.h"
#include "hex2base64.h"
#include "fixedXor.h"

int main(int argc, char ** argv)
{
    test_hex2base64();
    test_fixedXor();
    return 0;
}
