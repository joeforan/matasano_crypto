#include "challenge7.h"
#include "matasano.h"
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#define FILENAME "challenge7.txt"

#define DEBUG_CHALLENGE_7

void try_challenge7()
{
    struct bigint bi;
    read_int(FILENAME,&bi);
    printf("Bi Size: %d\n",bi.n);

	//TODO move this into main initialisation
	ERR_load_crypto_strings();
	OpenSSL_add_all_algorithms();
	OPENSSL_config(NULL);

	//TODO actually use api - have api wrapper c file and call it from here

	//TODO move this into main cleanup
	EVP_cleanup();
	CRYPTO_cleanup_all_ex_data();
	ERR_free_strings();
}
