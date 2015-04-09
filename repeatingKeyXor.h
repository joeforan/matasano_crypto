#ifndef _REPEATING_KEY_XOR_H_
#define _REPEATING_KEY_XOR_H_

#include "matasano.h"

void test_repeatingKeyXor();
void applyRepeatingKeyXor(const struct bigint * bytes, const struct bigint * key, struct bigint * encrypted);

#endif // _REPEATING_KEY_XOR_H_
