#include "challenge7.h"

#include <stdio.h>
#include <stdlib.h>

#include "matasano.h"
#include "openssl_wrapper.h"

#define FILENAME "challenge7.txt"

#define DEBUG_CHALLENGE_7

void try_challenge7()
{
    struct bigint bi;
	char * outstr = NULL;
	int n = 0;
	int bufsize = 0;
    read_int(FILENAME,&bi);
    printf("Bi Size: %d\n",bi.n);

	decrypt_bignum(&bi,(unsigned char *)"YELLOW SUBMARINE",&outstr,&bufsize,&n);

	if(outstr != NULL){
		printf("Decoded:\n=============\n%s\n=============\n",
			   outstr);
		free(outstr);
		printf("Buffer Size: %d\nText len: %d\n",
			   bufsize,n);
	}else{
		printf("Failed to decode");
	}
}
