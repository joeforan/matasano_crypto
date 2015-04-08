#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "matasano.h"

static void test(const char * str1, const char * str2, const char * expected);
static void xor(const struct bigint * a, const struct bigint * b,
				struct bigint * res);

void test_fixedXor()
{
	test("1c0111001f010100061a024b53535009181c",
		 "686974207468652062756c6c277320657965",
		 "746865206b696420646f6e277420706c6179");
}

static void test(const char * str1, const char * str2, const char * expected)
{
	struct bigint a;
	struct bigint b;
	struct bigint c;
	char * res = NULL;
	hex2val(str1,&a);
	hex2val(str2,&b);
	xor(&a,&b,&c);

	val2hex(&c,&res);

	if(strcmp(expected,res) != 0){
		printf("Failure Xoring\n           %s  with\n           %s\n   Result: %s\n Expected: %s\n",
			   str1,str2,res,expected);
	}else{
		printf("Xored\n           %s  with\n           %s\n   Result: %s\nOK!\n",
			   str1,str2,res);
	}

	free(a.bytes);
	free(b.bytes);
	free(c.bytes);
	free(res);
}


void xor(const struct bigint * a, const struct bigint * b,
		 struct bigint * res)
{
	int i;
	res->n = a-> n > b->n ? a->n : b->n;

	res->bytes = (uint8_t*)malloc(res->n);

	for(i=res->n-1; i>=0; i--){
		uint8_t aval = i < a->n ? a->bytes[i] : 0;
		uint8_t bval = i < b->n ? b->bytes[i] : 0;

		res->bytes[i] = aval ^ bval;
	}
}
