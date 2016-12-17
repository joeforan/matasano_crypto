#ifndef _OPEN_SSL_WRAPPER_H_
#define _OPEN_SSL_WRAPPER_H_

#include "matasano.h"

void openssl_init();
void openssl_cleanup();

void decrypt_bignum(const struct bigint * ciphertext,
                    const unsigned char * key,
                    char ** plaintext,
                    int * buffer_size,
					int * plaintext_len);

#endif // _OPEN_SSL_WRAPPER_H_
