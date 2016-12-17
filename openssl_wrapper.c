#include "openssl_wrapper.h"
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

void openssl_init()
{
	ERR_load_crypto_strings();
	OpenSSL_add_all_algorithms();
	OPENSSL_config(NULL);
}

void openssl_cleanup()
{
	EVP_cleanup();
	CRYPTO_cleanup_all_ex_data();
	ERR_free_strings();
}

void decrypt_bignum(const struct bigint * ciphertext,
                    const unsigned char * key,
                    char ** plaintext,
                    int * buffer_size,
					int * plaintext_len)
{
	EVP_CIPHER_CTX *ctx;

	int len;

	/* Create and initialise the context */
	if(!(ctx = EVP_CIPHER_CTX_new())) {
		printf("Failure calling EVP_CIPHER_CTX_new\n");
		goto cleanup;
	}

	/* Initialise the decryption operation. IMPORTANT - ensure you use a key
	 * and IV size appropriate for your cipher
	 * In this example we are using 256 bit AES (i.e. a 256 bit key). The
	 * IV size for *most* modes is the same as the block size. For AES this
	 * is 128 bits */
	if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL)){
		printf("Failure calling EVP_DecryptInit_ex()");
		goto cleanup;
	}


	*buffer_size = ciphertext->n + EVP_CIPHER_CTX_block_size(ctx);
	*plaintext = (char*)calloc(*buffer_size,1);


	/* Provide the message to be decrypted, and obtain the plaintext output.
	 * EVP_DecryptUpdate can be called multiple times if necessary
	 */
	if(1 != EVP_DecryptUpdate(ctx,
							  (unsigned char*)*plaintext,
							  &len,
							  ciphertext->bytes,
							  ciphertext->n)){
		printf("Failure calling EVP_DecryptUpdate\n");
		goto cleanup_free;
	}
	if(len > *buffer_size){
		printf("Warning!! Buffer overflow");
		goto cleanup_free;
	}
	*plaintext_len = len;
		
	/* Finalise the decryption. Further plaintext bytes may be written at
	 * this stage.
	 */
	if(1 != EVP_DecryptFinal_ex(ctx, (unsigned char*)(*plaintext + len), &len)){
		printf("Failure calling EVP_DecryptFinal_ex()\n");
		goto cleanup_free;
	}
	*plaintext_len += len;
	if(*plaintext_len > *buffer_size){
		printf("Warning!! Buffer overflow");
		goto cleanup_free;
	}
	if(*plaintext_len < *buffer_size){
		(*plaintext)[(*plaintext_len)] = '\0';
	}

	/* Clean up */
	goto cleanup;
	
cleanup_free:
	free(*plaintext);
	*plaintext = NULL;
	*plaintext_len = 0;
	*buffer_size = 0;

cleanup:
	EVP_CIPHER_CTX_free(ctx);
}
